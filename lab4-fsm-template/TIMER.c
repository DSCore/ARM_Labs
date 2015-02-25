/*
 * TIMER.c
 * Added by Devon Cormack 02/04/2015
 *
 */

#include "TIMER.h"
#include "stm32f4xx.h"

static void(*rx_callback_fn)(void);

void TIMER_init(void(*TIMER_rx_callback)(void)){

	//Set up the callback function for the timer
	rx_callback_fn = TIMER_rx_callback;

	//Enable the clock for the timer TIM2
	RCC->APB1ENR |= (0x1);//RCC_APB1ENR_TIM2EN;
	//Configure to count up.
	TIM2->CCMR1 &= (0xFFFFFF8F);
	TIM2->CCMR1 |= (0x10);//TIM2_CCMR1_OC1M;

	//Configure interrupt request enable for overflow interrupt
	TIM2->CR1 |= (0x4);

	//Enable the interrupt for updates
	TIM2->DIER |= (0x1); //Interrupt UIE enabled

	//Set the PSC so that it counts every 0.01 ms
	TIM2->PSC = 159999;
	//Set the ARR so that the period is 20 ms
	TIM2->ARR = 100;

//	//2.  Enable the TIM4 peripheral clock
//	//Enable the clock for the timer TIM2
//	RCC->APB1ENR |= (0x1);//RCC_APB1ENR_TIM2EN;
//	//3.  Configure the capture/compare register1 (CCMR1) OCM1 field as “Output Compare Mode”
//	/**
//	 * From Ref. Man. 15.4.7:
//	 * 110: PWM mode 1 - In upcounting, channel 1 is active as
//	 * long as TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive
//	 * (OC1REF=‘0) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
//	 */
//	//Configure to count up.
//	TIM2->CCMR1 &= (0xFFFFFF8F);
//	TIM2->CCMR1 |= (0x10);//TIM2_CCMR1_OC1M;
//
//	//Configure interrupt request enable for overflow interrupt
//	TIM2->CR1 |= (0x4);
//
//	//Enable the interrupt for updates
//	TIM2->DIER |= (0x1); //Interrupt UIE enabled
//
//	//4.  Configure the capture/compare enable register (CCER) CC1E field as “enabled”
////	TIM2->CCER |= TIM2_CCER_CC1E; //TODO: May not need this!
//
//	//5.  Set PSC and ARR to achieve the desired fundamental frequency.
//	//Set the PSC so that it counts every 0.01 ms
//	TIM2->PSC = 159999;//15999; //Set to 15999 so since SYSCLOCK/(PSC+1)=CK_CNT, the rate at which ARR counts, and SYSCLOCK = 16 MHz
//	//Set the ARR so that the period is 20 ms
//	TIM2->ARR = 100;//10000; //Set to 10000 so the period is 10 seconds
//
//	//Disable the timer
////	TIM2->CR1 &= 0b11111111111111111111111111111110;
//
//	//6.  Set CCR1 to achieve the desired duty cycle.
////	TIM2->CCR1 = 150;//5000; //Set CCR1 to initial value of 5 seconds
//
//	//7.  Enable the timer by writing ‘1’ to the CEN field in Control Register 1(CR1)
////	TIM2->CR1 |= TIM2_CR1_CEN;

}

void setTIMERTo5Sec(void){
	//Disable the flag.
	setTimer(0);

	//Clear the CNT
	TIM2->CNT = 0;

	//Read the status register (to clear the interrupt flag)
	TIM2->SR &= 0xFFFFFFFE;

	TIM2->ARR = 500;
	TIM2->CR1 |= (0x1);//TIM2_CR1_CEN;

//	//Disable the timer
////	TIM2->CR1 &= 0b11111111111111111111111111111110;
//	//Set the value to 5 seconds
////	TIM2->PSC = 15999; //Set to 15999 so since SYSCLOCK/(PSC+1)=CK_CNT, the rate at which ARR counts, and SYSCLOCK = 16 MHz
//	TIM2->ARR = 500; //Set to 10000 so the period is 10 seconds
////	TIM2->CCR1 = 5000; //Set CCR1 to initial value of 5 seconds
//	//Re-enable the timer
//	TIM2->CR1 |= (0x1);//TIM2_CR1_CEN;
}

void setTIMERTo2Sec(void){
	//Disable the flag.
	setTimer(0);

	//Read the status register
	TIM2->SR &= 0xFFFFFFFE;

	//Disable the timer
	TIM2->CR1 &= 0b11111111111111111111111111111110;
	//Set the value to 5 seconds
	TIM2->PSC = 15999; //Set to 15999 so since SYSCLOCK/(PSC+1)=CK_CNT, the rate at which ARR counts, and SYSCLOCK = 16 MHz
	TIM2->ARR = 2000; //Set to 10000 so the period is 10 seconds
//	TIM2->CCR1 = 2000; //Set CCR1 to initial value of 5 seconds
	//Re-enable the timer
	TIM2->CR1 |= TIM2_CR1_CEN;
}

void setTIMERTo1Sec(void){
	//Disable the flag.
	setTimer(0);
	//Disable the timer
	TIM2->CR1 &= 0b11111111111111111111111111111110;
	//Set the value to 5 seconds
	TIM2->PSC = 15999; //Set to 15999 so since SYSCLOCK/(PSC+1)=CK_CNT, the rate at which ARR counts, and SYSCLOCK = 16 MHz
	TIM2->ARR = 1000; //Set to 10000 so the period is 10 seconds
//	TIM2->CCR1 = 1000; //Set CCR1 to initial value of 5 seconds
	//Re-enable the timer
	TIM2->CR1 |= TIM2_CR1_CEN;
}

/*
 * The systick Interrupt Service Routine
 * Calls the callback function
 */
void __attribute__ ((interrupt)) TIM2_handler(void)
{
	USART2_putstr("INTERRUPT CALLED!!!");
	//Disable the timer
	TIM2->CR1 &= 0b11111111111111111111111111111110;
	TIM2->SR &= 0xFFFFFFFE;
	if( rx_callback_fn )
	{
		rx_callback_fn();
	}
}

