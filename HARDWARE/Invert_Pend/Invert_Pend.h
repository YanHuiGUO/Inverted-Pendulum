#ifndef _INVERT_PEND_H_
#define _INVERT_PEND_H_

#include "common.h"

#define ABS_Encoder_wholepulse   4096 //绝对式编码器整圈脉冲
#define ABS_Encoder_pendpulse  2048 //倒立时对应的脉冲数
typedef struct
{
		/*PID参数*/
    float     Kp;
    float     Ki;
    float     Kd;   
		/*********/
	  int pulse;
		int Pre_pulse;
		int D_Pre_pulse;
		int Delta_pulse;
		int Pre_Delta_pulse;
    float     Motor_Acce;
    float     Angle;
	  float     PreAngle;
		float			AngleSet;
    float     Angle_Delta;
		float     AngleCtrIntegral;
		float     Motor_Voltage;
		/*需要整定的和测量的系数*/
    float     Angle_k;
    float     Angle_Delta_k;  //编码器角度换算后的系数和角度差的系数来得到反向恢复力的加速度和反向阻尼力的加速度
	  float     Motor_Voltage_k;	//加速度和电机的电压值在加速期间成正比，此为系数
		float     Flag;
		float     Delta_Acce;
		/************************/
}Invert_Pend_PID;

extern Invert_Pend_PID  Encoder_PID;
void EncoderPID_Init(void);
void ABS_EncoderPID(u16 );
void Debug_PID(void);
void Goal_Encouder_Out(float );

void Function_None(void);
void Function_1(void);
void Function_2(void);
void Function_3(void);
void Function_4(void);
void Function_5(void);
void Function_6(void);
void Invert_Pend_Task(void);
#endif

