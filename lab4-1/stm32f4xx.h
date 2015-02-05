#pragma once

#include "stdint.h"

/*
 * Some useful definitions for controlling the clock enable registers,
 * configuring GPIO and turning LEDs on and off.
 */

/* GPIO Registers, see the reference manual sections 7.4.X */
volatile typedef struct
{
	uint32_t MODER;                /* Mode register                    - offset 0x00 */
	uint32_t OTYPER;               /* Output type register             - offset 0x04 */
	uint32_t OSPEEDR;              /* Output speed register            - offset 0x08 */
	uint32_t PUPDR;                /* Port pull-up/pull-down register  - offset 0x0C */
	uint32_t IDR;                  /* Input data register              - offset 0x10 */
	uint32_t ODR;                  /* Output data register             - offset 0x14 */
	uint32_t BSRR;                 /* Bit set-reset register           - offset 0x18 */
	uint32_t LCKR;                 /* Port configuration lock register - offset 0x1C */
	uint32_t AFRL;                 /* Alternate function low register  - offset 0x20 */
	uint32_t AFRH;                 /* Alternate function high register - offset 0x24 */
} GPIO_TypeDef;

/* Reset and Clock Control Registers, see User's Manual section 6.3.X */
volatile typedef struct
{
	uint32_t CR;                   /* Clock control register           - offset 0x00 */
	uint32_t PLLCFGR;              /* PLL configuration register       - offset 0x04 */
	uint32_t CFGR;                 /* RCC clock configuration register - offset 0x08 */
	uint32_t CIR;                  /* Clock configuration register     - offset 0x0C */
	uint32_t AHB1RSTR;             /* AHB1 peripheral reset register   - offset 0x10 */
	uint32_t AHB2RSTR;             /* AHB2 peripheral reset register   - offset 0x14 */
	uint32_t AHB3RSTR;             /* AHB3 peripheral reset register   - offset 0x18 */
	uint32_t RES0;                 /* Reserved                         - offset 0x1C */
	uint32_t APB1RSTR;             /* APB1 peripheral reset register   - offset 0x20 */
	uint32_t APB2RSTR;             /* APB2 peripheral reset register   - offset 0x24 */
	uint32_t RES1;                 /* Reserved                         - offset 0x28 */
	uint32_t RES2;                 /* Reserved                         - offset 0x2C */
	uint32_t AHB1ENR;              /* AHB1 peripheral clock enable     - offset 0x30 */
	uint32_t RES3;                 /* Reserved                         - offset 0x34 */
	uint32_t RES4;                 /* Reserved                         - offset 0x38 */
	uint32_t RES5;                 /* Reserved                         - offset 0x3C */
	uint32_t APB1ENR;              /* APB1 peripheral clock enable     - offset 0x40 */
	uint32_t APB2ENR;              /* APB2 peripheral clock enable     - offset 0x44 */
	/* Rest of registers not utilized in lab 3 */
} RCC_TypeDef;

/* Control Registers for the SYSTICK timer - Added by Devon Cormack */
volatile typedef struct
{
	uint32_t CS;		/* Control and Status Register 	0x10*/
	uint32_t RV;		/* Reload Value Register 		0x14*/
	uint32_t CV;		/* Current Value Register 		0x18*/
} SYSTICK_TypeDef;

/* ADC Registers for the potentiometer - Added by Devon Cormack */
volatile typedef struct
{
	uint32_t SR; 		/* Status Register 							0x00 */
	uint32_t CR1;		/* Control Register 1						0x04 */
	uint32_t CR2;		/* Control Register 2						0x08 */
	uint32_t SMPR1;		/* Sample Time Register 					0x0C */
	uint32_t SMPR2;		/* Sample Time Register			 			0x10 */
	uint32_t JOFR1;		/* Injected Channel Data offset Register 	0x14 */
	uint32_t JOFR2;		/* Injected Channel Data Offset Register 	0x18 */
	uint32_t JOFR3;		/* Injected Channel Data Offset Register 	0x1C */
	uint32_t JOFR4;		/* Injected Channel Data Offset Register 	0x20 */
	uint32_t HTR;		/* Watchdog Higher Threshold Register		0x24 */
	uint32_t LTR;		/* Watchdog Lower Threshold Register		0x28 */
	uint32_t SQR1;		/* Regular Sequence Register 1				0x2C */
	uint32_t SQR2;		/* Regular Sequence Register 2				0x30 */
	uint32_t SQR3;		/* Regular Sequence Register 3				0x34 */
	uint32_t JSQR;		/* Injected Sequence Register				0x38 */
	uint32_t JDR1;		/* Injected Data Register 1					0x3C */
	uint32_t JDR2;		/* Injected Data Register 2					0x40 */
	uint32_t JDR3;		/* Injected Data Register 3					0x44 */
	uint32_t JDR4;		/* Injected Data Register 4					0x48 */
	uint32_t DR;		/* Regular Data Register 					0x4C */
	uint32_t CSR;		/* Common Status Register					0x300 */
	uint32_t CCR;		/* Common Control Register					0x304 */
	uint32_t CDR;		/* Common Data Register						0x308 */
} ADC_Registers_TypeDef;

