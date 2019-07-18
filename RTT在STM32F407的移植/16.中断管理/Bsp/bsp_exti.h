#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f4xx.h"

//���Ŷ���
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOE
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOE
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_2
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOE
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource2
#define KEY1_INT_EXTI_LINE                EXTI_Line2
#define KEY1_INT_EXTI_IRQ                 EXTI2_IRQn

#define KEY1_IRQHandler                   EXTI2_IRQHandler

#define KEY2_INT_GPIO_PORT                GPIOE
#define KEY2_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOE
#define KEY2_INT_GPIO_PIN                 GPIO_Pin_3
#define KEY2_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOE
#define KEY2_INT_EXTI_PINSOURCE           EXTI_PinSource3
#define KEY2_INT_EXTI_LINE                EXTI_Line3
#define KEY2_INT_EXTI_IRQ                 EXTI3_IRQn

#define KEY2_IRQHandler                   EXTI3_IRQHandler

/*******************************************************/



void EXTI_Key_Config(void);

#endif

