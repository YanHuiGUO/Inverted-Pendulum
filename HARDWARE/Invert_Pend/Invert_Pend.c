#include "QEI.h"
#include "timer.h"
#include "Invert_Pend.h"
#include "key2oled.h"
#include "delay.h"
Invert_Pend_PID  Encoder_PID;

char Function_1_Flag = 0,Function_2_Flag = 0,Function_4_Flag = 0;
float EncoderPID_fPreDelta;
float EncoderPID_fDelta = 0;
float EncoderPID_fp=0,EncoderPID_fI=0,EncoderPID_fD=0;

char Function_4_Time;
typedef void (* Task_Invert_Pend)(void);

Task_Invert_Pend Task_Invert_Pend_Carry[] = {Function_None,Function_1,Function_2,Function_3,
																						 Function_4,Function_5,Function_6};

/**************************************************************************************
@f_name: void EncoderPID_Init(void)
@brief:	 绝对式编码器PID初始化
@param:	 None
@return: None
****************************************************************************************/
void EncoderPID_Init(void)
{
	Encoder_PID.AngleSet = 180;
	Encoder_PID.Kp	= 1000;
	Encoder_PID.Kd  = 10000;
	Encoder_PID.Ki  =	0	;
	Encoder_PID.Motor_Voltage_k=0;
	Encoder_PID.Angle_k=0;
	Encoder_PID.Angle_Delta_k =0;
}
void Debug_PID(void)
{
	Encoder_PID.AngleSet = RightSpeedCtr.SpeedSet;
	Encoder_PID.Kp	= RightSpeedCtr.Kp;//20 
	Encoder_PID.Kd  = RightSpeedCtr.Ki;//1
	Encoder_PID.Ki  = RightSpeedCtr.Kd;//4		
}

/***************************************************************************************
@f_name: void EncoderPID(u16 Endoder_Pulse)
@brief:	 编码器PID
@param:	 编码器的位置值
@return: None
****************************************************************************************/
void ABS_EncoderPID(u16 Endoder_Pulse)
{
	EncoderPID_fp = 0,EncoderPID_fI = 0,EncoderPID_fD = 0;

	Encoder_PID.PreAngle = Encoder_PID.Angle;
	Encoder_PID.Angle = (float)Endoder_Pulse/ABS_Encoder_wholepulse*360; 	//将编码器回传的脉冲值解算成角度		     
											
	EncoderPID_fPreDelta = EncoderPID_fDelta;            
  EncoderPID_fDelta  = Encoder_PID.AngleSet- Encoder_PID.Angle;  //角度误差计算
	
  EncoderPID_fp =  EncoderPID_fDelta * Encoder_PID.Kp * 10;
	EncoderPID_fD =  (EncoderPID_fDelta - EncoderPID_fPreDelta) * Encoder_PID.Kd * 10;
  EncoderPID_fI =  EncoderPID_fDelta * Encoder_PID.Ki * 10;
	Encoder_PID.AngleCtrIntegral +=  EncoderPID_fI;
		
	if( Encoder_PID.AngleCtrIntegral > SPEED_INTEGRAL_MAX)
	{
		 Encoder_PID.AngleCtrIntegral = SPEED_INTEGRAL_MAX; 
	}
	else if( Encoder_PID.AngleCtrIntegral < -SPEED_INTEGRAL_MAX)
	{
			Encoder_PID.AngleCtrIntegral = -SPEED_INTEGRAL_MAX;
	}
	Encoder_PID.Motor_Acce =  EncoderPID_fp + EncoderPID_fD + EncoderPID_fI;
	Encoder_PID.Motor_Voltage =  Encoder_PID.Motor_Acce ;					//电机电压值                         
}


void Goal_Encouder_Out(float Get_Motor)
{
	Encoder_PID.AngleSet=	Get_Motor;
}

