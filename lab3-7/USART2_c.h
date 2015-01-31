/*
 * USART2_c.h
 * By Devon Cormack 01/28/15
 */

#pragma once

#include "stdint.h"

/*
 * The USART2 Interrupt Service Routine
 */
void __attribute__ ((interrupt)) USART2_handler(void);

