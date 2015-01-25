/*
 * SYSTICK.S - SYSTICK support in assembly on the STM32F4-Discovery Board
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

	.equ CS, 0xE000E010 /* Control and Status register */
	.equ RV, 0xE000E014 /* Reload Value register */
	.equ CV, 0xE000E018 /* Current Value register */
					/* We will not be using the Configuration register */
	.equ WAIT_1_SECOND, 3200000 /* Number of processor counts in 1 second for 16 MHz processor */

    /* Make these routines visible to other modules */
    .global systick_init

    .text

/*
 * systick_init
 *
 * Initialzes the systick handling process.
 */systick_init:
	push {r0-r2, lr}
	//Disable the interrupts until setup.
	cpsid i

	//Load 0 into CS to initially disable the timer
	ldr r2, =CS
	mov r0, #0
	str r0, [r2]

	//Load a number into RV corresponding to the number
	//of processor cycles to count before causing interrupt.
	ldr r2, =RV
	ldr r0, =WAIT_1_SECOND
	str r0, [r2]

	//Load 0 into CV to clear the current value
	ldr r2, =CV
	mov r0, #0
	str r0, [r2]

	//Load 7 into CS to start interrupts.
	ldr r2, =CS
	mov r0, #7
	str r0, [r2]

	//Re-enable interrupts.
	cpsie i

  	/* Return */
  	pop {r0-r2, pc}
