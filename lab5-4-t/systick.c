/*
 * systick.c
 * By Devon Cormack 01/27/15
 */
#include "systick.h"
#include "stm32f4xx.h"
#include "fsm.h"
#include "LED.h" //For the LED code and values.

static const int WAIT_1_MILLISECOND = 16000;

/* module-global pointer to the callback function for rx'd ticks */
/* populated in init function. */
static void(*rx_callback_fn)(void);

///*
// * void systick_init(void)
// *
// * Set the values of CS, CR, and RV to initialize and enable SYSTICK.
// *
// */
//void systick_init(void){
//	//Disable interrupts for setup.
//	__asm ("  cpsid i \n" );
//	//Initially disable the timer
//	SYSTICK->CS = 0;
//	//Load a number into RV corresponding to the number
//	//of processor cycles between interrupts.
//	SYSTICK->RV = WAIT_1_SECOND;
//	//Load 0 into CV to clear current value
//	SYSTICK->CV = 0;
//	//Load 7 into CS to start interrupts again.
//	SYSTICK->CS = 7;
//	//Re-enable interrupts
//	__asm ("  cpsie i \n" );
//}

/**
 * void systick_init(void(*USART2_rx_callback)(uint8_t byte))
 *
 * Set the values of CS,CR, and RV to initialize and enable
 * SYSTICK using the passed in callback function
 *
 */
void systick_init(void(*systick_rx_callback)(void)){

	//Assign callback function
	rx_callback_fn = systick_rx_callback;

	//Disable interrupts for setup.
	__asm ("  cpsid i \n" );
	//Initially disable the timer
	SYSTICK->CS = 0;
	//Load a number into RV corresponding to the number
	//of processor cycles between interrupts.
	SYSTICK->RV = 30*WAIT_1_MILLISECOND;
	//Load 0 into CV to clear current value
	SYSTICK->CV = 0;
	//Load 7 into CS to start interrupts again.
	SYSTICK->CS = 7;
	//Re-enable interrupts
	__asm ("  cpsie i \n" );
}

void systick_reinit(){
	//Disable interrupts for setup.
		__asm ("  cpsid i \n" );
		//Initially disable the timer
		SYSTICK->CS = 0;
		//Load a number into RV corresponding to the number
		//of processor cycles between interrupts.
		SYSTICK->RV = 30*WAIT_1_MILLISECOND;
		//Load 0 into CV to clear current value
		SYSTICK->CV = 0;
		//Load 7 into CS to start interrupts again.
		SYSTICK->CS = 7;
		//Re-enable interrupts
		__asm ("  cpsie i \n" );
}

void set_systick_time(uint32_t time_in_secs){
		//Disable interrupts for setup.
		__asm ("  cpsid i \n" );
		//Initially disable the timer
		SYSTICK->CS = 0;
		//Load a number into RV corresponding to the number
		//of processor cycles between interrupts.
		SYSTICK->RV = WAIT_1_MILLISECOND*1000*time_in_secs;
		//Load 0 into CV to clear current value
		SYSTICK->CV = 0;
		//Load 7 into CS to start interrupts again.
		SYSTICK->CS = 7;
		//Set timer flag to indicate non-standard timer setting
//		setTimer(1);

		//Re-enable interrupts
		__asm ("  cpsie i \n" );
}

void set_systick_time_millis(uint32_t millis){
		//Disable interrupts for setup.
		__asm ("  cpsid i \n" );
		//Initially disable the timer
		SYSTICK->CS = 0;
		//Load a number into RV corresponding to the number
		//of processor cycles between interrupts.
		SYSTICK->RV = WAIT_1_MILLISECOND*millis;
		//Load 0 into CV to clear current value
		SYSTICK->CV = 0;
		//Load 7 into CS to start interrupts again.
		SYSTICK->CS = 7;
		//Set timer flag to indicate non-standard timer setting
//		setTimer(1);

		//Re-enable interrupts
		__asm ("  cpsie i \n" );
}

void set_systick_time_clocks(uint32_t clock_cycles){
		//Disable interrupts for setup.
		__asm ("  cpsid i \n" );
		//Initially disable the timer
		SYSTICK->CS = 0;
		//Load a number into RV corresponding to the number
		//of processor cycles between interrupts.
		SYSTICK->RV =clock_cycles;
		//Load 0 into CV to clear current value
		SYSTICK->CV = 0;
		//Load 7 into CS to start interrupts again.
		SYSTICK->CS = 7;
		//Set timer flag to indicate non-standard timer setting
//		setTimer(1);

		//Re-enable interrupts
		__asm ("  cpsie i \n" );
}

void set_systick_disabled(){
	//Disable interrupts for setup.
	__asm ("  cpsid i \n" );
	//Initially disable the timer
	SYSTICK->CS = 0;
	//Enable interrupts
	__asm ("  cpsie i \n" );
}

/*
 * The Timer Interrupt Service Routine
 * Calls the callback function
 */
void __attribute__ ((interrupt)) systick_handler(void)
{

	if( rx_callback_fn )
	{
		rx_callback_fn();
	}
}


