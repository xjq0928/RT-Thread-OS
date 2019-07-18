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

static rt_event_t test_event=RT_NULL;

#define KEY1_EVENT   (0x01<<0)
#define KEY2_EVENT   (0x01<<1)


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
    rt_kprintf("事件实验\n");
    rt_kprintf("\n\n");
    //创建一个事件
    test_event = rt_event_create("test_event",
                                  RT_IPC_FLAG_FIFO);
                            
    if(test_event!=RT_NULL)
    {
        rt_kprintf("事件创建成功!\n");
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
    rt_uint32_t recved;
    
    while (1)
    {
        rt_event_recv(test_event,
                      KEY1_EVENT|KEY2_EVENT,
                      RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR,
                      RT_WAITING_FOREVER,
                      &recved);
                      
        if(recved==(KEY1_EVENT|KEY2_EVENT)) 
        {
            rt_kprintf("KEY1与KEY2都按下，获取事件成功\n");
            LED1_TOGGLE;
        }
        else        
        {
            rt_kprintf("事件错误！\n");
        }
        rt_thread_delay(20);   	
    }
    
}


void send_thread_entry(void* parameter)
{
    u8 key_sta;
    
    while (1)
    {
        key_sta = KEY_Scan(0);
        
        if(key_sta==1)
        {
            rt_kprintf("KEY1被按下！\n");
            rt_event_send(test_event,KEY1_EVENT);
        }
        else if(key_sta==2)
        {
            rt_kprintf("KEY2被按下！\n");
            rt_event_send(test_event,KEY2_EVENT);
        }
        
        rt_thread_delay(20);   	
    }

}
