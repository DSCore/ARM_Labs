#pragma once

#define NVIC_ISER1_ADDR (0xE000E100)
#define NVIC_ISER2_ADDR (0xE000E104)
#define NVIC_ISER3_ADDR (0xE000E108)

#define NVIC_ISER1 (*((uint32_t *)(NVIC_ISER1_ADDR)))
#define NVIC_ISER2 (*((uint32_t *)(NVIC_ISER2_ADDR)))
#define NVIC_ISER3 (*((uint32_t *)(NVIC_ISER3_ADDR)))

/* EXTI0 is position 6, priorithy 13 */
#define NVIC_INTERRUPT_EXTI_0_ENABLE()        (NVIC_ISER1 = 0x00000040)
#define NVIC_INTERRUPT_EXTI_0_DISABLE()       (NVIC_ICER1 = 0x00000040)

/* USART2 is position 38, priority 45 */
#define NVIC_INTERRUPT_USART_2_ENABLE()       (NVIC_ISER2 = 0x00000040)
#define NVIC_INTERRUPT_USART_2_DISABLE()      (NVIC_ICER2 = 0x00000040)




