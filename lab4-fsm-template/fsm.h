#include <stdint.h>

typedef enum {
	STATE_RESET = 0,
	STATE_TRAFFICFLOW,
	STATE_YLWLIGHT,
	STATE_REDLIGHT,
	STATE_GATECLOSED,
	STATE_MAINTON,
	STATE_MAINTOFF,
	STATE_GATEOPEN,
	STATE_WALK
} state_t;

/*
 * State signal flags
 * 1 = triggered
 * 0 = not triggered
 */
uint8_t arriving = 0;
uint8_t hold = 0;
uint8_t clear = 0;
uint8_t person = 0;

/* Initialize the finite state machine */
void fsm_init(void);

/* Acquire a lock on the fsm before accessing/altering state */
#define FSM_LOCK_FAILED   (0)
#define FSM_LOCK_ACQUIRED (1)
uint8_t fsm_lock(void);
void fsm_unlock(void);

/* Query FSM state - only do this when you have acquired the lock! */
state_t fsm_get_state(void);

/* Alter FSM state - only do this when you have acquired the lock! */
void fsm_set_state(state_t new_state);
