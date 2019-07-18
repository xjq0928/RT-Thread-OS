#include "includes.h"

/*
	串口资源分配
	
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

/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}

#endif




//////////////////////////////////////////
//初始化IO 串口1   
//bound:波特率
////////////////////////////////////////////
void Uart1_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1  tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1  rx

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	    //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);                   //初始化PA9，PA10

	//USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1 

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	
	g_UART1_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//串口1中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
////////////////////////////////////////////////////////////////////
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 i;
	u8 Res;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
		
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
//初始化IO 串口2   
//bound:波特率
////////////////////////////////////////////
void Uart2_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2  tx
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2  rx

	//USART2端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2

	USART_Cmd(USART2, ENABLE);  //使能串口2 

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	g_UART2_STA.frm_sta = FR_W_BEGIN;
}



///////////////////////////////////////////////////////////////////
//串口2中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
///////////////////////////////////////////////////////////////////
void USART2_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
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
//初始化IO 串口3  
//bound:波特率
////////////////////////////////////////////
void Uart3_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOb时钟  pb 10 tx
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //               pc 5  rx
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART3时钟

	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10复用为USART3  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource5,GPIO_AF_USART3); //GPIOC5复用为USART3  rx

	//USART3端口配置
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;          //GPIOB10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PA9，PA10

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;          //GPIOC5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//USART3 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3

	USART_Cmd(USART3, ENABLE);  //使能串口3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断
	
	g_UART3_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//串口3中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
////////////////////////////////////////////////////////////////////
void USART3_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART3);	//读取接收到的数据
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
//初始化IO 串口4  
//bound:波特率
////////////////////////////////////////////
void Uart4_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART4时钟

	//串口4 IO  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA0复用为USART4  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA1复用为USART4  rx

	//USART4端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11; //GPIOC10与GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); 

	//USART4 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口4

	USART_Cmd(UART4, ENABLE);  //使能串口4

	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

	g_UART4_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//串口4中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
////////////////////////////////////////////////////////////////////
void UART4_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //
	{
		Res =USART_ReceiveData(UART4);//(USART4->DR);	//读取接收到的数据
		
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
//初始化IO 串口5 
//bound:波特率
////////////////////////////////////////////
void Uart5_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOc时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOD时钟

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//使能USART5时钟

	//串口5 IO  pa0 pa1 
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOA0复用为USART5  tx
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOA1复用为USART5  rx

	//USART5端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC12

	//USART5端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PD2

	//USART5 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口5

	USART_Cmd(UART5, ENABLE);  //使能串口5 

	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断

	g_UART5_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//串口5中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
////////////////////////////////////////////////////////////////////
void UART5_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据

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
//初始化IO 串口6  
//bound:波特率
////////////////////////////////////////////
void Uart6_Init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟

	//串口6 IO  pc7  rx  pc6 tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_USART6); //GPIOC6复用为USART6  tx
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_USART6); //GPIOC7复用为USART6  rx

	//USART6端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PC12

	//USART6初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口6

	USART_Cmd(USART6, ENABLE);  //使能串口6 

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
	
	g_UART6_STA.frm_sta = FR_W_BEGIN;
}


///////////////////////////////////////////////////////////////////
//串口6中断服务程序
//接收帧格式为：$:开头  0D 0A结尾
////////////////////////////////////////////////////////////////////
void USART6_IRQHandler(void)                	
{
	u8 i,Res;

	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(USART6);//(USART1->DR);	//读取接收到的数据

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




//////////////////////////////////////轮询等待发送////////////////////////////////////////

///////////////////////////////////////////////////
//	串口 发送程序
//  USARTx 是串口号
//  bufer 是发送数组
//  len 是待发送帧长
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






//////////////////////////////////串口测试/////////////////////////////////////////////////
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
//初始化串口  
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
