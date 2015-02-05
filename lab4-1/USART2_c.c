/*
 * USART2.c
 * By Devon Cormack, 01/28/15
 *
 */
#include "USART2_c.h"
#include "USART2.h"
#include "stm32f4xx.h"
#include "stdint.h"

/*
 * The USART2 Interrupt Service Routine
 */
void __attribute__ ((interrupt)) USART2_handler(void)
{
	/* This code receives input from the USART2
	 * interface and repeats it back to the user
	 * on the same interface.
	 */
	uint8_t c = USART2_recv();
	USART2_send(c);

	//Check if the input was one of our commands:
	//h - history
	if(c == 'h'){
		//Print the history
		printHistory();
	}
	if(c == 'a'){
		//Print the average of the history
		printAverageHistory();
	}
	if(c == 'c'){
		//Clear the history
		clearHistory();
	}

}

