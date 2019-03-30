#include "key2oled.h"
#include "OLED.h"
#include "key_matrix.h"

typedef void (* Task_Show)(void);
KEY_FLAG Key_Flag;
s16  Key_Number,Key_Down_Right;
//函数指针数组，按键与OLED显示
Task_Show Task_ShowCarry[] = {A_Show,B_Show,C_Show,D_Show,
															E_Show,F_Show,G_Show,H_Show,
															I_Show,J_Show,K_Show,L_Show,
															M_Show,N_Show,O_Show,P_Show};

/*************************************************************************************
函数名称：void Key_deal(void)
功    能：按键处理
参    数：None
返回值  ：None
**************************************************************************************/
															
void Key_deal(void)
{
	u8 i = 0,j = 0;
	Check_Key();
	for(i = 0;i < 16;i++)
	{
		if((KEY_MSG_FIFO[i].status == KEY_DOWN)&&(Key_Down_Right == 1))
		{	
			Key_Down_Right = 0;
			(*Task_ShowCarry[KEY_MSG_FIFO[i].key])();
		}
		if(KEY_MSG_FIFO[i].status == KEY_UP)
		{
			j++;
			if(j == 16)
			{
				Key_Down_Right = 1;
			}
		}
	}
}

/*************************************************************************************
函数名称：void A_Show(void)
功    能：0值按键按下后OLED显示
参    数：None
返回值  ：None
**************************************************************************************/

void A_Show(void)
{
	Key_Flag.Flag_1 = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"1");
}

void B_Show(void)
{
	Key_Flag.Flag_1 = 2;
	OLED_Clear();
	OLED_P6x8Str(0,0,"2");
}

void C_Show(void)
{
	Key_Flag.Flag_1 = 3;
	OLED_Clear();
	OLED_P6x8Str(0,0,"3");
}

void D_Show(void)
{
	Key_Flag.Flag_A = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"A");
}

void E_Show(void)
{
	Key_Flag.Flag_1 = 4;
	OLED_Clear();
	OLED_P6x8Str(0,0,"4");
}

void F_Show(void)
{
	Key_Flag.Flag_1 = 5;
	OLED_Clear();
	OLED_P6x8Str(0,0,"5");
}

void G_Show(void)
{
	Key_Flag.Flag_1 = 6;
	OLED_Clear();
	OLED_P6x8Str(0,0,"6");
}

void H_Show(void)
{
	Key_Flag.Flag_B = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"B");
}

void I_Show(void)
{
	Key_Flag.Flag_7 = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"7");
}

void J_Show(void)
{
	Key_Flag.Flag_8 = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"8");
}

void K_Show(void)
{
	Key_Flag.Flag_9 = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"9");
}

void L_Show(void)
{
	Key_Flag.Flag_C = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"C");
}

void M_Show(void)
{
	Key_Flag.Flag_Star = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"*");
}

void N_Show(void)
{
	Key_Flag.Flag_0 = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"0");
}

void O_Show(void)
{
	Key_Flag.Flag_Enter = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"Enter");
}

void P_Show(void)
{
		Key_Flag.Flag_D = 1;
	OLED_Clear();
	OLED_P6x8Str(0,0,"D");
}

