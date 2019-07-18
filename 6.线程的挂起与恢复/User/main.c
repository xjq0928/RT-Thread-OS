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
static rt_thread_t led0_thread=RT_NULL;
static rt_thread_t led1_thread=RT_NULL;
static rt_thread_t key_thread=RT_NULL;


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led0_thread_entry(void* parameter);
static void led1_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("�̵߳Ĺ�����ָ�ʵ��\n");
    rt_kprintf("\n\n");
    
    led0_thread = 
	rt_thread_create( "led0",
                    led0_thread_entry,
                    RT_NULL,
                    512,
                    4,
                    30);
    if(led0_thread!=RT_NULL)
    {
        rt_thread_startup(led0_thread);
    }
    else
    {
        return -1;
    }
    
    led1_thread = 
	rt_thread_create( "led1",
                    led1_thread_entry,
                    RT_NULL,
                    512,
                    5,
                    30);
    if(led1_thread!=RT_NULL)
    {
        rt_thread_startup(led1_thread);
    }
    else
    {
        return -1;
    }
    
    key_thread = 
	rt_thread_create( "key",
                    key_thread_entry,
                    RT_NULL,
                    512,
                    3,
                    30);
    if(key_thread!=RT_NULL)
    {
        rt_thread_startup(key_thread);
    }
    else
    {
        return -1;
    }
    
   
}	


/*
*************************************************************************
*                             �̶߳���
*************************************************************************
*/
void led0_thread_entry(void* parameter)
{
    while (1)
    {
        LED0_ON;
        rt_thread_delay(100);   //��ʱ100��tick
        
        LED0_OFF;    
        rt_thread_delay(100);   //��ʱ100��tick	 		
    }
    
}

void led1_thread_entry(void* parameter) 
{
    while (1)
    {
        LED1_ON;
        rt_thread_delay(100);   //��ʱ100��tick
        
        LED1_OFF;     
        rt_thread_delay(100);   //��ʱ100��tick	 		
    }
    
}

void key_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    u8 key_sta;
    
    while (1)
    {
        key_sta = KEY_Scan(0);
        
        switch(key_sta)
        {
            case 1://K1
                rt_kprintf("����LED0�߳�!\n");
                uwRet = rt_thread_suspend(led0_thread);
                if(uwRet==RT_EOK)
                {
                    rt_kprintf("����LED0�̳߳ɹ�!\n"); 
                }
                else
                {
                    rt_kprintf("����LED0�߳�ʧ��!ʧ�ܴ��룺0x%lx\n",uwRet);
                }
                break;
                
            case 2://K2
                rt_kprintf("����LED1�߳�!\n");
                uwRet = rt_thread_suspend(led1_thread);
                if(uwRet==RT_EOK)
                {
                    rt_kprintf("����LED1�̳߳ɹ�!\n"); 
                }
                else
                {
                    rt_kprintf("����LED1�߳�ʧ��!ʧ�ܴ��룺0x%lx\n",uwRet);
                }
            
                break;

            case 3://K3
                rt_kprintf("�ָ�LED0�߳�!\n");
                uwRet = rt_thread_resume(led0_thread);
                if(uwRet==RT_EOK)
                {
                    rt_kprintf("�ָ�LED0�̳߳ɹ�!\n"); 
                }
                else
                {
                    rt_kprintf("�ָ�LED0�߳�ʧ��!ʧ�ܴ��룺0x%lx\n",uwRet);
                }
            
                break;

            case 4://K_UP
                rt_kprintf("�ָ�LED1�߳�!\n");
                uwRet = rt_thread_resume(led1_thread);
                if(uwRet==RT_EOK)
                {
                    rt_kprintf("�ָ�LED1�̳߳ɹ�!\n"); 
                }
                else
                {
                    rt_kprintf("�ָ�LED1�߳�ʧ��!ʧ�ܴ��룺0x%lx\n",uwRet);
                }
            
                break;
              
        }
        rt_thread_delay(10);   		
    }

}
