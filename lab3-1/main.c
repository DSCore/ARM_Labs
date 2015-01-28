/*
 * main.c
 */

#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "USART2.h"     /* assembly routines in USART.S */
#include "systick.h"	/* initialize and handle systick timer */

int main()
{
	/*
	 * Initialize the SYSTICK timer
	 */
	systick_init();

	/*
	 * Demonstrate use of LED.h/LED.c - modifies hardware registers using C
	 */
	LED_init();
	LED_update(LED_BLUE_ON);
	LED_update(LED_BLUE_OFF);
	LED_update(LED_RED_ON | LED_BLUE_ON | LED_ORANGE_ON | LED_GREEN_ON );

	/*
	 * Demonstrate use of in-line assembly - enable interrupts
	 */
	__asm ("  cpsie i \n" );


	/* Initialize the USART for 9600, 8N1, send '!' - calls into USART2.S */
	/* NOTE: must set USART2 interrupt config register to enable TX/RX interrupts */
	USART2_init();
	USART2_send('!');

	/* Wait here forever */
	while(1);

	/* We'll never reach this line */
	return 0;
}
