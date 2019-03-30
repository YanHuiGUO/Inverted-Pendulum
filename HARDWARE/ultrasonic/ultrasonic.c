#include "ultrasonic.h"
#include "Timer.h"
#include "delay.h"
#include "UartLink.h"

int ultra_count = 0;
int distance = 0;
int Ultra_Dis[5];

/*************************************************************************************************
@f_name: void TIM3_Init(u16 Prescaler,u16 Period)
@brief:	 �����������ظߵ�ƽ�ļ�ʱ��
@param:	 Prescaler  ��Ƶֵ  Period ����
@return: None
***************************************************************************************************/

void TIM13_Init(u32 TIM_scale, u32 TIM_Period)//TIM_PeriodΪ16λ����
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//��������װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);
    //	TIM_PrescalerConfig(TIM3, (TIM_scale-1), TIM_PSCReloadMode_Immediate);
    //TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM13, ENABLE);
}

/********************************************************************************************************************************************
@f_name: void Ultran_Init(void)
@brief:	 ������Ӳ��ģ���ʼ��
@param:	 None
@return: None
*********************************************************************************************************************************************/
void Ultran_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
 	RCC_AHB1PeriphClockCmd(ULTRA_CLK, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//
	
	GPIO_InitStructure.GPIO_Pin = ULTRA_TRIG;   //LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);                   //��ʼ��GPIO
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);

	GPIO_InitStructure.GPIO_Pin = ULTRA_ECHO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE2 ���ӵ��ж���2

  /* ����EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	//�ж����ã����ȼ��Ĵ�������
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//�ж�
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*************************************************************************************************
@f_name: void Ranging(void)
@brief:	 �����������źţ� �������̷��ֲ�����̬�ıȽ��ȶ�����̬�Ĳ��ȶ���3�׷�Χ�ڱȽ��ȶ�
@param:	 None
@return: None
***************************************************************************************************/

void Ultra_Ranging(void)
{
		TRIG_Send=1;
		delay_us(20);
		TRIG_Send=0; 	
}

/*************************************************************************************************
@f_name: void EXTI9_5_IRQHandler(void)
@brief:	 �����������ⲿ�ж�
@param:	 None
@return: None
***************************************************************************************************/

void EXTI3_IRQHandler(void)
{    			    
   if(EXTI_GetFlagStatus(EXTI_Line3))  //ȷ����EXTI6�ж�
   {  
     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //����Ǹߵ�ƽ������
	  {
         TIM_SetCounter(TIM13,0);          //װ�س�ֵ   
         TIM_Cmd(TIM13, ENABLE);    //ʹ��ʹ��TIM2	     
	  }

	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //����ǵ͵�ƽ����
	  {
	     TIM_Cmd(TIM13, DISABLE);    //ʹ��ʹ��TIM2
			 ultra_count=TIM_GetCounter(TIM13);
			 Ultra_Distance();
	  }
	  EXTI_ClearFlag(EXTI_Line3);	   //����жϱ�־λ
   }   
}

/*************************************************************************************************
@f_name: void Ultra_Distance(void)
@brief:	 ������������㣬ȡ10��ƽ������λΪmm
@param:	 None
@return: None
***************************************************************************************************/

void Ultra_Distance(void)
{		
		int i;
		distance=(u16)(17.3641*ultra_count/100);
		Ultra_Dis[4] = distance;
		for(i = 0;i < 4;i++)
		{
			Ultra_Dis[i] = Ultra_Dis[i+1];
			distance += Ultra_Dis[i];
		}		
		distance = (u16)distance/5;
}




