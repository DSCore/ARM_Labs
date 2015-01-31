/*
 * Potentiometer.c
 * Added by Devon Cormack 01/29/2015
 *
 */

#include "Potentiometer.h"
#include "stm32f4xx.h"

//Enables GPIO Port A1 for analog use.
void enablePA1Analog(){
	//Enable port A
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	//Set PA1 to analog so the POT can communicate.
	GPIOA->MODER |= (0xC); //PA1 analog is 11 in bits 2 and 3
}

//Initializes the potentiometer for use
void ADC_init(void){

	//Prepare GPIOA1 for analog input.
	enablePA1Analog();

	//Turn on the ADC clock on
	RCC->APB2ENR |= ADC_ENABLE;

	//Set the sequence length of the ADC read to 1 port
	ADC->SQR1 &= ADC_SET_SEQUENCE_LENGTH_1; //this could  be 0

	//Set the first regular sequence conversion to be from port A1
	ADC->SQR3 = ADC_SET_1ST_REGULAR_SEQ_CONVERSION; //does stepping on the rest matter here?

	//Set the sample time for the ADC to 15 for channel 1
	ADC->SMPR2 |= (0x8);

	//Now that everything is set up, enable conversions.
	ADC->CR2 |= ADC_CR2_ADON;
	uint32_t test = ADC->CR2;
}

//Reads in a value from the Analog to Digital Converter and returns it
uint32_t ADC_read(void){
	//Initiate a read by writing 1 to SWSTART (bit 30)
	ADC->CR2 |= ADC_CR2_SWSTART;
	uint32_t read = 0;
	while(read == 0){
		//The status hasn't been converted yet, check again
		read = ADC->SR;
		uint32_t mask = ADC_SR_EOC;
		read = read & mask;
	}

	//Clear the status read bit by reading from Data Register.
	uint32_t data = ADC->DR & ADC_DR_MASK; //This should really be masked down to 12 bits.
	return data;
}
