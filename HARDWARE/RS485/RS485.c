#include "RS485.h"	 
#include "delay.h"	 

#if EN_USART3_RX   		//如果使能了接收   	  
//接收缓存区 	
u8 RS485_RX_BUF[16];  	//接收缓冲,最大64个字节.
//接收到的数据长度
u8 RS485_RX_CNT=0;  
int buff4;
#endif										 
//初始化IO 串口2
//bound:波特率	  
void RS485_Init(USART_TypeDef* USARTx)
{  	 	
		GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);   //TX   ????
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);   //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = McSerial;   //?????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USARTx, &USART_InitStructure);
    USART_Cmd(USARTx, ENABLE);      //????
    USART_ClearFlag(USARTx, USART_FLAG_TC);	 //?????
    USART_ITConfig(USARTx,USART_IT_RXNE,ENABLE);	//??????


//*********************4096???????485?????B12***********************//////
     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
     GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;             
     GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
     GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void USART3_IRQHandler(void)
{			   
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//接收到数据
	{	 	
		RS485_RX_BUF[RS485_RX_CNT] = USART_ReceiveData(USART3);
		if(RS485_RX_BUF[RS485_RX_CNT] == 'X')
		{
			RS485_RX_BUF[0] = 'X';
			RS485_RX_CNT = 0;
		}
		RS485_RX_CNT++;	
		if(RS485_RX_CNT == 16) 
		{RS485_RX_CNT = 0;
			buff4++;
		}
	}  											 
} 
