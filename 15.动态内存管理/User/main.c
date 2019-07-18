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
static rt_thread_t alloc_thread=RT_NULL;
static rt_thread_t free_thread=RT_NULL;

static rt_uint32_t *p_test = RT_NULL;


#define TEST_SIZE 100

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void alloc_thread_entry(void* parameter);
static void free_thread_entry(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("��̬�ڴ����ʵ��\n");
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
*                             �̶߳���
*************************************************************************
*/
void alloc_thread_entry(void* parameter)
{
    rt_kprintf("�������ڴ�������ڴ�........\n");
    
    p_test = rt_malloc(TEST_SIZE);  
    if(p_test==RT_NULL)
    {
        rt_kprintf("��̬�ڴ�����ʧ�ܣ�\n");
        LED0_TOGGLE;
    }
    else
    {
        rt_kprintf("��̬�ڴ�����ɹ�����ַΪ0x%x��\n\n",p_test);
    }
    
    rt_kprintf("������p_testд������........\n");
    *p_test = 1234;
    rt_kprintf("�Ѿ�д��p_test��ַ������\n");
    rt_kprintf("*p_test =  %.4d����ַΪ0x%x��\n\n",*p_test,p_test);
    
    while (1)
    {
        LED0_TOGGLE;
        rt_thread_delay(1000);
    }
    
}


void free_thread_entry(void* parameter)
{
    rt_kprintf("�����ͷ��ڴ�........\n");
    rt_free(p_test);
    rt_kprintf("�ͷ��ڴ�ɹ���\n\n");
    
    while (1)
    {
        LED1_TOGGLE;
        rt_thread_delay(500);   	
    }

}
