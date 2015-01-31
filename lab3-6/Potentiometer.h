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

/**
 * ADC_sample_on_USER_with_History:
 * Samples the potentiometer when the USER input button
 * is pressed. Waits until the user input button is pressed
 * before sampling. Prints out history after sampling.
 */
void ADC_sample_on_USER_with_History(void);
