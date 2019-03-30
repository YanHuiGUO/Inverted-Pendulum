#ifndef _INVERT_PEND_H_
#define _INVERT_PEND_H_

#include "common.h"

#define ABS_Encoder_wholepulse   4096 //����ʽ��������Ȧ����
#define ABS_Encoder_pendpulse  2048 //����ʱ��Ӧ��������
typedef struct
{
		/*PID����*/
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
		/*��Ҫ�����ĺͲ�����ϵ��*/
    float     Angle_k;
    float     Angle_Delta_k;  //�������ǶȻ�����ϵ���ͽǶȲ��ϵ�����õ�����ָ����ļ��ٶȺͷ����������ļ��ٶ�
	  float     Motor_Voltage_k;	//���ٶȺ͵���ĵ�ѹֵ�ڼ����ڼ�����ȣ���Ϊϵ��
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

