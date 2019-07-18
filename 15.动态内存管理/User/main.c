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
static rt_thread_t alloc_thread=RT_NULL;
static rt_thread_t free_thread=RT_NULL;

static rt_uint32_t *p_test = RT_NULL;


#define TEST_SIZE 100

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void alloc_thread_entry(void* parameter);
static void free_thread_entry(void* parameter);


/*
*************************************************************************
*                             main 函数
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("动态内存管理实验\n");
    rt_kprintf("\n\n");
    
    
    alloc_thread = 
	rt_thread_create( "alloc",
                    alloc_thread_entry,
                    RT_NULL,
                    512,
                    1,
                    20);
    if(alloc_thread!=RT_NULL)
    {
        rt_thread_startup(alloc_thread);
    }
    else
    {
        return -1;
    }
    
    free_thread = 
	rt_thread_create( "free",
                    free_thread_entry,
                    RT_NULL,
                    512,
                    2,
                    20);
    if(free_thread!=RT_NULL)
    {
        rt_thread_startup(free_thread);
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
void alloc_thread_entry(void* parameter)
{
    rt_kprintf("正在向内存池申请内存........\n");
    
    p_test = rt_malloc(TEST_SIZE);  
    if(p_test==RT_NULL)
    {
        rt_kprintf("动态内存申请失败！\n");
        LED0_TOGGLE;
    }
    else
    {
        rt_kprintf("动态内存申请成功，地址为0x%x！\n\n",p_test);
    }
    
    rt_kprintf("正在向p_test写入数据........\n");
    *p_test = 1234;
    rt_kprintf("已经写入p_test地址的数据\n");
    rt_kprintf("*p_test =  %.4d，地址为0x%x。\n\n",*p_test,p_test);
    
    while (1)
    {
        LED0_TOGGLE;
        rt_thread_delay(1000);
    }
    
}


void free_thread_entry(void* parameter)
{
    rt_kprintf("正在释放内存........\n");
    rt_free(p_test);
    rt_kprintf("释放内存成功！\n\n");
    
    while (1)
    {
        LED1_TOGGLE;
        rt_thread_delay(500);   	
    }

}
