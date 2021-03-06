#include "key_matrix.h"
#include "gpio_cfg.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"
#include "key2oled.h"
 
//保存按键状态
KEY_MSG   KEY_MSG_FIFO[KEY_MSG_FIFO_MAX];

//按键按下次数，达到一定次数才认为按下
u32 Key_Press_Time[KEY_MAX];

/***********************************************************************************
函数名称：void Key_Init(void)
功    能：初始化键盘的IO端口
参    数：无
返回值  ：无
************************************************************************************/
void Key_Init(void)
{   
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYIN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		   //配置IO方向
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //设置上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin  = KEYOUTC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //配置IO方向
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //输出方式（推挽或者开漏�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	       //设置上下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin  = KEYOUTD_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		   //配置IO方向
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   //输出方式（推挽或者开漏）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //设置上下拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);  
}

/**************************************************************************************
函数名称：void  KeyPin_Out(u16 Number)
功    能：键盘IO口输出
参    数：Number：键盘输出第几行
返回值  ：无
***************************************************************************************/

void  KeyPin_Out(u16 Number)   
{
	switch(Number)
	{
		case 0: Key_Out_0;break;
		case 1: Key_Out_1;break;
		case 2: Key_Out_2;break;
		case 3: Key_Out_3;break;
		default:break;
	}
}

/*************************************************************************************
函数名称：void Send_Key_msg(KEY_MSG key_msg)
功    能：存储键盘IO口键值与状态
参    数：key_msg：键盘状态信息
返回值  ：无
**************************************************************************************/

void Send_Key_msg(KEY_MSG key_msg)
{
	KEY_MSG_FIFO[key_msg.key].status=key_msg.status;
	KEY_MSG_FIFO[key_msg.key].key = key_msg.key;		
}

/*************************************************************************************
函数名称：void Send_Key_msg(KEY_MSG key_msg)
功    能：存储键盘IO口键值与状态
参    数：key_msg：键盘状态信息
返回值  ：无
**************************************************************************************/
/*************************************** 
            key_Val 对应键值 
      列：[PC3]  [PC2]  [PC1]   [PC0]
            ↓      ↓      ↓      ↓
 行：
[PD1]→      0      1      2      3   
[PD0]→      4      5      6      7    
[PC5]→      8      9     10     11   
[PC4]→     12     13     14     15   
***************************************/

void Check_Key(void)
{   
  s8 j,row,col,tmp2;
  KEY_MSG  key_msg;
  for(row = 0;row < 4;row++)            //行扫描
  {
    KeyPin_Out(row);          //B(00001111)                                            
    if((KEY_IN & 0x0f)<0x0f)              //是否某列中有一位为0
    {                       
      tmp2 = 0x01;                        // tmp2用于检测出哪一位为0  
      for(col = 0;col < 4;col++)          // 列检测 
      {    
        if((KEY_IN & tmp2) == 0x00)
        {   
           tmp2 <<= 1;                       // tmp2右移1位 
           Key_Press_Time[row*4+col]++;  
           if(Key_Press_Time[row*4+col] <= KEY_DOWN_TIME)
           {
						 col = -1;
						 tmp2 = 0x01; 
						 continue;
           }
           else 
           {
						 for(j = 0;j < 16;j++)
							KEY_MSG_FIFO[j].status = KEY_UP;
						 key_msg.key=(KEY_e)(row*4+(3-col));   
						 key_msg.status=KEY_DOWN;
						 Send_Key_msg(key_msg);
           } 
         }
         else
         {
						key_msg.key=(KEY_e)(row*4+(3-col)); 
						key_msg.status=KEY_UP;
						Key_Press_Time[row*4+col]=0;
						Send_Key_msg(key_msg);  
						tmp2 <<= 1;                       // tmp2右移1位 
         }
       }
     }
		else
		{
			for(j = 4*row;j < 4*row+4 ;j++)
				KEY_MSG_FIFO[j].status = KEY_UP;
		}
  }	
}   



