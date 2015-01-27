#pragma once

#include "stdint.h"

/*
 * void USART2_init(void)
 *
 * Configure the USART2 peripheral at 9600 baud, 8 data bits, no parity, 1 stop
 * to use PD5 & PD6.  Configures the NVIC to pass interrupts from the USART2
 * peripheral to the processor, but does not set the interrupt configuration flags
 * in the USART2 interrupt control registers.
 */
void USART2_init(void);

/*
 * void USART2_send(uint8_t data)
 *
 * Send the value passed in variable 'data'.  Polling based driver.
 */
void USART2_send(uint8_t data);

/*
 * uint8_t USART2_recv(void)
 *
 * Read a byte from the USART2 peripheral.  Polling driver/blocking call.
 */
uint8_t USART2_recv(void);
