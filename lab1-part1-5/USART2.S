/*
 * USART2.S - USART2 support in assembly on the STM32F4-Discovery Board
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
    .global USART2_init
    .global USART2_send
    .global USART2_recv

    .text

/*
 * USART2_init
 *
 * Enable the GPIOD and USART2 peripherals.
 * Configure GPIOD Pin #5 as USART2_TX, GPIOD Pin #6 as USART2_RX.
 * Configure USART2 for 9600 baud, 8 data bits, no parity, 1 stop bit.
 */
USART2_init:
	push {r0-r3,lr}     /* Save r0-r3 so we can use them in this routine */

	/* Enable GPOID peripheral clock See: [1]-6.3.12 */
	.equ RCC_AHB1ENR, 0x40023830
	.equ RCC_AHB1ENR_GPIODEN, 0x00000008
	ldr r0, =RCC_AHB1ENR
	ldr r1, =RCC_AHB1ENR_GPIODEN
	ldr r2, [r0]     	/* read-modify-write */
	orr r2, r2, r1
	str r2, [r0]

	/* Enable USART2 peripheral clock See: [1]-6.3.16 */
	.equ RCC_APB1ENR, 0x40023840
	.equ RCC_APB1ENR_USART2EN, 0x00020000
	ldr r0, =RCC_APB1ENR
	ldr r1, =RCC_APB1ENR_USART2EN
	ldr r2, [r0]		/* read-modify-write */
	orr r2, r2, r1
	str r2, [r0]

	/*
	 * Configure GPIOD Pin 5 as:
	 *   Alternate function output, AF7
	 *   Push-pull
	 *   No pull-up/pull-down
	 *   2 MHz drive speed
	 *
	 * See [1]-7.4 and [3]-Table 8 (pp 60).
	 */

	 /* Set pin #5 as Alternate Function */
	.equ GPIOD_MODER, 0x40020C00
	.equ GPIOD_MODER_PIN5_RESET_MASK, 0xfffff3ff
	.equ GPIOD_MODER_PIN5_AF, 0x00000800
	ldr r0, =GPIOD_MODER
	ldr r1, =GPIOD_MODER_PIN5_RESET_MASK
	ldr r2, =GPIOD_MODER_PIN5_AF
	ldr r3, [r0]      /* Read current settings */
	and r3, r3, r1    /* Reset Pin 5 Mode bits */
	orr r3, r3, r2    /* Configure New Pin 5 Mode bits */
	str r3, [r0]      /* Update configuration */

	/* Repeat for pin #6 */
	.equ GPIOD_MODER_PIN6_RESET_MASK, 0xffffcfff
	.equ GPIOD_MODER_PIN6_AF, 0x00002000
	ldr r1, =GPIOD_MODER_PIN6_RESET_MASK
	ldr r2, =GPIOD_MODER_PIN6_AF
	ldr r3, [r0]
	and r3, r3, r1
	orr r3, r3, r2
	str r3, [r0]

	/* Default settings on reset are push-pull, 2MHz, no pull-up/pull-down */

	/* Pin 6 is configured as input floating on reset - rely on defaults */

	/* Set pin #5 and #6 MUX to Alternate Function #7 (USART2 TX) */
	/* See [3]-Table 8 (pp 60). */
	.equ GPIOD_AFRL, 0x40020C20
	.equ GPIOD_AFRL_PIN5_MASK, 0xff0fffff
	.equ GPIOD_AFRL_PIN5_AF7,  0x00700000
	.equ GPIOD_AFRL_PIN6_MASK, 0xf0ffffff
	.equ GPIOD_AFRL_PIN6_AF7, 0x07000000
	ldr r0, =GPIOD_AFRL
	ldr r1, =GPIOD_AFRL_PIN5_MASK
	ldr r2, =GPIOD_AFRL_PIN5_AF7
	ldr r3, [r0]      /* Read current settings */
	and r3, r3, r1    /* Reset Pin 5 AFRL bits */
	orr r3, r3, r2    /* Configure Pin 5 AFRL for AF7 */
	ldr r1, =GPIOD_AFRL_PIN6_MASK
	ldr r2, =GPIOD_AFRL_PIN6_AF7
	and r3, r3, r1    /* Reset Pin 6 AFRL bits */
	orr r3, r3, r2    /* Configure Pin 6 AFRL for AF7 */
	str r3, [r0]      /* Update configuration */

	/* Enable the USART peripheral in the USART (separate from clock enable) */
	.equ USART2_CR1, 0x4000440C
	.equ USART2_CR1_EN, 0x00002000
	ldr r0, =USART2_CR1
	ldr r1, =USART2_CR1_EN
	str r1, [r0]

	/* Set the baudrate to 9600. */
	.equ USART2_BRR, 0x40004408
	.equ USART2_BRR_9600, 1667  /* SYSCLK = 16 MHz: 16,000,000 / 9,600 = 1667 */
	ldr r0, =USART2_BRR
	ldr r1, =USART2_BRR_9600
	str r1, [r0]

	/* Enable transmitter and receiver */
	.equ USART2_CR1_TXRX_EN, 0x0000000C
	ldr r0, =USART2_CR1
	ldr r1, =USART2_CR1_TXRX_EN
	ldr r2, [r0]     /* read-modify-write - don't step on enable bit! */
	orr r2, r2, r1
	str r2, [r0]

	pop {r0-r3,pc}


/*
 * USART2_send
 *
 * Send value passed in register r0 out the USART2 communications port
 */
 	.equ USART2_SR, 0x40004400
 	.equ USART2_SR_TXE_MASK, 0x00000080
 	.equ USART2_DR, 0x40004404
USART2_send:
 	push {r1,r2,r3,lr}  /* Get a set of working registers */

 	/* Wait for USART transmit shift register to be empty */
 check_txe:
 	ldr r1, =USART2_SR
 	ldr r2, [r1]
 	ldr r3, =USART2_SR_TXE_MASK
 	and r2, r2, r3
 	cmp r2, #0
 	beq check_txe
 	/* Load byte to send into transmit shift register (transmit) */
 	ldr r1, =USART2_DR
 	str r0, [r1]

	pop {r1,r2,r3,pc}	/* Recover working registers */

/*
 * USART2_recv
 *
 * Polling receive - wait for a character to arrive from the USART,
 * then return received byte to caller in r0
 */
USART2_recv:
	push {lr}
	/* Insert your code here */
	pop {pc}

	.end