void Function_None(void)
{
	
}
void Function_1(void)
{								
	Encoder_PID.Delta_pulse = Encoder_PID.pulse - Encoder_PID.Pre_pulse;
	
	if(Function_1_Flag == 0)
	{
		LeftAngleCtr.AngleSet = 20;
		Function_1_Flag = 1;
	}
	if((Encoder_PID.pulse < 60*4096/360)&&(Encoder_PID.Delta_pulse < 0))
	LeftAngleCtr.AngleSet = 25;
	if((Encoder_PID.pulse > 300*4096/360)&&(Encoder_PID.Delta_pulse > 0))
	LeftAngleCtr.AngleSet = 15;	
	
	Left_AngleControl();
	Motor_Output_Pos(LeftAngleCtr.AngleCtrOut,0);
}

void Function_2(void)
{
	Encoder_PID.Delta_pulse = Encoder_PID.pulse - Encoder_PID.Pre_pulse;
	
	if(Function_2_Flag == 0)
	{
		LeftAngleCtr.AngleSet = 60;
		Function_2_Flag = 1;
	}
	
	if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse < 0))
	LeftAngleCtr.AngleSet = 30;//30;
	
	if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse < 0))
	LeftAngleCtr.AngleSet = 50;//45;
	
	if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse > 0))
	LeftAngleCtr.AngleSet = 70;//75;
	
	else if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse > 0))
	LeftAngleCtr.AngleSet = 90;//90;

	Left_AngleControl();
	Motor_Output_Pos(LeftAngleCtr.AngleCtrOut,0);
}

