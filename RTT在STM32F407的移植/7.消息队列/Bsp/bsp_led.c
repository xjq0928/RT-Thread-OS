#include "includes.h"



//------------------------板载运行指示灯------------------------------------
//运行指示灯IO初始化
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LED0_GPIO_CLK | LED1_GPIO_CLK | LED2_GPIO_CLK, ENABLE); 
	//LED0
	GPIO_InitStructure.GPIO_Pin = LED0_GPIO_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(LED0_GPIO_PORT, &GPIO_InitStructure);
    //LED1
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN; 
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);
    //LED2
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN; 
	GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);


    LED0_OFF;    
    LED1_OFF;    
    LED2_OFF;    
}



//------------------------------------------------------------------------





