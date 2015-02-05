#include "fsm.h"
#include "USART2.h"
#include "userbutton.h"

void USART2_callback_fn(uint8_t byte)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		switch( fsm_get_state() )
		{
		case STATE_RESET:
		case STATE_4:
			if( byte == ' ')
				fsm_set_state(STATE_1);
			else
				fsm_set_state(STATE_3);
			break;
		case STATE_1:
			if( byte == ' ')
				fsm_set_state(STATE_2);
			else
				fsm_set_state(STATE_4);
			break;
		case STATE_2:
			if( byte == ' ')
				fsm_set_state(STATE_3);
			else
				fsm_set_state(STATE_1);
			break;
		case STATE_3:
			if( byte == ' ')
				fsm_set_state(STATE_4);
			else
				fsm_set_state(STATE_2);
			break;
		default:
			fsm_set_state(STATE_RESET);
			break;
		}
		fsm_unlock();
	}
}

void userbutton_callback_fn(void)
{
	if( fsm_lock() == FSM_LOCK_ACQUIRED )
	{
		fsm_set_state(STATE_RESET);
		fsm_unlock();
	}
}

void main(void)
{
	/* Set up the USART2 9600-8N1 and to call USART2_callback_fn when new data arrives */
	USART2_init(USART2_callback_fn);

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
