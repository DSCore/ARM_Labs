/*
 * main.c
 */

#include "stdint.h"     /* uint32_t, etc... */
#include "stm32f4xx.h"  /* Useful definitions for the MCU */
#include "LED.h"        /* C routines in LED.c */
#include "USART2.h"     /* assembly routines in USART.S */
#include "USART2_c.h"
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

	uint32_t test_val = 785904457;
	printHex(785904457);

	/* Wait here forever */
	while(1);

	/* We'll never reach this line */
	return 0;
}

/*
 * void printHex(uint32_t value):
 *
 * Prints the hexadecimal representation of value.
 */
void printHex(uint32_t value){
	//Print out 0x
	uint8_t zero = 48;
	USART2_send(48);
	uint8_t x = 120;
	USART2_send(x);

	static char hex_digits[] = "0123456789ABCDEF";

	//Print out the hexadecimal representation of the number
	uint32_t mask = 0xF0000000;
	int i;
	for( i = 7; i >= 0; i-- ){
		//Mask out all but the highest 4 bits
		uint32_t masked_value = value & mask;
		//Shift off all but the highest 4 bits
		masked_value = masked_value >> 4*i;
		mask = mask >> 4;
		//Print out the hexadecimal representation of the value
		USART2_send(hex_digits[masked_value]);
	}
}
