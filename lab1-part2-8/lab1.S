/*
 * lab1.S - LEDs and USART2 in assembly on the STM32F4-Discovery Board
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 * REFERENCES:
 *
 * All documentation references written as "[#]-<section>" where:
 *
 * [1] = RM0090 STM32F407 Reference Manual.pdf
 * [2] = STM32F4 Discovery Schematic.pdf
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /*
     * On power-up the Cortex-M family processors expect a few pieces
     * of information to be pre-configured at a few pre-defined addresses.
     * Specifically, a table of "vectors" placed at the start of
     * memory (address 0x00000000) contains, in order:
     * 0x00000000 : Address of stack top in RAM
     * 0x00000004 : Address of code to run on start-up/reset
     * 0x00000008 : Address of a hardware fault handler routine
     *
     * The assembler directive ".type <func name>, %function" must
     * be provided for each routine listed in the vector table
     * to hint the linker to provide the correct target address for
     * routines written using "thumb" instructions
     *
     * In order to place the vector table where it belongs in memory
     * it is given the special section name "vectors". This is done
     * using the ".section" directive below so the linker knows which
     * chunk of code is the vector table and can place it where
     * required.  (See the STM32F407VG.ld linker script).
     */
	.type reset_handler, %function
	.type NMI_handler, %function
	.type fault_handler, %function
	.equ STACK_TOP, 0x20000100     /* 256 bytes of stack in RAM region */

	.section vectors
	.word STACK_TOP
	.word reset_handler    /* What to do on power-on */
	.word NMI_handler      /* Non-maskable interrupt handler */
	.word fault_handler    /* Hardware fault handler */



	/* Generic code section */
	.text


	/* Data section */
HelloString:
	.ascii "hello!!\n\r\0"
	.align 2 /* pad to 2^2 bit boundary. (4 bits) */

	/* In the event of a non-maskable interrupt, loop here forever */
NMI_handler:
	b NMI_handler

	/* In the event of a fault, loop here forever */
fault_handler:
	.equ ONE_SECOND, 1
	.equ GPIOD_BSRR_SET_14,   0x00004000  /* ([1]-7.4.7 */ /* red LED */

	/* Blink the red LED for 1 second */
	ldr r1, =ONE_SECOND
	ldr r0, =GPIOD_BSRR_SET_14
	bl LED_on_for_n

	ldr r0, =ONE_SECOND
	bl waitn

	b fault_handler


	/* This is the code we want to run out of reset/on start-up */
reset_handler:
	cpsid i					/* disable interupts - power-on default */

  	/* STEP 1: Enable GPIOD Peripheral Clock */
  	.equ RCC_AHB1ENR,          0x40023830  /* ([1]-2.3 Table 2) */
  	.equ RCC_AHB1ENR_GPIODEN,  0x08        /* ([1]-6.3.12) */

  	ldr r0, =RCC_AHB1ENR          /* r0=addr of GPIO Clock Enable Register */
  	ldr r1, =RCC_AHB1ENR_GPIODEN  /* r1=Port D Enable Bit */
  	str r1, [r0]			      /* Store #0x08 into address 0x40023830 */

  	/* STEP2: Configure GPIO connected to LED as output, push-pull */
   	.equ GPIOD_MODER,        0x40020C00  /* ([1]-2.3, [1]-7.4.1) */
  	.equ GPIOD_MODER_GP,     0x55000000  /* ([1]-7.4.1) */
  	ldr r0, =GPIOD_MODER
  	ldr r1, =GPIOD_MODER_GP
  	str r1, [r0]

  	/* HERE IS THE CODE THAT FORCES A FAULT */
  	/* Just load an essentially random value into the PC */
//  	ldr pc, =GPIOD_MODER

/* Test the receiving */
	bl controlLights

/* Execute the lights sequence 3 times */
	.equ NUM, 3
	mov r0, #0
	ldr r1, =NUM
loop3start:
	cmp r0,r1
	bge loop3end
	push {r0,r1}

	/* Turn each LED on for 1 second, then all on for 3 seconds. */
	.equ NUM_SECONDS, 1
	.equ THREE_SECONDS, 3
	.equ GPIOD_BSRR_SET_12,   0x00001000  /* ([1]-7.4.7 */ /* green LED */
	.equ GPIOD_BSRR_SET_13,   0x00002000  /* ([1]-7.4.7 */ /* orange LED */
	.equ GPIOD_BSRR_SET_15,   0x00008000  /* ([1]-7.4.7 */ /* blue LED */
	.equ GPIOD_BSRR_SET_OGB,   0x0000B000  /* orange, green, and blue LEDs */

	/* Turn the green LED on for 1 second */
	ldr r1, =NUM_SECONDS
	ldr r0, =GPIOD_BSRR_SET_12
	bl LED_on_for_n

	/* Turn the orange LED on for 1 second */
	ldr r1, =NUM_SECONDS
	ldr r0, =GPIOD_BSRR_SET_13
	bl LED_on_for_n

	/* Turn the blue LED on for 1 second */
	ldr r1, =NUM_SECONDS
	ldr r0, =GPIOD_BSRR_SET_15
	bl LED_on_for_n

	/* Turn all the LEDs on for 3 seconds */
	ldr r1, =THREE_SECONDS
	ldr r0, =GPIOD_BSRR_SET_OGB
	bl LED_on_for_n

	pop {r0,r1}
	add r0, #1
	b loop3start
loop3end:
	/* Send a test message */
	ldr r0, =HelloString
	bl print

endloop:
	b endloop  /* spin here forever in a tight loop */

/* FUNCTION DEFINTIONS */

