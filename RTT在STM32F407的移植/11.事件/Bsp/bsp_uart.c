#include "includes.h"

/*
	������Դ����
	
	usart1: 
	usart2: 
	usart3: 
	usart4: 
	usart5: 
	usart6: 
*/

u8 USART1_RX_BUF[USART1_REC_LEN];
u8 USART1_TX_BUF[USART1_TX_LEN];

u8 USART2_RX_BUF[USART2_REC_LEN];
u8 USART2_TX_BUF[USART2_TX_LEN];

u8 USART3_RX_BUF[USART3_REC_LEN];
u8 USART3_TX_BUF[USART3_TX_LEN];

u8 USART4_RX_BUF[USART4_REC_LEN];
u8 USART4_TX_BUF[USART4_TX_LEN];

u8 USART5_RX_BUF[USART5_REC_LEN];
u8 USART5_TX_BUF[USART5_TX_LEN];

u8 USART6_RX_BUF[USART6_REC_LEN];
u8 USART6_TX_BUF[USART6_TX_LEN];


UART_STA g_UART1_STA;
UART_STA g_UART2_STA;
UART_STA g_UART3_STA;
UART_STA g_UART4_STA;
UART_STA g_UART5_STA;
UART_STA g_UART6_STA;



#if 1
	#pragma import(__use_no_semihosting)

struct __FILE
{
	int handle;
};

FILE __stdout;

int _sys_exit(int x)
{
	x=x;
	return x;
}

/// �ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// �ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

#endif




//////////////////////////////////////////
//��ʼ��IO ����1   
//bound:������
////////////////////////////////////////////
void Uart1_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1  tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1  rx

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1

	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�
	
	g_UART1_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//����1�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 i;
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		switch(g_UART1_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART1_RX_BUF[g_UART1_STA.rx_index]=Res;
								g_UART1_STA.rx_index = 1;
								g_UART1_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART1_STA.rx_index = 0;
								g_UART1_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART1_RX_BUF[g_UART1_STA.rx_index]=Res;
								g_UART1_STA.rx_index = 2;
								g_UART1_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART1_STA.rx_index = 0;
								g_UART1_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART1_RX_BUF[g_UART1_STA.rx_index]=Res;
							g_UART1_STA.rx_index++;
			
							if(g_UART1_STA.rx_index>64)
							{
								for(i=0;i<g_UART1_STA.rx_index;i++)
								 {
									USART1_RX_BUF[g_UART1_STA.rx_index] = 0;
								 }
								 g_UART1_STA.rx_index = 0;
								 g_UART1_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART1_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART1_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART1_RX_BUF[g_UART1_STA.rx_index]=Res;
							g_UART1_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART1_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART1_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}
		
	}
}
	


//////////////////////////////////////////
//��ʼ��IO ����2   
//bound:������
////////////////////////////////////////////
void Uart2_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��

	//����2��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2  tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2  rx

	//USART2�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA2��PA3

	//USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2

	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2 

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�

	g_UART2_STA.frm_sta = FR_W_BEGIN;
}



///////////////////////////////////////////////////////////////////
//����2�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
///////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//��ȡ���յ�������
		switch(g_UART2_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART2_RX_BUF[g_UART2_STA.rx_index]=Res;
								g_UART2_STA.rx_index = 1;
								g_UART2_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART2_STA.rx_index = 0;
								g_UART2_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART2_RX_BUF[g_UART2_STA.rx_index]=Res;
								g_UART2_STA.rx_index = 2;
								g_UART2_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART2_STA.rx_index = 0;
								g_UART2_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART2_RX_BUF[g_UART2_STA.rx_index]=Res;
							g_UART2_STA.rx_index++;
			
							if(g_UART2_STA.rx_index>64)
							{
								for(i=0;i<g_UART2_STA.rx_index;i++)
								 {
									USART2_RX_BUF[g_UART2_STA.rx_index] = 0;
								 }
								 g_UART2_STA.rx_index = 0;
								 g_UART2_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART2_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART2_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART2_RX_BUF[g_UART2_STA.rx_index]=Res;
							g_UART2_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART2_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART2_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}
		
	}		
} 




//////////////////////////////////////////
//��ʼ��IO ����3  
//bound:������
////////////////////////////////////////////
void Uart3_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIObʱ��  pb 10 tx
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //               pc 5  rx
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART3ʱ��

	//����3��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource5,GPIO_AF_USART3); //GPIOC5����ΪUSART3  rx

	//USART3�˿�����
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;          //GPIOB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOB,&GPIO_InitStructure); //��ʼ��PA9��PA10

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;          //GPIOC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//USART3 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure); //��ʼ������3

	USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//��������ж�
	
	g_UART3_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//����3�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
