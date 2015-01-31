/*
 * Potentiometer.h
 * Added by Devon Cormack 01/29/2015
 *
 */
#include "stm32f4xx.h"
#include "stdint.h"


//Initializes the potentiometer for use
void Potentiometer_init(void);

//Reads in a value from the Analog to Digital Converter and returns it
uint32_t ADC_read(void);
