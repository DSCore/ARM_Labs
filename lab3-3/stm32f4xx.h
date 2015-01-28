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

/* See datasheet for memory map / base addresses */

/*
 * GPIOA & GPIOD
 */
#define GPIOA_BASE   (0x40020000)  /* Base address of GPIOA peripheral */
#define GPIOA        ((GPIO_TypeDef*)GPIOA_BASE)
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
#define RCC_AHB1ENR_GPIODEN     (0x08)

/**
 * SYSTICK Timer:
 */
#define SYSTICK_BASE	(0xE000E010)
#define SYSTICK			((SYSTICK_TypeDef*)SYSTICK_BASE)
