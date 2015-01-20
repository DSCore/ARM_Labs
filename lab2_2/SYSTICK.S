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

    /* Make these routines visible to other modules */
    .global systick_init

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
 * systick_init
 *
 * Initialzes the systick handling process.
 */systick_init:
	push {r0-r2, lr}

	/* Fill in this routine */

  	/* Return */
  	pop {r0-r2, pc}
