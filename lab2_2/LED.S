/*
 * LED.S - LED support in assembly on the STM32F4-Discovery Board
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
 * [3] = STM32F407xx Data Sheet.pdf
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Make these routines visible to other modules */
    .global LED_init
    .global LED_on
    .global LED_off
    .global LED_toggle_grn
    .global LED_toggle_red

    /* Constants visible to the world ( due to ".global" below ) */
    .equ LED_grn, 0x1
    .equ LED_org, 0x2
    .equ LED_red, 0x4
    .equ LED_blu, 0x8
    .equ LED_all, 0xf

    .global LED_grn
    .global LED_red
    .global LED_org
    .global LED_blu
    .global LED_all

    .text

/*
 * LED_init
 *
 * Initialze the GPIOD port to drive the LEDs attached to PD12-PD15
 */
LED_init:
	push {r0-r2, lr}
	/* Enable GPIOD Peripheral Clock (using read-modify-write) */
  	.equ RCC_AHB1ENR,          0x40023830  /* ([1]-2.3 Table 2) */
  	.equ RCC_AHB1ENR_GPIODEN,  0x08        /* ([1]-6.3.12) */
  	ldr r0, =RCC_AHB1ENR          /* r0=addr of GPIO Clock Enable Register */
  	ldr r1, =RCC_AHB1ENR_GPIODEN  /* r1=Port D Enable Bit */
  	ldr r2, [r0]
  	orr r2, r2, r1
  	str r2, [r0]			      /* Store #0x08 into address 0x40023830 */

  	/* Configure GPIOs connected to LEDs as output, push-pull */
   	.equ GPIOD_MODER,          0x40020C00   /* ([1]-2.3, [1]-7.4.1) */
  	.equ GPIOD_MODER_LEDS_OUT, 0x55000000   /* ([1]-7.4.1) */
  	.equ GPIOD_MODER_LEDS_MASK, 0x00fffffff /* Reset bits 23-31 */
  	ldr r0, =GPIOD_MODER
  	ldr r1, =GPIOD_MODER_LEDS_MASK
  	ldr r2, [r0]
  	and r2, r2, r1
  	ldr r1, =GPIOD_MODER_LEDS_OUT
  	orr r2, r2, r1
  	str r2, [r0]

  	/* Return */
  	pop {r0-r2, pc}

/*
 * LED_on:
 *
 * Turn on the LEDs flagged in register r0
 * r0- If bit 12 is 1, then the bit 12 will be turned on.
 */
LED_on:
	push {r0, r2, lr}
	.equ MASK_LED_ON, 0xFFFF0000

	ldr r2, =MASK_LED_OFF
	and r0, r2
	bl LED_change_state

	pop {r0, r2, pc}


/*
 * LED_off:
 *
 * Turn off the LEDs flagged in register r0
 */
LED_off:
	push {r0, r2, lr}
	.equ MASK_LED_OFF, 0x0000FFFF

	ldr r2, =MASK_LED_OFF
	and r0, r2
	bl LED_change_state

	pop {r0, r2, pc}

/*
 * LED_change_state:
 *
 * changes the state of the LEDs flagged in r0
 *
 */
LED_change_state:
	//GPIO Port D Set/Reset Register
	.equ GPIOD_BSRR,          0x40020C18  /* ([1]-2.3, [1]-7.4.7) */

	/* Change the LED state */
	ldr r2, =GPIOD_BSRR
	str r0, [r2] /* sets or resets GPIOD_BSRR */

/*
 * LED_toggle_grn:
 *
 * Toggle the state of the green LED
 */
LED_toggle_grn:
	push {r0, r1, lr}
	.equ MASK_G, 0x00001000
	ldr r0, =MASK_G
	bl ToggleLED
	/* Print out if the LED was turned on or off */
	ldr r0, =MASK_G
/* Print funcitonality
	ldr r1, =GREENON
	ldr r2, =GREENOFF
	bl printOnOff */
	pop {r0,r1, pc}

/*
 * LED_toggle_red:
 *
 * Toggle the state of the green LED
 */
LED_toggle_red:
	push {r0, r1, lr}
	.equ MASK_R,   0x00004000  /* ([1]-7.4.7 */ /* red LED */
	ldr r0, =MASK_R
	bl ToggleLED
	/* Print out if the LED was turned on or off */
	ldr r0, =MASK_R
/* Print funcitonality
	ldr r1, =GREENON
	ldr r2, =GREENOFF
	bl printOnOff */
	pop {r0,r1, pc}

/*
 *ToggleLED:
 *
 *Toggles the LEDs based on the mask passed in r0
 *
 */
ToggleLED:
	push {r0,r1,r2,r3,lr}

	/* Load the current state of the LEDs */
	.equ IDR, 0x40020C10
	ldr r1, =IDR
	ldr r2, [r1]

	/* Read the current state of the LED(s) */
	/* that we want to toggle */
	and r2,r0 /* Mask out the ones we want */
	mov r1, r2

	/* Shift so that the on ones will be turned off */
	mov r3, r1 /* Back up current state */
	mov r1, r1, lsl #16 /* Shift to reset bits */
	mov r2, r1 /* Save the bits to reset in r2 */

	/* This will turn on the ones that were off */
	mov r1,r3 /* Restore current LED state to r1 */
	mvn r1, r1 /* Flip it */
	/* Mask out everything we aren't toggling */
	and r1, r0 /* The mask is in r0 from above */
	/* Mask out turning on the red LED */
	.equ MASK_OUT_R, 0x0000B000
	ldr r3, =MASK_OUT_R
	and r1,r3
	orr r1, r2
	/* Change the state of the LEDs by */
	/* writing it to GPIOD_BSRR */
	mov r0, r1
	bl LED_change_state

	pop {r0,r1,r2,r3,pc}

	.end
