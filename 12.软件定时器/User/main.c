/**
  *********************************************************************
  * @file    main.c
  * @author  xuejinqiang
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   RT-Thread 3.0 + STM32F407
  **********************************************************************
  */ 

/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
#include "includes.h"

/*
*************************************************************************
*                               ����
*************************************************************************
*/
static rt_timer_t swtmr1=RT_NULL;
static rt_timer_t swtmr2=RT_NULL;

static u32 TmrCb_Count1=0;
static u32 TmrCb_Count2=0;

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void swtmr1_callback(void* parameter);
static void swtmr2_callback(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("�����ʱ��ʵ��\n");
    rt_kprintf("\n\n");
    
    swtmr1 = rt_timer_create("swtmr1_callback",
                             swtmr1_callback,
                             0,
                             5000,
                             RT_TIMER_CTRL_SET_ONESHOT|RT_TIMER_FLAG_SOFT_TIMER);
                            
    if(swtmr1!=RT_NULL)
    {
        rt_timer_start(swtmr1);
    }
    
    swtmr2 = rt_timer_create("swtmr2_callback",
                             swtmr2_callback,
                             0,
                             1000,
                             RT_TIMER_CTRL_SET_PERIODIC|RT_TIMER_FLAG_SOFT_TIMER);
                            
    if(swtmr2!=RT_NULL)
    {
        rt_timer_start(swtmr2);
    }
    
}	


/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/
void swtmr1_callback(void* parameter)
{
    u32 tick_num1;
        
    TmrCb_Count1++;

    tick_num1 = (u32)rt_tick_get();
    
    rt_kprintf("swtmr1_callback����ִ�� %d ��\n",TmrCb_Count1);
    
    rt_kprintf("�δ�ʱ����ֵ = %d \n",tick_num1);
    
}


void swtmr2_callback(void* parameter)
{
    u32 tick_num2;
        
    TmrCb_Count2++;

    tick_num2 = (u32)rt_tick_get();
    
    rt_kprintf("swtmr2_callback����ִ�� %d ��\n",TmrCb_Count2);
    
    rt_kprintf("�δ�ʱ����ֵ = %d \n",tick_num2);
}
