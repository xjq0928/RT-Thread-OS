#include "includes.h"

u32 send_data1 = 1;
u32 send_data2 = 2;


void EXTI_Key_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* 使能 SYSCFG 时钟 ，使用GPIO外部中断时必须使能SYSCFG时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* 连接 EXTI 中断源 到key1引脚 */
    SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);

    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    /* 连接 EXTI 中断源 到key2 引脚 */
    SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);

    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}


void KEY1_IRQHandler(void)
{
    /* 进入中断 */
    rt_interrupt_enter();

	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
        LED1_TOGGLE;
        /* 将数据写入（发送）到队列中，等待时间为 0  */
        rt_mq_send(	test_mq,	/* 写入（发送）队列的ID(句柄) */
                    &send_data1,			/* 写入（发送）的数据 */
                    sizeof(send_data1));	/* 数据的长度 */
		//清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
   
    /* 离开中断 */
    rt_interrupt_leave();
}


void KEY2_IRQHandler(void)
{
    /* 进入中断 */
    rt_interrupt_enter();

    //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
        LED2_TOGGLE;
        /* 将数据写入（发送）到队列中，等待时间为 0  */
        rt_mq_send(	test_mq,	/* 写入（发送）队列的ID(句柄) */
                    &send_data2,			/* 写入（发送）的数据 */
                    sizeof(send_data2));			/* 数据的长度 */
		//清除中断标志位
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
  
    /* 离开中断 */
    rt_interrupt_leave();
}




