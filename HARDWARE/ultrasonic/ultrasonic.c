#include "ultrasonic.h"
#include "Timer.h"
#include "delay.h"
#include "UartLink.h"

int ultra_count = 0;
int distance = 0;
int Ultra_Dis[5];

/*************************************************************************************************
@f_name: void TIM3_Init(u16 Prescaler,u16 Period)
@brief:	 作超声波返回高电平的计时器
@param:	 Prescaler  分频值  Period 周期
@return: None
***************************************************************************************************/

void TIM13_Init(u32 TIM_scale, u32 TIM_Period)//TIM_Period为16位的数
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//    NVIC_InitTypeDef  NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = TIM_Period;//计数器重装值
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
@brief:	 超声波硬件模块初始化
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
	
	GPIO_InitStructure.GPIO_Pin = ULTRA_TRIG;   //LED0和LED1对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;            //普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;           //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);                   //初始化GPIO
	GPIO_ResetBits(ULTRA_PORT,ULTRA_TRIG);

	GPIO_InitStructure.GPIO_Pin = ULTRA_ECHO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(ULTRA_PORT, &GPIO_InitStructure);//初始化GPIOE2,3,4

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE2 连接到中断线2

  /* 配置EXTI_Line13 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line3;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
  EXTI_Init(&EXTI_InitStructure);//配置
	//中断设置，优先级寄存器划分
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//中断
	NVIC_InitStructure.NVIC_IRQChannel=EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/*************************************************************************************************
@f_name: void Ranging(void)
@brief:	 超声波发射信号， 测量过程发现测量静态的比较稳定，动态的不稳定，3米范围内比较稳定
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
@brief:	 超声波输入外部中断
@param:	 None
@return: None
***************************************************************************************************/

void EXTI3_IRQHandler(void)
{    			    
   if(EXTI_GetFlagStatus(EXTI_Line3))  //确定是EXTI6中断
   {  
     if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==SET)  //如果是高电平进来的
	  {
         TIM_SetCounter(TIM13,0);          //装载初值   
         TIM_Cmd(TIM13, ENABLE);    //使能使能TIM2	     
	  }

	  if(GPIO_ReadInputDataBit(ULTRA_PORT,ULTRA_ECHO)==RESET)  //如果是低电平进来
	  {
	     TIM_Cmd(TIM13, DISABLE);    //使能使能TIM2
			 ultra_count=TIM_GetCounter(TIM13);
			 Ultra_Distance();
	  }
	  EXTI_ClearFlag(EXTI_Line3);	   //清除中断标志位
   }   
}

/*************************************************************************************************
@f_name: void Ultra_Distance(void)
@brief:	 超声波距离计算，取10次平均，单位为mm
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




