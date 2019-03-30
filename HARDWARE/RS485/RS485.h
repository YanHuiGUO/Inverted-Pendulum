#ifndef __RS485_H
#define __RS485_H			 
#include "sys.h"	 								  
  	
extern u8 RS485_RX_BUF[16]; 		//���ջ���,���64���ֽ�
extern u8 RS485_RX_CNT;   			//���յ������ݳ���

#define     McUart        USART3
#define     McSerial	    115200

//ģʽ����
#define RS485_TX_EN		PDout(15)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define EN_USART3_RX 	1			//0,������;1,����.

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
