////////////////////////////////////////////////////////////////////
void USART3_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		switch(g_UART3_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART3_RX_BUF[g_UART3_STA.rx_index]=Res;
								g_UART3_STA.rx_index = 1;
								g_UART3_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART3_STA.rx_index = 0;
								g_UART3_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART3_RX_BUF[g_UART3_STA.rx_index]=Res;
								g_UART3_STA.rx_index = 2;
								g_UART3_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART3_STA.rx_index = 0;
								g_UART3_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART3_RX_BUF[g_UART3_STA.rx_index]=Res;
							g_UART3_STA.rx_index++;
			
							if(g_UART3_STA.rx_index>64)
							{
								for(i=0;i<g_UART3_STA.rx_index;i++)
								 {
									USART3_RX_BUF[g_UART3_STA.rx_index] = 0;
								 }
								 g_UART3_STA.rx_index = 0;
								 g_UART3_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART3_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART3_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART3_RX_BUF[g_UART3_STA.rx_index]=Res;
							g_UART3_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART3_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART3_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}
		
	}   		     

} 
	


//////////////////////////////////////////
//��ʼ��IO ����4  
//bound:������
////////////////////////////////////////////
void Uart4_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART4ʱ��

	//����4 IO  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA0����ΪUSART4  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA1����ΪUSART4  rx

	//USART4�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //GPIOC10��GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	//USART4 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART4, &USART_InitStructure); //��ʼ������4

	USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���4

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//��������ж�

	g_UART4_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//����4�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
////////////////////////////////////////////////////////////////////
void UART4_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //
	{
		Res =USART_ReceiveData(UART4);//(USART4->DR);	//��ȡ���յ�������
		
		switch(g_UART4_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART4_RX_BUF[g_UART4_STA.rx_index]=Res;
								g_UART4_STA.rx_index = 1;
								g_UART4_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART4_STA.rx_index = 0;
								g_UART4_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART4_RX_BUF[g_UART4_STA.rx_index]=Res;
								g_UART4_STA.rx_index = 2;
								g_UART4_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART4_STA.rx_index = 0;
								g_UART4_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART4_RX_BUF[g_UART4_STA.rx_index]=Res;
							g_UART4_STA.rx_index++;
			
							if(g_UART4_STA.rx_index>64)
							{
								for(i=0;i<g_UART4_STA.rx_index;i++)
								 {
									USART4_RX_BUF[g_UART4_STA.rx_index] = 0;
								 }
								 g_UART4_STA.rx_index = 0;
								 g_UART4_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART4_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART4_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART4_RX_BUF[g_UART4_STA.rx_index]=Res;
							g_UART4_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART4_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART4_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}

	}	
} 


//////////////////////////////////////////
//��ʼ��IO ����5 
//bound:������
////////////////////////////////////////////
void Uart5_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOcʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIODʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��USART5ʱ��

	//����5 IO  pa0 pa1 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOA0����ΪUSART5  tx
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOA1����ΪUSART5  rx

	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC12

	//USART5�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PD2

	//USART5 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(UART5, &USART_InitStructure); //��ʼ������5

	USART_Cmd(UART5, ENABLE);  //ʹ�ܴ���5 

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//��������ж�

	g_UART5_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//����5�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
////////////////////////////////////////////////////////////////////
void UART5_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������

		switch(g_UART5_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART5_RX_BUF[g_UART5_STA.rx_index]=Res;
								g_UART5_STA.rx_index = 1;
								g_UART5_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART5_STA.rx_index = 0;
								g_UART5_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART5_RX_BUF[g_UART5_STA.rx_index]=Res;
								g_UART5_STA.rx_index = 2;
								g_UART5_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART5_STA.rx_index = 0;
								g_UART5_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART5_RX_BUF[g_UART5_STA.rx_index]=Res;
							g_UART5_STA.rx_index++;
			
							if(g_UART5_STA.rx_index>64)
							{
								for(i=0;i<g_UART5_STA.rx_index;i++)
								 {
									USART5_RX_BUF[g_UART5_STA.rx_index] = 0;
								 }
								 g_UART5_STA.rx_index = 0;
								 g_UART5_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART5_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART5_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART5_RX_BUF[g_UART5_STA.rx_index]=Res;
							g_UART5_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART5_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART5_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}
		
    }
} 



