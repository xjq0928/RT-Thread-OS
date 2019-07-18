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

static rt_mq_t test_mq=RT_NULL;

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
    rt_kprintf("��Ϣ����ʵ��\n");
    rt_kprintf("\n\n");
    //������Ϣ����
    test_mq = rt_mq_create("test_mq",
                            40,
                            20,
                            RT_IPC_FLAG_FIFO);
                            
    if(test_mq!=RT_NULL)
    {
        rt_kprintf("��Ϣ���д����ɹ�!\n");
    }
    
    //���������߳�
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
    //���������߳�
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
*                             �̶߳���
*************************************************************************
*/
void receive_thread_entry(void* parameter)
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
            rt_kprintf("���ν��յ�������%d\n",r_queue);
        }
        else
        {
            rt_kprintf("���ݽ��ճ���������룺0x%lx\n",uwRet);
        }
        LED0_TOGGLE;
        rt_thread_delay(200);   //��ʱ200��tick	 		
    }
    
}


void send_thread_entry(void* parameter)
{
    rt_err_t uwRet = RT_EOK;
    u32 send_data1 = 1;
    u32 send_data2 = 2;
    u8 key_sta;
    
    while (1)
    {
        key_sta = KEY_Scan(0);
        
        switch(key_sta)
        {
            case 1://K1
                uwRet = rt_mq_send(test_mq,
                                   &send_data1,
                                   sizeof(send_data1));
                
                if(uwRet!=RT_EOK)
                {
                    rt_kprintf("���ݲ��ܷ��͵���Ϣ����!������룺%lx\n",uwRet); 
                }
                break;
                
            case 2://K2
                
                uwRet = rt_mq_send(test_mq,
                                   &send_data2,
                                   sizeof(send_data2));
                if(uwRet!=RT_EOK)
                {
                    rt_kprintf("���ݲ��ܷ��͵���Ϣ����!������룺%lx\n",uwRet); 
                }
            
                break;

            case 3://K3
            
                break;

            case 4://K_UP
            
                break;
              
        }
        rt_thread_delay(20);   		
    }

}
