/*
 * TIMER.h
 *
 *  Created on: Feb 4, 2015
 *      Author: Devon Cormack
 */

void TIMER_init(void(*TIMER_rx_callback)(void));

void setTIMERTo5Sec(void);

void setTIMERTo2Sec(void);

void setTIMERTo1Sec(void);

void __attribute__ ((interrupt)) TIM2_handler(void);

