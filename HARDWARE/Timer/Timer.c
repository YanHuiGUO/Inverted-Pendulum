#include "Timer.h"
#include "pwm.h"
#include "QEI.h"
#include "led.h"
#include "key_matrix.h"
#include "MyNvic.h"
#include "key2oled.h"
#include "dmp_mpu6050.h"
#include "TeeterBoard.h"
#include "ultrasonic.h"
#include "nrf24l01.h"
#include "Invert_Pend.h"
#include "RS485.h"	
#define 	CONTROL_PERIOD 	50

char Half_Sec = 0;
u8   Cir_time;
u32  SysRunSecond;
bool Key_Start;
u32  Now_Time = 0;
u32  SysRunSecond;
extern s32 Left_Speed,Right_Speed;//��������ȡ���ٶ�ֵ
u8 tmp_buf[4];
int buff2,buff3;	

int a=0;
/***************************************************************************************
@f_name: void TIM5_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	 	Tim5ʱ�ӳ�ʼ����Ƶ��84M����ʱδ�õ�
@param:	 TIM_scale = ��Ƶ��, TIM_Period = ����
@return: None
****************************************************************************************/
void TIM5_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ����
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    //	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
    //TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM5, ENABLE);
}

/***************************************************************************************
@f_name: void TIM7_Init(u32 TIM_scale, u32 TIM_Period)
@brief:	 ϵͳ�������ʱ�ӳ�ʼ����1ms��һ���жϣ�Ƶ��84M
@param:	 TIM_scale = ��Ƶ��, TIM_Period = ����
@return: None
****************************************************************************************/

void TIM7_Init(u32 TIM_scale, u32 TIM_Period)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM7, ENABLE);
    MyNVIC_Init(NVIC_PriorityGroup_2,2,0,TIM7_IRQn,ENABLE);	      //QEI�ٶȱջ�,���ܻ��к�վʱ��Ŀ��Ƴ�����ôͨ���жϽ���ȫ���ܵ�Ӱ�죬�����������ȼ��������ͨ�ŵ�
}

/***************************************************************************************
@f_name: void TIM7_IRQHandler(void)
@brief:	 ϵͳ�������ʱ���ж�
@param:	 None
@return: None
****************************************************************************************/

void TIM7_IRQHandler(void)
{
	if (TIM7->SR&TIM_IT_Update && TIM7->DIER&TIM_IT_Update)
  {		
		if( Cir_time >= CONTROL_PERIOD)
		{  
			Cir_time = 0;
		}
		if(Cir_time%1 == 0)
		{ 	
			buff2++;
		}
		if(Cir_time%2 == 0)
		{		
		}
		if(Cir_time%3 == 0)
		{ 		
		}
		if(Cir_time%4 == 0)
		{ 				
		}
		if(Cir_time%5 == 0)
		{		
			Encoder_PID.Pre_pulse = Encoder_PID.pulse;
			Encoder_PID.pulse = (RS485_RX_BUF[11]-48)*1000+(RS485_RX_BUF[12]-48)*100+(RS485_RX_BUF[13]-48)*10+RS485_RX_BUF[14]-48;
			Invert_Pend_Task();	
		}
		if(Cir_time%10 == 0)
		{			
			
		} 
		if(Cir_time%50 == 0)
		{ 	
			//Ultra_Ranging();			   //��������������⵽�ز��������ⲿ�жϣ��������
			Key_deal();              //���̴�����
	
			Half_Sec++;
			if(Half_Sec == 8)
			{

			
				Half_Sec = 0;						
		  }  

	  }
		Cir_time++; 
		TIM7->SR = (UInt16)~TIM_IT_Update;
	}
}