//////////////////////////////////////////
//��ʼ��IO ����6  
//bound:������
////////////////////////////////////////////
void Uart6_Init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOCʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��

	//����6 IO  pc7  rx  pc6 tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6����ΪUSART6  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7����ΪUSART6  rx

	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PC12

	//USART6��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������6

	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���6 

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	
	g_UART6_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//����6�жϷ������
//����֡��ʽΪ��$:��ͷ  0D 0A��β
////////////////////////////////////////////////////////////////////
void USART6_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART6);//(USART1->DR);	//��ȡ���յ�������

		switch(g_UART6_STA.frm_sta)
		{
			case FR_W_BEGIN:
							if(Res=='$')
							{
								USART6_RX_BUF[g_UART6_STA.rx_index]=Res;
								g_UART6_STA.rx_index = 1;
								g_UART6_STA.frm_sta = FR_W_S;

							}
							else
							{
								g_UART6_STA.rx_index = 0;
								g_UART6_STA.frm_sta = FR_W_BEGIN;
							}
							break;
			
			case FR_W_S:
							if(Res==':')
							{
								USART6_RX_BUF[g_UART6_STA.rx_index]=Res;
								g_UART6_STA.rx_index = 2;
								g_UART6_STA.frm_sta = FR_W_0x0D;
							}
							else
							{
								g_UART6_STA.rx_index = 0;
								g_UART6_STA.frm_sta = FR_W_BEGIN;
							}
					        break;
			
			case FR_W_0x0D:
							
							USART6_RX_BUF[g_UART6_STA.rx_index]=Res;
							g_UART6_STA.rx_index++;
			
							if(g_UART6_STA.rx_index>64)
							{
								for(i=0;i<g_UART6_STA.rx_index;i++)
								 {
									USART6_RX_BUF[g_UART6_STA.rx_index] = 0;
								 }
								 g_UART6_STA.rx_index = 0;
								 g_UART6_STA.frm_sta = FR_W_BEGIN;
							}
							else
							{
								if(Res==0x0D)
								{
									g_UART6_STA.frm_sta = FR_W_0x0A;
								}
								else
								{
									g_UART6_STA.frm_sta = FR_W_0x0D;
								}
							}
					        break;
			
			case FR_W_0x0A:
							USART6_RX_BUF[g_UART6_STA.rx_index]=Res;
							g_UART6_STA.rx_index++;
			
							if(Res==0x0A)
							{
								g_UART6_STA.frm_sta = FR_OK;
							}
							else
							{
								g_UART6_STA.frm_sta = FR_W_0x0D;
							}
							
							break;
			
			default: break;
		}
		
    }
} 




//////////////////////////////////////��ѯ�ȴ�����////////////////////////////////////////

///////////////////////////////////////////////////
//	���� ���ͳ���
//  USARTx �Ǵ��ں�
//  bufer �Ƿ�������
//  len �Ǵ�����֡��
//////////////////////////////////////////////////
void Uart_Send(USART_TypeDef* USARTx,u8 *bufer,u16 len)
{
	u8 i;
	
	for(i=0;i<len;i++)
	{	
		USART_SendData(USARTx, (uint8_t)(*(bufer+i)));		
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET) {}	
	}
}






//////////////////////////////////���ڲ���/////////////////////////////////////////////////
void Usart_Test(void)
{
	Uart_Send(USART1,(u8 *)" uart1 on line!\r\n",17);//ok
//	Uart_Send(USART2,(u8 *)" uart2 on line!\r\n",17);//ok
//	Uart_Send(USART3,(u8 *)" uart3 on line!\r\n",17);//ok
//	Uart_Send(UART4,(u8 *)" uart4 on line!\r\n",17);//ok
//	Uart_Send(UART5,(u8 *)" uart5 on line!\r\n",17);//ok
//	Uart_Send(USART6,(u8 *)" uart6 on line!\r\n",17);//ok
}

//////////////////////////////////////////
//��ʼ������  
////////////////////////////////////////////
void Uart_Init(void)
{
	Uart1_Init(115200);
	g_UART1_STA.rx_sta=0;
	g_UART1_STA.rx_index=0;

//	Uart2_Init(115200);
//	g_UART2_STA.rx_sta=0;
//	g_UART2_STA.rx_index=0;
//	
//	Uart3_Init(115200);
//	g_UART3_STA.rx_sta=0;
//	g_UART3_STA.rx_index=0;

//	Uart4_Init(115200);
//	g_UART4_STA.rx_sta=0;
//	g_UART4_STA.rx_index=0;

//	Uart5_Init(115200);
//	g_UART5_STA.rx_sta=0;
//	g_UART5_STA.rx_index=0;

//	Uart6_Init(115200);
//	g_UART6_STA.rx_sta=0;
//	g_UART6_STA.rx_index=0;
	
//	Usart_Test();

}
