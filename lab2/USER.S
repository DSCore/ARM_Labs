/* USER.S
 *
 * Target: STM32F4-Discovery w/ STM32F407VGT6 processor
 * Handles the user input from the user input button
 *
 * Written by Devon Cormack for ENGS62, Microprocessors.
 *
 */
	.syntax unified   			/* Use unified style assembly syntax */
	.thumb            			/* Cortex-M3 only supports Thumb code */

    /* Make these routines visible to other modules */
    .global USER_init
    .global USER_count

    .text

/*
 * USER_init
 *
 * Initialze the GPIOD port to read the USER button attached to PD12-PD15
 */
USER_init:
	push {r0-r2, lr}
	/* Enable GPIOD Peripheral Clock (using read-modify-write) */
  	.equ RCC_AHB1ENR,          0x40023830  /* ([1]-2.3 Table 2) */
  	.equ RCC_AHB1ENR_GPIOAEN,  0x01        /* ([1]-6.3.12) */
  	ldr r0, =RCC_AHB1ENR          /* r0=addr of GPIO Clock Enable Register */
  	ldr r1, =RCC_AHB1ENR_GPIOAEN  /* r1=Port A Enable Bit */
  	ldr r2, [r0]
  	orr r2, r2, r1
  	str r2, [r0]			      /* Store #0x01 into address 0x40023830 */

  	/* Configure GPIOs connected to button as input (reset state) */
   	.equ GPIOA_MODER,          0x40020000   /* ([1]-2.3, [1]-7.4.1) */
  	.equ GPIOA_MODER_BUTTON_IN, 0x00 /* Value to reset port 0 (button) to input */
  	.equ GPIOA_MODER_BUTTON_MASK, 0xffffff00
  	ldr r0, =GPIOA_MODER
  	ldr r1, =GPIOA_MODER_BUTTON_MASK
  	ldr r2, [r0]
  	and r2, r2, r1
  	ldr r1, =GPIOA_MODER_BUTTON_IN
  	orr r2, r2, r1
  	str r2, [r0]

  	/* Return */
  	pop {r0-r2, pc}

/* USER_count:
 *
 * Counts from 1 to 5 button presses
 * and outputs the results to the screen.
 *
 */
USER_count:
	push {r0-r3, lr}
	.equ GPIOA_IDR, 0x40020010
	.equ GPIOA_BUTTON_MASK, 0x00000001
USER_count_reset_counter:
	/* Initialize value of counter */
//	bl mutex_lock
	mov r3, #0
 USER_count_check_button:
 	/* Wait for the user to press a button */
	//Load in the port A data register to r2
	bl USER_debounce
	//Increment, print, repeat
	add r3, #1
	mov r0, r3
	add r0, #48 //Convert to ASCII representation
	bl USART2_send
	/* If the count is 5, jump to end
	 * else, jump back to waiting
	 */
	cmp r3, #5
	beq USER_count_end
	b USER_count_check_button
USER_count_end:
//	bl mutex_unlock
	pop {r0-r3, pc}

/* USER_debounce:
 *
 * Waits for a debounced input.
 * Returns 1 upon receiving a
 * debounced input.  //MAKE THIS WAIT FOR 16000 cycles per check!
 *
 */
 USER_debounce:
 	push {r0-r3, lr}
	.equ GPIOA_IDR, 0x40020010
	.equ GPIOA_BUTTON_MASK, 0x00000001
	.equ DEBOUNCED_REQUIREMENT, 20
	.equ CLOCKS_BETWEEN_CHECKS, 16000 //16000 clock cycles = 1 msec
USER_debounce_reset_counter:
	/* Initialize value of counter */
	mov r3, #0
 	/* Wait for the user to press a button */
	//Load in the port A data register to r2
USER_debounce_check_button:
	ldr r0, =CLOCKS_BETWEEN_CHECKS
	bl wait_n_clock
	ldr r0, =GPIOA_IDR
	ldr r2, [r0]
	//Get the value of the button into r2
	ldr r1, =GPIOA_BUTTON_MASK
	and r2, r1
	/* If the value of the button is 1 (depressed)
	 * increment the counter, else reset it */
	cmp r2, #1
	bne USER_debounce_reset_counter
	add r3, #1
	/* If we have enough in a row, return 1
	 * else, keep looking */
	ldr r0, =DEBOUNCED_REQUIREMENT
	cmp r3, r0
	bne USER_debounce_check_button
	mov r0, #1
	pop {r0-r3, pc}

	.end
