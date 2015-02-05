/*
 * USER.h
 * Added by Devon Cormack 01/30/2015
 *
 */
#include "stm32f4xx.h"
#include "stdint.h"

//Initialize the GPIOD port to read the USER button
void USER_init(void);

 /* USER_debounce:
  *
  * Waits for a debounced input.
  * Returns 1 upon receiving a
  * debounced input.  //MAKE THIS WAIT FOR 16000 cycles per check!
  *
  */
uint32_t USER_debounce(void); //This is wrong, but for some reason uint32_t isn't recognized here.

