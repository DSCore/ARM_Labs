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

/* Initialize the finite state machine */
void fsm_init(void);

/* Acquire a lock on the fsm before accessing/altering state */
#define FSM_LOCK_FAILED   (0)
#define FSM_LOCK_ACQUIRED (1)
uint8_t fsm_lock(void);
void fsm_unlock(void);

uint8_t getPerson(void);
void setPerson(uint8_t);
uint8_t getArriving(void);
void setArriving(uint8_t);
uint8_t getHold(void);
void setHold(uint8_t);
uint8_t getClear(void);
void setClear(uint8_t);
uint8_t getTimer(void);
void setTimer(uint8_t val);

/* Query FSM state - only do this when you have acquired the lock! */
state_t fsm_get_state(void);

/* Alter FSM state - only do this when you have acquired the lock! */
void fsm_set_state(state_t new_state);
