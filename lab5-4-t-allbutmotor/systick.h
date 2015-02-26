/*
 * systick.h - routines for initializing and controlling systick timer
 */
#pragma once

#include "stdint.h"       /* define uint16_t, etc... */

///*
// * void systick_init(void)
// *
// * Set the values of CS, CR, and RV to initialize and enable SYSTICK.
// *
// */
//void systick_init(void);

/**
 * Initialize the systick_init with a callback function
 */
void systick_init(void(*systick_rx_callback)(void));

/*
 * The systick Interrupt Service Routine
 */
void systick_handler(void);


