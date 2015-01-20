/*
 * lab2.S - Interrupts and Mutual Exclusion
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor,
 *         ROM = 1M   @ 0x08000000
 *         RAM = 192k @ 0x20000000
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Linker hints for vector table functions */
	.type reset_handler, %function
	.type NMI_handler, %function
	.type fault_handler, %function
	.type default_handler, %function
	.type systick_handler, %function
	.type USART2_handler, %function

	.equ STACK_TOP, 0x20000100     /* 256 bytes of stack in RAM region */

	/*
	 * VECTOR TABLE
	 */
	.section vectors
	/* System Handlers */
	.word STACK_TOP
	.word reset_handler    /* What to do on power-on */
	.word NMI_handler      /* Non-maskable interrupt handler */
	.word fault_handler    /* Hardware fault handler */
	.word fault_handler    /* Memory manager handler (disabled) */
	.word fault_handler    /* Bus fault (disabled) */
	.word fault_handler    /* Usage fault (disabled) */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word 0                /* Reserved */
	.word default_handler  /* System service call */
	.word default_handler  /* Debug Monitor */
	.word default_handler  /* Reserved */
	.word default_handler  /* Pendable service call */
	.word systick_handler  /* Systick */
	/* Peripheral Handlers */
	.rept 38  /* Repeat this block 38 times */
	.word default_handler  /* ISRs 0-37 */
	.endr
	.word USART2_handler   /* ISR 38, USART2 handler */
	.rept 43
	.word default_handler  /* ISRs 39-81 */
	.endr

	/* Generic code section */
	.text

	/* In the event of a non-maskable interrupt, loop here forever */
NMI_handler:
	b NMI_handler

	/* In the event of a fault, loop here forever */
fault_handler:
	.equ ONE_SECOND, 1
	.equ GPIOD_BSRR_SET_14,   0x00004000  /* ([1]-7.4.7 */ /* red LED */

	/* Blink the red LED for 1 second */
	bl LED_toggle_red
	ldr r0, =ONE_SECOND
	bl waitn
	b fault_handler

	b fault_handler

	/* Address system tick event */
systick_handler:
	b systick_handler

	/* Address USART2 interrupts */
USART2_handler:
	b USART2_handler

	/* Default handler for un-handled exceptions/interrupts */
default_handler:
	b default_handler


/*
 * NVIC_init:
 *
 * Set the enable bit for Interrupt #38 (USART2).
 * See the Programming Manual section 4.3.2 page 196.
 *
 * NOTE: the NVIC configuration out of power-on/reset is sufficient
 */
NVIC_init:
	.equ NVIC_ISER1, 0xE000E104      /* Interrupt Set Enable Register 1 */
	.equ NVIC_ISER1_USART2_EN, 0x40  /* Enable USART2 Global Interrupts */
	push {r0,r1,lr}
	ldr r0, =NVIC_ISER1
	ldr r1, =NVIC_ISER1_USART2_EN
	str r1, [r0]
	pop {r0,r1,pc}

/*
 * reset_handler:
 *
 * This is the code we want to run out of reset/on start-up
 */
reset_handler:
	/* Global interrupt disable - power on/reset default */
	cpsid i

	/* Enable LEDs and turn them all on */
  	bl LED_init
  	ldr r0, =LED_all
  	bl LED_on

  	/* Initialize USART2 9600 baud, 8 data, no parity, 1 stop */
  	bl USART2_init

  	/* Enable peripheral interrupts, see routine above */
  	bl NVIC_init

  	/* Turn all LEDs off */
  	bl LED_off

  	/* Just toggle green LED on */
  	bl LED_toggle_grn

	/* Global interrupt enable */
  	cpsie i

	/* Show the USART2 peripheral is configured and connected */
  	mov r0, #'!'
  	bl USART2_send

endloop:
	b endloop  /* spin here forever in a tight loop */

/*
 * waitn:
 *
 * Waits for n seconds
 * r0 contains n
 */
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



/*
 * wait_1_second:
 *
 * Waits for 1 second
 */
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


	.end


