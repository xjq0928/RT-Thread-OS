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
*                             包含的头文件
*************************************************************************
*/ 
#include "includes.h"

/*
*************************************************************************
*                               变量
*************************************************************************
*/
static rt_thread_t led_thread=RT_NULL;
static rt_thread_t key_thread=RT_NULL;
static rt_thread_t usart_thread=RT_NULL;

rt_mq_t test_mq=RT_NULL;
rt_sem_t test_sem=RT_NULL;


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led_thread_entry(void* parameter);
static void key_thread_entry(void* parameter);
static void usart_thread_entry(void* parameter);


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("中断管理实验\n");
    rt_kprintf("\n\n");
    
    //创建一个消息队列
    test_mq = rt_mq_create("test_mq",
                            4,
                            2,
                            RT_IPC_FLAG_FIFO);
                            
    if(test_mq!=RT_NULL)
    {
        rt_kprintf("消息队列创建成功!\n\n");
    }
    
    //创建一个信号量
    test_sem = rt_sem_create("test_sem",
                             0,
                             RT_IPC_FLAG_FIFO);
    if(test_sem!=RT_NULL)
    {
        rt_kprintf("信号量创建成功!\n\n");
    }
    
    //创建线程1
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
   
    //创建线程2
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
    //创建线程3
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
*                             线程定义
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
            rt_kprintf("收到数据：%s\n",USART1_RX_BUF);
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
            rt_kprintf("触发中断的是KEY%d!\n\n",r_queue);
        }
        else
        {
            rt_kprintf("数据接收出错！\n\n");
        }
        
        rt_thread_delay(10);
    }

}
