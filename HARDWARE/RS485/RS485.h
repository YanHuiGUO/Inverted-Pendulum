#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
  	
extern u8 RS485_RX_BUF[16]; 		//接收缓冲,最大64个字节
extern u8 RS485_RX_CNT;   			//接收到的数据长度

#define     McUart        USART3
#define     McSerial	    115200

//模式控制
#define RS485_TX_EN		PDout(15)	//485模式控制.0,接收;1,发送.
//如果想串口中断接收，设置EN_USART2_RX为1，否则设置为0
#define EN_USART3_RX 	1			//0,不接收;1,接收.

typedef struct
{ unsigned char flag;
	unsigned char caught;
	unsigned char cache;
	unsigned int count_num;
	unsigned int out;
	
	unsigned char data[];
	
}Bianmastr;
														 
void RS485_Init(USART_TypeDef* USARTx);
void RS485_Send_Data(u8 *buf,u8 len);
void RS485_Receive_Data(u8 *buf,u8 *len);		 
void RS485_Task(void);

#endif	   
