volatile typedef struct
{
	uint32_t CR1;		/* Control Register 1						0x00 */
	uint32_t CR2;		/* Control Register 2						0x04 */
	uint32_t SMCR;		/* Slave Mode Control Register				0x08 */
	uint32_t DIER;		/* DMA/Interrupt Control Register			0x0C */
	uint32_t SR;		/* Status Register 							0x10 */
	uint32_t EGR;		/* Event Generation Register				0x14 */
	uint32_t CCMR1;		/* Capture/Compare Mode Register 1			0x18 */
	uint32_t CCMR2;		/* Capture/Compare Mode Register 2			0x1C */
	uint32_t CCER;		/* Capture/Compare Enable Register			0x20 */
	uint32_t CNT;		/* Counter									0x24 */
	uint32_t PSC;		/* Prescaler								0x28 */
	uint32_t ARR;		/* Auto-Reload Register						0x2C */
	uint32_t placeholder_1;										/*	0x30 */
	uint32_t CCR1;		/* Capture/Compare Register 1				0x34 */
	uint32_t CCR2;		/* Capture/Compare Register 2				0x38 */
	uint32_t CCR3;		/* Capture/Compare Register 3				0x3C */
	uint32_t CCR4;		/* Capture/Compare Register 4				0x40 */
	uint32_t placeholder_2;										/*	0x44 */
	uint32_t DCR;		/* DMA Control Register						0x48 */
	uint32_t DMAR;		/* DMA Address For Full Transfer			0x4C */
	uint32_t OR;		/* TIM2 or TIM5 Option Register				0x50 */
} TIM2_5_TypeDef;

/* See datasheet for memory map / base addresses */

/*
 * GPIOA, GPIOB & GPIOD
 */
#define GPIOA_BASE   (0x40020000)  /* Base address of GPIOA peripheral */
#define GPIOA        ((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOB_BASE	 (0x40020400) /* Base address of GPIOB peripheral */
#define GPIOB		 ((GPIO_TypeDef*)GPIOB_BASE)
#define GPIOD_BASE   (0x40020C00)  /* Base address of GPIOD peripheral */
#define GPIOD        ((GPIO_TypeDef*)GPIOD_BASE)

#define GPIOx_MODER_PIN12_MASK  (0xfcffffff)
#define GPIOx_MODER_PIN13_MASK  (0xf3ffffff)
#define GPIOx_MODER_PIN14_MASK  (0xcfffffff)
#define GPIOx_MODER_PIN15_MASK  (0x3fffffff)
#define GPIOx_MODER_PIN12_GPOUT (0x01000000)
#define GPIOx_MODER_PIN13_GPOUT (0x04000000)
#define GPIOx_MODER_PIN14_GPOUT (0x10000000)
#define GPIOx_MODER_PIN15_GPOUT (0x40000000)

#define RCC_BASE     (0x40023800)
#define RCC          ((RCC_TypeDef*)RCC_BASE)

#define RCC_AHB1ENR_GPIOAEN     (0x01)
#define RCC_AHB1ENR_GPIOBEN		(0x02)
#define RCC_AHB1ENR_GPIODEN     (0x08)

/**
 * ADC
 */

#define ADC_BASE 	(0X40012000)
#define ADC		((ADC_Registers_TypeDef*)ADC_BASE)

	//Masks for the ADC
#define ADC_ENABLE 			(0x100) // Turn on bit 8 for ADC1
#define ADC_SET_SEQUENCE_LENGTH_1	(0b11111111000011111111111111111111) //0000 means 1 conversion (Ref. Man. 11.13.19)
#define ADC_SET_1ST_REGULAR_SEQ_CONVERSION 	(0x1) //Sets the first conversion in regular sequence to input 1
#define ADC_CR2_ADON (0x1)
#define ADC_SR_EOC	(0x2)
#define ADC_DR_DATA (0xFFFF)
//#define ADC_CR2_SWSTART (0x40000000)
#define ADC_CR2_SWSTART (0b01000000000000000000000000000000)
#define ADC_DR_MASK (0xFFF)


/**
 * PWM
 */
//Register addresses
#define TIM4_BASE  	(0x40000800)
#define TIM4		((TIM2_5_TypeDef*)TIM4_BASE)
//Masks
#define RCC_APB1ENR_TIM4EN	(0x4)
#define TIM4_CCMR1_OC1M_PWMmode1 (0b1100000) // (0x60) is equivalent
#define TIM4_CCER_CC1E	(0x1)
#define TIM4_CR1_CEN	(0x1)


/**
 * SYSTICK Timer:
 */
#define SYSTICK_BASE	(0xE000E010)
#define SYSTICK			((SYSTICK_TypeDef*)SYSTICK_BASE)
