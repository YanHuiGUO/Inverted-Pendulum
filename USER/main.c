#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"
#include "delay.h"
#include "common.h"
#include "OLED.h"
#include "UartLink.h"
#include "stm32f4xx_it.h"
#include "MyNvic.h"
#include "DCMI.h"
#include "IIC.h"
#include "led.h" 
#include "LedBuz.h"
#include "timer.h"
#include "pwm.h"
#include "QEI.h"
#include "dmp_driver.h"
#include "dmp_mpu6050.h"
#include "TeeterBoard.h"
#include "key_matrix.h"
#include "key2oled.h"
#include "nrf24l01.h"
#include "ultrasonic.h"
#include "RS485.h"	
#include "Invert_Pend.h"

/*
������    #define TRIG_Send  PEout(2)
				  #define ECHO_Reci  PEin(3)
nrf24l01  #define NRF24L01_CE   PDout(10) 	//24L01Ƭѡ�ź�
					#define NRF24L01_CSN  PDout(11) 	//SPIƬѡ�ź�	   
					#define NRF24L01_IRQ  PDin(14)  	//IRQ������������
		SPI   GPIOB GPIO_Pin_13
								GPIO_Pin_14
								GPIO_Pin_15
RS485 	  GPIOD GPIO_Pin_15

ʣ����չ���� GPIOB_12��GPIOE_5��GPIOE_6δʹ��
*/	

extern int buff;	
extern int buff2,buff3;
extern u8 tmp_buf[4];
int main(void)
{  		
	DisableInterrupts;

	delay_init(168);
	TIM5_Init(84,10000);
	TIM7_Init(84,1000); 
	TIM13_Init(84,0xffff);
	LED_Init();
	//BuzLed_Init();
	//Buzzer_ON;
	Key_Init();
	UartInit(USART2);
	OLED_Init();

	PWM_Init(17000);
	PID_Init();
	//Balance_PID_Init();
	QEI_Init();
  EncoderPID_Init();
	RS485_Init(USART3);
	NRF24L01_Init();
	//Ultran_Init();
	//while(!Ov7620_Init());//�ж�OV7620��ʼ���Ƿ�ɹ� 
	//MPU6050DMP_Init();
	
	/*�����ж����ȼ�*/
	MyNVIC_Init(NVIC_PriorityGroup_2,1,0,USART2_IRQn,ENABLE);	  //�������ȼ�	
	MyNVIC_Init(NVIC_PriorityGroup_2,0,0,USART3_IRQn,ENABLE);	  //�������ȼ�	
	Buzzer_OFF; 

	EnableInterrupts;
	
	RS485_TX_EN = 0;
  for(;;)
	{ 		
		//Debug_PID();
		
		push(0,Encoder_PID.pulse);
		push(1,Encoder_PID.Motor_Voltage);
		push(2,LeftSpeedCtr.SpeedCtrOut);
		push(3,LeftSpeedCtr.Cnt);
		push(4,LeftAngleCtr.Angle);
		push(5,LeftSpeedCtr.SpeedSet);
		Send_NData();   							//��ѵ���������ݵ���λ�������ɷ����жϣ�	
	}
}

