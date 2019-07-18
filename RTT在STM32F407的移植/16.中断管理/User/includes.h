#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32f4xx.h" 
#include "stdio.h"
#include "string.h"
#include "math.h"

//ϵͳ���ͷ�ļ�
#include "board.h"
#include "rtthread.h"


//BSPͷ�ļ�
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_key.h"
#include "bsp_nvic.h"
#include "bsp_exti.h"



extern rt_sem_t test_sem;
extern rt_mq_t test_mq;


#endif


