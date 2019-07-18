#include "includes.h"

u32 send_data1 = 1;
u32 send_data2 = 2;


void EXTI_Key_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* ʹ�� SYSCFG ʱ�� ��ʹ��GPIO�ⲿ�ж�ʱ����ʹ��SYSCFGʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    /* ���� EXTI �ж�Դ ��key1���� */
    SYSCFG_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE,KEY1_INT_EXTI_PINSOURCE);

    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    /* ���� EXTI �ж�Դ ��key2 ���� */
    SYSCFG_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE,KEY2_INT_EXTI_PINSOURCE);

    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}


void KEY1_IRQHandler(void)
{
    /* �����ж� */
    rt_interrupt_enter();

	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
        LED1_TOGGLE;
        /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
        rt_mq_send(	test_mq,	/* д�루���ͣ����е�ID(���) */
                    &send_data1,			/* д�루���ͣ������� */
                    sizeof(send_data1));	/* ���ݵĳ��� */
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
	}  
   
    /* �뿪�ж� */
    rt_interrupt_leave();
}


void KEY2_IRQHandler(void)
{
    /* �����ж� */
    rt_interrupt_enter();

    //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
        LED2_TOGGLE;
        /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0  */
        rt_mq_send(	test_mq,	/* д�루���ͣ����е�ID(���) */
                    &send_data2,			/* д�루���ͣ������� */
                    sizeof(send_data2));			/* ���ݵĳ��� */
		//����жϱ�־λ
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
  
    /* �뿪�ж� */
    rt_interrupt_leave();
}




