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
static rt_thread_t receive_thread=RT_NULL;
static rt_thread_t send_thread=RT_NULL;

static rt_sem_t test_sem=RT_NULL;

u8 ucValue[2] = {0x00,0x00};
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("二值信号量实验\n");
    rt_kprintf("\n\n");
    //创建一个信号量
    test_sem = rt_sem_create("test_sem",
                            1,
                            RT_IPC_FLAG_FIFO);
                            
    if(test_sem!=RT_NULL)
    {
        rt_kprintf("信号量创建成功!\n");
    }
    
    //创建接收线程
    receive_thread = 
	rt_thread_create( "receive",
                    receive_thread_entry,
                    RT_NULL,
                    512,
                    4,
                    20);
    if(receive_thread!=RT_NULL)
    {
        rt_thread_startup(receive_thread);
    }
    else
    {
        return -1;
    }
    //创建发送线程
    send_thread = 
	rt_thread_create( "send",
                    send_thread_entry,
                    RT_NULL,
                    512,
                    5,
                    20);
    if(send_thread!=RT_NULL)
    {
        rt_thread_startup(send_thread);
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
void receive_thread_entry(void* parameter)
{
    
    while (1)
    {
        rt_sem_take(test_sem,
                    RT_WAITING_FOREVER);
                    
        if(ucValue[0]==ucValue[1])
        {
            rt_kprintf("Successfull\n");
        }
        else
        {
            rt_kprintf("Fail\n");
        }
        rt_sem_release(test_sem);
        
        rt_thread_delay(1000);   	
    }
    
}


void send_thread_entry(void* parameter)
{
    
    while (1)
    {
        rt_sem_take(test_sem,
                    RT_WAITING_FOREVER);
                    
        ucValue[0]++;
        rt_thread_delay(100);  
        ucValue[1]++;
        rt_sem_release(test_sem);
        rt_thread_yield();
    }

}
