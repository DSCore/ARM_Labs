#include "fsm.h"
#include "USART2.h"
#include "USART3.h"
#include "userbutton.h"
#include "Wifly.h"
#include "systick.h"

void USART2_callback_fn(uint8_t byte)
{
	if(fsm_get_state() == STATE_1 || fsm_get_state() == STATE_RESET){
		USART3_putchar(byte); //forward to wifi module
	}
}

void USART3_callback_fn(uint8_t byte)
{
	if(fsm_get_state() == STATE_1 || fsm_get_state() == STATE_RESET){
		USART2_putchar(byte); //forward to console
	}
	else if(fsm_get_state() == STATE_2){
		//Store the received byte in the Ping structure
		Wifly_Receive_Ping(byte);
	}
}

void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		if(fsm_get_state() == STATE_1 || fsm_get_state() == STATE_RESET){
			fsm_set_state(STATE_2);
		}
		else if(fsm_get_state() == STATE_2 || fsm_get_state() == STATE_2_READ){
			fsm_set_state(STATE_1);
		}
		else{
			USART2_putstr("Userbutton: Bad state!\n\r\0");
		}
		fsm_unlock();
	}
}

void systick_callback_fn(){
	if(fsm_lock() == FSM_LOCK_ACQUIRED){
		if(fsm_get_state() == STATE_2){
			fsm_set_state(STATE_2_READ);
		}
		else if(fsm_get_state() == STATE_2_READ){
			fsm_set_state(STATE_2);
		}
		else{
			USART2_putstr("BAD STATE IN SYSTICK CALLBACK \n\r\0");
		}
		fsm_unlock();
	}
}

void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

	/* Set up the USART3 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART3_init(USART3_callback_fn);

	/* Set up and initialize the LEDs. */
	LED_init();

	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
 	userbutton_init(userbutton_callback_fn);

 	/* Initialize the systick timer with its callback function */
 	systick_init(systick_callback_fn);
 	set_systick_disabled();

	/* initialize the finite state machine */
	fsm_init();

	//Test ping the server
	Wifly_Send_Ping();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	//Put the wifi module into command mode
//	USART3_putchar('$');
//	USART3_putchar('$');
//	USART3_putchar('$');
//	USART2_putchar('$');
//	USART2_putchar('$');
//	USART2_putchar('$');//	USART2_putstr("$$$\n\r\0");

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}


/*
 * void printHex(uint32_t value):
 *
 * Prints the hexadecimal representation of value.
 */
void printHex(uint32_t value){
	//Print out 0x
	uint8_t zero = 48;
	USART2_putchar(48);
	uint8_t x = 120;
	USART2_putchar(x);

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
		USART2_putchar(hex_digits[masked_value]);
	}
	USART2_putchar('\n');
	USART2_putchar('\r');
}

