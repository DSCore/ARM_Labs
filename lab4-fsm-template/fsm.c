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

			/* Display usage information */
			USART2_putstr("FSM Rest\r\n");
			USART2_putstr("Press the space bar to turn on LEDs in a clockwise rotation\r\n");
			USART2_putstr("Press any other key to turn on LEDs in a counter-clockwise rotation\r\n");
			USART2_putstr("Press the user button to reset the FSM\r\n\n");

			/* Turn on all of the LEDs */
			LED_update( LED_ORANGE_ON | LED_RED_ON | LED_BLUE_ON | LED_GREEN_ON );
			break;

		case STATE_1:
			/* Turn on the orange LED only */
			LED_update( LED_ORANGE_ON | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_OFF );
			break;

		case STATE_2:
			/* Turn on the red LED only */
			LED_update( LED_ORANGE_OFF | LED_RED_ON | LED_BLUE_OFF | LED_GREEN_OFF );
			break;

		case STATE_3:
			/* Turn on the blue LED only */
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_ON | LED_GREEN_OFF );
			break;

		case STATE_4:
			/* Turn on the green LED only */
			LED_update( LED_ORANGE_OFF | LED_RED_OFF | LED_BLUE_OFF | LED_GREEN_ON );
			break;
		}
	}
}
