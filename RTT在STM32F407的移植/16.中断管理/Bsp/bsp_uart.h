#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

#define DEBUG_USART  (USART1)       //调试串口宏定义
// 串口对应的DMA请求通道
#define  USART_RX_DMA_CHANNEL      DMA_Channel_4
#define  DEBUG_USART_DMA_STREAM    DMA2_Stream2
// 外设寄存器地址
#define  USART_DR_ADDRESS        (&DEBUG_USART->DR)


////////////////////////////////////////////////////////////////////////////////// 	
#define USART1_REC_LEN  	64   //定义最大接收字节数 64
#define USART1_TX_LEN       64   //最大发射字节数     64

#define USART2_REC_LEN  	64  //定义最大接收字节数  64
#define USART2_TX_LEN       64  //最大发射字节数      64

#define USART3_REC_LEN  	64   //定义最大接收字节数 64
#define USART3_TX_LEN       64  //最大发射字节数      64

#define USART4_REC_LEN  	64   //定义最大接收字节数 64
#define USART4_TX_LEN       64   //最大发射字节数     64

#define USART5_REC_LEN  	64   //定义最大接收字节数 64
#define USART5_TX_LEN       64   //最大发射字节数     64

#define USART6_REC_LEN  	64   //定义最大接收字节数 64
#define USART6_TX_LEN       64   //最大发射字节数     64


#define TX_FREE   0x00
#define TX_BUZY   0x01
#define TX_OK     0x02
#define RX_FREE    0x10
#define RX_WAIT    0x11   //该状态表示,接收处于等待状态,即发射后需要回复的状态
#define RX_OK      0x12 
#define RX_TIMEOUT 0x14
#define RX_MARK    0x18   //for mark use 


////////////frm_sta///////////////// 
#define FR_W_BEGIN  0x22
#define FR_W_S      0x23
#define FR_W_NEXT   0x29
#define FR_W_END    0x2E
#define FR_W_SUM    0x24  
#define FR_W_0x0A   0x25
#define FR_W_0x0D   0x26  
#define FR_OK       0x2F 

////////////////////////////////////////////////////////


typedef	struct
{
	u8 tx_index;   //当前发送字节位置
	u8 tx_sta;	   //发送状态  free  buzy 
	u8 tx_order;   //待发送字节数
	u8 rx_index;   //接收字节数量
	u8 rx_sta;	   //接收状态  buzy  free  wait  rxok  
	u8 rx_time;    //超时计数   
	u8 frm_sta;     //帧状态
	u8 rx_order;    //待收字节数
} UART_STA;

/////////////////////////////////////////////////////////

extern u8 USART1_RX_BUF[USART1_REC_LEN];     
extern u8 USART1_TX_BUF[USART1_TX_LEN];
extern u8 USART2_RX_BUF[USART2_REC_LEN];     
extern u8 USART2_TX_BUF[USART2_TX_LEN];
extern u8 USART3_RX_BUF[USART3_REC_LEN]; 
extern u8 USART3_TX_BUF[USART3_TX_LEN];
extern u8 USART4_TX_BUF[USART4_TX_LEN];
extern u8 USART4_RX_BUF[USART4_REC_LEN];
extern u8 USART5_TX_BUF[USART5_TX_LEN];
extern u8 USART5_RX_BUF[USART5_REC_LEN];
extern u8 USART6_TX_BUF[USART6_TX_LEN];
extern u8 USART6_RX_BUF[USART6_REC_LEN];


extern UART_STA g_UART1_STA;
extern UART_STA g_UART2_STA;
extern UART_STA g_UART3_STA;
extern UART_STA g_UART4_STA;
extern UART_STA g_UART5_STA;
extern UART_STA g_UART6_STA;

/////////////////////////////////////////////////////////////
void Uart_Init(void);
void Uart_Send(USART_TypeDef* USARTx,u8 *bufer,u16 len);


#endif

