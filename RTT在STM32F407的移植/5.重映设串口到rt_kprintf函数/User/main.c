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
static rt_thread_t led0_thread=RT_NULL;
static rt_thread_t led1_thread=RT_NULL;

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void led0_thread_entry(void* parameter);
static void led1_thread_entry(void* parameter);


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("重映设串口到kprintf函数实验\n");
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
   
}	


/*
*************************************************************************
*                             线程定义
*************************************************************************
*/
void led0_thread_entry(void* parameter)
{
    while (1)
    {
        LED0_ON;
        rt_kprintf("LED0 is ON\n");
        rt_thread_delay(500);   //延时500个tick
        
        LED0_OFF;    
        rt_kprintf("LED0 is OFF\n");
        rt_thread_delay(500);   //延时500个tick	 		
    }
    
}

void led1_thread_entry(void* parameter) 
{
    while (1)
    {
        LED1_ON;
        rt_thread_delay(500);   //延时500个tick
        
        LED1_OFF;     
        rt_thread_delay(500);   //延时500个tick	 		
    }
    
}



