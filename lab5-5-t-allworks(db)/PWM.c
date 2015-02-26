/*
 * PWM.c
 * Added by Devon Cormack 02/04/2015
 *
 */

#include "PWM.h"
#include "stm32f4xx.h"


void enable_PB6_AF2(){
	//Enable Port B
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//Set PB6 to alternate function mode for the servo to communicate with the timer.
	//Read Modify Write GPIO_MODER6 to 10 for AF use
	GPIOB->MODER &= (0xFFFFCFFF);
	GPIOB->MODER |= (0x2000); //PB6 in alternate function mode //CHANGE TO READ-MODIFY-WRITE

	//TODO: Set the AF register!!!!
	GPIOB->AFRL &= 	(0xF0FFFFFF);
	GPIOB->AFRL |=	(0x02000000);

	//Enable the clock for the timer TIM4
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
}

void PWM_init(){
	//1.  Configure port D pin#12 for the appropriate alternate function output (TIM4_CH1)
	//2.  Enable the TIM4 peripheral clock
	enable_PB6_AF2();
	//3.  Configure the capture/compare register1 (CCMR1) OCM1 field as “PWMmode1.”
	/**
	 * From Ref. Man. 15.4.7:
	 * 110: PWM mode 1 - In upcounting, channel 1 is active as
	 * long as TIMx_CNT<TIMx_CCR1 else inactive. In downcounting, channel 1 is inactive
	 * (OC1REF=‘0) as long as TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
	 */
	TIM4->CCMR1 |= TIM4_CCMR1_OC1M_PWMmode1;

	//4.  Configure the capture/compare enable register (CCER) CC1E field as “enabled”
	TIM4->CCER |= TIM4_CCER_CC1E;

	//5.  Set PSC and ARR to achieve the desired fundamental frequency.
	//Set the PSC so that it counts every 0.01 ms
	TIM4->PSC = 159; //Set to 159 since SYSCLOCK/(PSC+1)=CK_CNT, the rate at which ARR counts, and SYSCLOCK = 16 MHz
	//Set the ARR so that the period is 20 ms
	TIM4->ARR = 2000; //Set to 20 since 1/20 kHz = 50 Hz

	//6.  Set CCR1 to achieve the desired duty cycle.
	TIM4->CCR1 = 150; // The desired duty cycle is between 5% and 10%. To be conservative, we will set it to right in the middle at 7.5%, or 150.

	//7.  Enable the timer by writing ‘1’ to the CEN field in Control Register 1(CR1)
	TIM4->CR1 |= TIM4_CR1_CEN;

}

void PWM_to_ZeroDeg(){
	//Set the servo to 0 degrees (100 ms period)
	TIM4->CCR1 = 100;
}

void PWM_to_NinetyDeg(){
	//Set the servo to 90 degrees (200 ms period)
	TIM4->CCR1 = 200;
}

/**
 * Set the servo to a location between 0 and 90 degrees
 * offset - value between 0 and 4096
 */
void PWM_to_Deg(uint32_t offset){
	//Set the servo to an offset between 0 degrees(100 ms period)
	// and 90 degrees(200 ms period)
	TIM4->CCR1 = 100+100*offset/4096;
}