/* Takes in a character from the terminal, and acts upon it. */
/* a- toggle all three LEDs */
/* g - toggle green LED */
/* o - toggle orange LED */
/* b - toggle blue LED */
/* e - exit control mode */
controlLights:
	.equ LOWCASE_E, 101
	push {r0,r1,lr}
loop:
	bl getCharacterInput
	bl toggleLED
	ldr r1, =LOWCASE_E
	cmp r0, r1
	beq endcontrolLights
	b loop
endcontrolLights:
	pop {r0,r1,pc}

/* Toggles all LEDs */
/* LED indicator is in r0 */
/* a- toggle all three LEDs */
/* g - toggle green LED */
/* o - toggle orange LED */
/* b - toggle blue LED */
toggleLED:
	.equ LOWCASE_A, 97
	.equ LOWCASE_G, 103
	.equ LOWCASE_O, 111
	.equ LOWCASE_B, 98

	.equ IDR, 0x40020C10

	.equ MASK_G, 0x00001000
	.equ MASK_O, 0x00002000
	.equ MASK_OUT_R, 0x0000B000
	.equ MASK_B, 0x00008000
	.equ MASK_A, 0x0000B000

	push {r0,r1,r2,r3,lr}

	/* Load the current state of the LEDs */
	ldr r1, =IDR
	ldr r2, [r1]

A:	ldr r1, =LOWCASE_A
	cmp r0, r1
	bne G
	/* Set the mask */
	ldr r1, =MASK_A
	b endToggleLED

G:	ldr r1, =LOWCASE_G
	cmp r0, r1
	bne O
	ldr r1, =MASK_G
	b endToggleLED

O:	ldr r1, =LOWCASE_O
	cmp r0, r1
	bne B
	ldr r1, =MASK_O
	b endToggleLED

B:	ldr r1, =LOWCASE_B
	cmp r0, r1
	bne doNothing
	ldr r1, =MASK_B
	b endToggleLED

endToggleLED:
	/* Store the mask in r0 for use later*/
	mov r0, r1
	/* Read the current state of the LED(s) */
	/* that we want to toggle */
	and r1,r2

	/* Shift so that the on ones will be turned off */
	mov r3, r1 /* Back up current state */
	mov r1, r1, lsl #16 /* Shift to reset */
	mov r2, r1 /* Save the off states in r2 */

	/* This will turn on the ones that were off */
	mov r1,r3 /* Restore current LED state to r1 */
	mvn r1, r1 /* Flip it */
	/* Mask out everything we aren't toggling */
	and r1, r0 /* The mask is in r0 from above */
	/* Mask out turning on the red LED */
	ldr r3, =MASK_OUT_R
	and r1,r3
	orr r1, r2
	/* Change the state of the LEDs by */
	/* writing it to GPIOD_BSRR */
	ldr r2, =GPIOD_BSRR
	str r1,[r2]
doNothing:
	pop {r0,r1,r2,r3,pc}

/* Gets an input character and echoes it back to the screen. */
/* Returns the character in r0 */
getCharacterInput:
	push {lr}
	/* Initialize the USART2 interface */
	bl USART2_init
	bl USART2_recv
	bl USART2_send
	pop {pc}


/*Print string that starts at address in r0 */
print:
	push {lr}
	/* Initialize the USART2 interface */
	bl USART2_init
startsend:
	ldrb r1,[r0] /* load the first character (at addr in r0 into r1) */
	cmp r1,#0
	beq endprint
	mov r3,r0
	mov r0,r1 /* move the character into r0 so it can be sent */
	bl USART2_send /* send the character */
	mov r0,r3
	add r0, 1
	ldrb r1, [r0]
	bl startsend
endprint:
	pop {pc}

/* Waits for n seconds */
/* r0 contains n */
waitn:
	/* enter function setup */
	push {r0,r1,lr}

	/* for loop where r0 is i and r1 is n */
	mov r1, r0 /* n = parameter n in r0*/
	mov r0, #0 /* i = 0 */
loopstart:
	cmp r0, r1 /* i < n */
	bge loopend1
		bl wait_1_second
	add r0, #1 /* i = i+1 */
	b loopstart

loopend1:
	/* leave function setup */
	pop {r0,r1,pc}

/* Waits for 1 second */
wait_1_second:
	/* enter function setup */
	push {r2,lr}

	/* Wait for 1 second */
	.equ BUSYWAIT_1_SECOND, 3200000
	ldr r2, =BUSYWAIT_1_SECOND
waitloop:
	sub r2, r2, #1
	cmp r2, #0
	bne waitloop

	/* leave function setup */
	pop {r2,pc}

/* Turns LED on or off based on the
	GPIOD parameter passed in r0 */
turnLED:
	push {r2,lr}
	.equ GPIOD_BSRR,          0x40020C18  /* ([1]-2.3, [1]-7.4.7) */

	/* Turn the LED on or off */
	ldr r2, =GPIOD_BSRR
	str r0, [r2] /* sets or resets GPIOD_BSRR */

	pop {r2,pc}

/* Turns an LED passed in on for n seconds */
/* r0 - LED set value */
/* r1 - n seconds */
LED_on_for_n:
		/* enter function setup */
	push {r0,r1,lr}

	/* Turn the LED on */
	bl turnLED

	/* Wait for n seconds */
	/* Since n was passed in in r1, pass it on */
	push {r0}
	mov r0, r1
	bl waitn
	pop {r0}

	/* Turn the LED off */
	mov r0, r0, lsl #16
	bl turnLED

	/* leave function setup */
	pop {r0,r1,pc}

	.end
