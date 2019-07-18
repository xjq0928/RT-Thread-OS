#ifndef __BSP_KEY_H
#define __BSP_KEY_H
	 
#include "stm32f4xx.h" 
	 
#define KEY1_PIN                  GPIO_Pin_2                 
#define KEY1_GPIO_PORT            GPIOE                      
#define KEY1_GPIO_CLK             RCC_AHB1Periph_GPIOE

#define KEY2_PIN                  GPIO_Pin_3                 
#define KEY2_GPIO_PORT            GPIOE                      
#define KEY2_GPIO_CLK             RCC_AHB1Periph_GPIOE

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 



#define KEY0_PRES 	1
#define KEY1_PRES	2

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数	

#endif
