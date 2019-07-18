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
static rt_thread_t receive_thread=RT_NULL;
static rt_thread_t send_thread=RT_NULL;

static rt_mailbox_t test_mail = RT_NULL;


char test_str1[] = "this is a mail test 1";
char test_str2[] = "this is a mail test 2";

/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void receive_thread_entry(void* parameter);
static void send_thread_entry(void* parameter);


/*
*************************************************************************
*                             main ����
*************************************************************************
*/
int main(void)
{ 
    rt_kprintf("\n\n");
    rt_kprintf("����ʵ��\n");
    rt_kprintf("\n\n");
    //����һ������
    test_mail = rt_mb_create("test_mail",
                             10,
                             RT_IPC_FLAG_FIFO);
    if(test_mail!=RT_NULL)
    {
        rt_kprintf("���䴴���ɹ�!\n\n");
    }
    
    //���������߳�
    receive_thread = 
	rt_thread_create( "receive",
                    receive_thread_entry,
                    RT_NULL,
                    512,
                    3,
                    20);
    if(receive_thread!=RT_NULL)
    {
        rt_thread_startup(receive_thread);
    }
    else
    {
        return -1;
    }
    //���������߳�
    send_thread = 
	rt_thread_create( "send",
                    send_thread_entry,
                    RT_NULL,
                    512,
                    2,
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
*                             �̶߳���
*************************************************************************
*/
void receive_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    char *r_str;
    
    while (1)
    {
        uwRet = rt_mb_recv(test_mail,
                            (rt_uint32_t *)&r_str,
                            RT_WAITING_FOREVER);
        if(uwRet==RT_EOK)
        {
            rt_kprintf("���������ǣ�%s\n\n",r_str);
            LED0_TOGGLE;
        }
        else
        {
            rt_kprintf("������մ��󣡴��������0x%x\n",uwRet);
        }
    }
    
}


void send_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    u8 key_sta;
    
    while (1)
    {
        key_sta = KEY_Scan(0);
        
        if(key_sta==1)
        {
            rt_kprintf("KEY1�����£�\n");
            uwRet = rt_mb_send(test_mail,(rt_uint32_t)&test_str1);
            if(uwRet==RT_EOK)
            {
                rt_kprintf("������Ϣ���ͳɹ���\n");
            }
            else
            {
                rt_kprintf("������Ϣ����ʧ�ܣ�\n");
            }
        }
        else if(key_sta==2)
        {
            rt_kprintf("KEY2�����£�\n");
            uwRet = rt_mb_send(test_mail,(rt_uint32_t)&test_str2);
            if(uwRet==RT_EOK)
            {
                rt_kprintf("������Ϣ���ͳɹ���\n");
            }
            else
            {
                rt_kprintf("������Ϣ����ʧ�ܣ�\n");
            }
            
        }
        
        rt_thread_delay(20);   	
    }

}
