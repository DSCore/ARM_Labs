#include <fsm.h>

#include <LED.h>
#include <mutex.h>
#include <USART2.h>

static uint32_t fsm_mutex;

static state_t state = STATE_RESET;

void fsm_init(void)
{
	mutex_init(&fsm_mutex);
	while( fsm_lock() != FSM_LOCK_ACQUIRED );
	fsm_set_state(STATE_RESET);
	fsm_unlock();
}

uint8_t fsm_lock(void)
{
	uint8_t retval = FSM_LOCK_FAILED;

	if( mutex_lock(&fsm_mutex) == MUTEX_LOCKED )
		retval = FSM_LOCK_ACQUIRED;

	return retval;
}

void fsm_unlock(void)
{
	mutex_unlock(&fsm_mutex);
}

state_t fsm_get_state(void)
{
	return state;
}

void fsm_set_state(state_t new_state)
{
	/* only do this if the FSM has been locked! */
	if( fsm_mutex == MUTEX_LOCKED )
	{
		state = new_state;

		switch( state )
		{
		case STATE_RESET:
		default:
			/* Initialize the LEDs */
			LED_init();

			break;

		case STATE_TRAFFICFLOW:
			/* Turn on the GREEN LED only */
			USART2_putstr("DON'T WALK!");
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON );
			break;

		case STATE_YLWLIGHT:
			/* Turn on the YELLOW(ORANGE) LED only */
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF );
			break;

		case STATE_REDLIGHT:
			/* Turn on the RED LED only */
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF );
			break;

		case STATE_GATECLOSED:
			/* Close the gate by moving the servo */
			PWM_to_NinetyDeg();
			/* Send the gate-closed message to the substation */
			USART2_putstr("GATE CLOSED");
			break;

		case STATE_MAINTON:
			/* Turn the BLU LED on, leave the rest alone */
			LED_update(LED_BLUE_ON);
			break;

		case STATE_MAINTOFF:
			/* Turn the BLU LED on, leave the rest alone */
			LED_update( LED_BLUE_OFF);
			break;

		case STATE_GATEOPEN:
			/* Open the gate */
			PWM_to_ZeroDeg();
			break;

		case STATE_WALK:
			/* Print out walk */
			//TODO: Print out walk on USART
			USART2_putstr("WALK!");
			break;
		}
	}
}