void Function_3(void)
{
	LeftSpeedCtr.SpeedCtrOut = 0;
	if((Encoder_PID.pulse < 190*4096/360)&&(Encoder_PID.pulse > 170*4096/360))	
	{
		LeftSpeedCtr.Kp = 800;
		Left_SpeedControl();
	}
	if((Encoder_PID.pulse < 210*4096/360)&&(Encoder_PID.pulse > 150*4096/360))	
	{
		LeftSpeedCtr.Kp = 2800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else if((Encoder_PID.pulse < 230*4096/360)&&(Encoder_PID.pulse > 130*4096/360))	
	{
		LeftSpeedCtr.Kp = 4800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else if((Encoder_PID.pulse < 250*4096/360)&&(Encoder_PID.pulse > 110*4096/360))	
	{
		LeftSpeedCtr.Kp = 6800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else Motor_Output_Pos(0,0);
}

void Function_4(void)
{
	if((Encoder_PID.pulse < 90*4096/360)||(Encoder_PID.pulse > 270*4096/360))
	{
		Encoder_PID.Delta_pulse = Encoder_PID.pulse - Encoder_PID.Pre_pulse;

		if(Function_4_Flag == 0)
		{
		LeftAngleCtr.AngleSet = 60;
		Function_4_Flag = 1;
		}

		if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse < 0))
		LeftAngleCtr.AngleSet = 32;//30;

		if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse < 0))
		LeftAngleCtr.AngleSet = 50;//45;

		if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse > 0))
		LeftAngleCtr.AngleSet = 70;//75;

		else if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse > 0))
		LeftAngleCtr.AngleSet = 88;//90;

		Left_AngleControl();
		Motor_Output_Pos(LeftAngleCtr.AngleCtrOut,0);
	}
	else
	{
		LeftSpeedCtr.SpeedCtrOut = 0;
		if((Encoder_PID.pulse < 190*4096/360)&&(Encoder_PID.pulse > 170*4096/360))	
		{
			Left_SpeedControl();
		}
		if((Encoder_PID.pulse < 210*4096/360)&&(Encoder_PID.pulse > 150*4096/360))	
		{
			LeftSpeedCtr.Kp = 2800;
			Encoder_PID.Kd  = 10000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else if((Encoder_PID.pulse < 230*4096/360)&&(Encoder_PID.pulse > 130*4096/360))	
		{
			LeftSpeedCtr.Kp = 5800;
			Encoder_PID.Kd  = 10000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else if((Encoder_PID.pulse < 250*4096/360)&&(Encoder_PID.pulse > 110*4096/360))	
		{
			LeftSpeedCtr.Kp = 8800;
			Encoder_PID.Kd  = 4000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else Motor_Output_Pos(0,0);
	}
}

void Function_5(void)
{
		if((Encoder_PID.pulse < 90*4096/360)||(Encoder_PID.pulse > 270*4096/360))
	{
		Encoder_PID.Delta_pulse = Encoder_PID.pulse - Encoder_PID.Pre_pulse;

		if(Function_4_Flag == 0)
		{
		LeftAngleCtr.AngleSet = 60;
		Function_4_Flag = 1;
		}

		if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse < 0))
		LeftAngleCtr.AngleSet = 32;//30;

		if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse < 0))
		LeftAngleCtr.AngleSet = 50;//45;

		if((Encoder_PID.pulse > 270*4096/360)&&(Encoder_PID.Delta_pulse > 0))
		LeftAngleCtr.AngleSet = 70;//75;

		else if((Encoder_PID.pulse < 90*4096/360)&&(Encoder_PID.Delta_pulse > 0))
		LeftAngleCtr.AngleSet = 88;//90;

		Left_AngleControl();
		Motor_Output_Pos(LeftAngleCtr.AngleCtrOut,0);
	}
	else
	{
		LeftSpeedCtr.SpeedCtrOut = 0;
		if((Encoder_PID.pulse < 190*4096/360)&&(Encoder_PID.pulse > 170*4096/360))	
		{
			Left_SpeedControl();
		}
		if((Encoder_PID.pulse < 210*4096/360)&&(Encoder_PID.pulse > 150*4096/360))	
		{
			LeftSpeedCtr.Kp = 2800;
			Encoder_PID.Kd  = 10000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else if((Encoder_PID.pulse < 230*4096/360)&&(Encoder_PID.pulse > 130*4096/360))	
		{
			LeftSpeedCtr.Kp = 5800;
			Encoder_PID.Kd  = 10000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else if((Encoder_PID.pulse < 250*4096/360)&&(Encoder_PID.pulse > 110*4096/360))	
		{
			LeftSpeedCtr.Kp = 8800;
			Encoder_PID.Kd  = 4000;
			ABS_EncoderPID(Encoder_PID.pulse);	
			Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
		}
		else Motor_Output_Pos(0,0);
	}
}
void Function_6(void)
{
	LeftSpeedCtr.SpeedCtrOut = 0;
	LeftAngleCtr.PreAngle = LeftAngleCtr.Angle;
	LeftAngleCtr.Angle = (float)LeftSpeedCtr.Cnt  * MOTOR_ANGLE_CONSTANT_LEFT + LeftAngleCtr.PreAngle;
	if(LeftAngleCtr.Angle >= 360)LeftSpeedCtr.SpeedSet = 0;
	else LeftSpeedCtr.SpeedSet = 0.5; 
	LeftSpeedCtr.Cnt = 0;
	if((Encoder_PID.pulse < 190*4096/360)&&(Encoder_PID.pulse > 170*4096/360))	
	{
		LeftSpeedCtr.Ki = 2000;
		LeftSpeedCtr.Kp = 800;
		
		Left_SpeedControl();
	}
	if((Encoder_PID.pulse < 210*4096/360)&&(Encoder_PID.pulse > 150*4096/360))	
	{
		LeftSpeedCtr.Kp = 2800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else if((Encoder_PID.pulse < 230*4096/360)&&(Encoder_PID.pulse > 130*4096/360))	
	{
		LeftSpeedCtr.Kp = 4800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else if((Encoder_PID.pulse < 250*4096/360)&&(Encoder_PID.pulse > 110*4096/360))	
	{
		LeftSpeedCtr.Kp = 6800;
		ABS_EncoderPID(Encoder_PID.pulse);	
		Motor_Output_Pos(-Encoder_PID.Motor_Voltage-LeftSpeedCtr.SpeedCtrOut,0);  //182.65
	}
	else Motor_Output_Pos(0,0);
}
void Invert_Pend_Task(void)
{
	(*Task_Invert_Pend_Carry[Key_Flag.Flag_1])();
}

