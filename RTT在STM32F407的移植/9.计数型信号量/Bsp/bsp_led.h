#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f4xx.h"

//LED�˿ں궨��
//LED0
#define LED0_GPIO_PORT    	GPIOF			              
#define LED0_GPIO_CLK 	    RCC_AHB1Periph_GPIOF		
#define LED0_GPIO_PIN		GPIO_Pin_8			        

//LED1
#define LED1_GPIO_PORT    	GPIOF			             
#define LED1_GPIO_CLK 	    RCC_AHB1Periph_GPIOF		
#define LED1_GPIO_PIN		GPIO_Pin_9			        

//LED2
#define LED2_GPIO_PORT    	GPIOF			              
#define LED2_GPIO_CLK 	    RCC_AHB1Periph_GPIOF		
#define LED2_GPIO_PIN		GPIO_Pin_10			        


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRRL=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BSRRH=i;}	 //����͵�ƽ
#define digitalToggle(p,i)   {p->ODR ^=i;}   //�����ת״̬


/* �������IO�ĺ� */
#define LED0_TOGGLE		   digitalToggle(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_OFF		   digitalHi(LED0_GPIO_PORT,LED0_GPIO_PIN)
#define LED0_ON			   digitalLo(LED0_GPIO_PORT,LED0_GPIO_PIN)

#define LED1_TOGGLE		   digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		   digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			   digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		   digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		   digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			   digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)


void LED_Init(void);


#endif

