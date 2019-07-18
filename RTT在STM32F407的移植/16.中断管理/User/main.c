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
static rt_thread_t led_thread=RT_NULL;
static rt_thread_t key_thread=RT_NULL;
static rt_thread_t usart_thread=RT_NULL;

rt_mq_t test_mq=RT_NULL;
rt_sem_t test_sem=RT_NULL;


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void led_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);
static void usart_thread_entry(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("�жϹ���ʵ��\n");
    rt_kprintf("\n\n");
    
    //����һ����Ϣ����
    test_mq = rt_mq_create("test_mq",
                            4,
                            2,
                            RT_IPC_FLAG_FIFO);
                            
    if(test_mq!=RT_NULL)
    {
        rt_kprintf("��Ϣ���д����ɹ�!\n\n");
    }
    
    //����һ���ź���
    test_sem = rt_sem_create("test_sem",
                             0,
                             RT_IPC_FLAG_FIFO);
    if(test_sem!=RT_NULL)
    {
        rt_kprintf("�ź��������ɹ�!\n\n");
    }
    
    //�����߳�1
    led_thread = 
    rt_thread_create("led",
                     led_thread_entry,
                     RT_NULL,
                     512,
                     5,
                     20);
     if(led_thread!=RT_NULL)
    {
        rt_thread_startup(led_thread);
    }
    else
    {
        return -1;
    }
   
    //�����߳�2
    key_thread = 
	rt_thread_create( "key",
                    key_thread_entry,
                    RT_NULL,
                    512,
                    1,
                    20);
    if(key_thread!=RT_NULL)
    {
        rt_thread_startup(key_thread);
    }
    else
    {
        return -1;
    }
    //�����߳�3
    usart_thread = 
	rt_thread_create( "usart",
                    usart_thread_entry,
                    RT_NULL,
                    512,
                    2,
                    20);
    if(usart_thread!=RT_NULL)
    {
        rt_thread_startup(usart_thread);
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
void led_thread_entry(void* parameter)
{
    while(1)
    {
        LED0_TOGGLE;
        rt_thread_delay(500);
    }
}

void usart_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    
    while (1)
    {
        uwRet = rt_sem_take(test_sem,
                            0);
        
        if(uwRet==RT_EOK)
        {
            rt_kprintf("�յ����ݣ�%s\n",USART1_RX_BUF);
            memset(USART1_RX_BUF,0,USART1_REC_LEN);
        }
        
        rt_thread_delay(10);
    }
    
}


void key_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    u32 r_queue;
    
    while (1)
    {
        uwRet = rt_mq_recv(test_mq,
                           &r_queue,
                           sizeof(r_queue),
                           RT_WAITING_FOREVER);
                           
        if(uwRet==RT_EOK)                   
        {
            rt_kprintf("�����жϵ���KEY%d!\n\n",r_queue);
        }
        else
        {
            rt_kprintf("���ݽ��ճ���\n\n");
        }
        
        rt_thread_delay(10);
    }

}
