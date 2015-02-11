#include "fsm.h"
#include "USART2.h"
#include "userbutton.h"

/**
 * Handles all signals from the operators.
 */
void USART2_callback_fn(uint8_t byte)
{
	//TODO: Triggers on input from the user. Use this to pass the following flags:
	//a - arriving
	//h - hold for maintenance
	//c - clear the maintenance state

	if(byte == 'a'){
		arriving = 1;
	}
	else if(byte == 'h'){
		hold = 1;
	}
	else if(byte == 'c'){
		clear = 1;
	}
}

/**
 * Handles the "crosswalk button"
 * - When the user button is pressed, it is registered
 * as someone wanting to cross the crosswalk.
 */
void userbutton_callback_fn(void)
{
	person = 1;
}

/**
 * Changes states depending on flags on each systick.
 */
void handle_systick(void){
	//TODO: Add in all state-changing logic!!!
}

void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

	/* Set up the PWM for use with the servo motor.	 */
	PWM_init();

	/* Configure user pushbutton and call pushbutton_callback_fn when button press-released */
 	userbutton_init(userbutton_callback_fn);

	/* initialize the finite state machine */
	fsm_init();

	/* Enable interrupts - do this after initializing the system */
	__asm ("  cpsie i \n" );

	/* Wait here forever - everything is now interrupt driven */
	while(1)
	{
		;;;
	}
}
