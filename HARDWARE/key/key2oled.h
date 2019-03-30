#ifndef __KEY2OLED_H
#define __KEY2OLED_H

#include "common.h"

typedef struct 
{
	char Flag_1;
	char Flag_2;
	char Flag_3;
	char Flag_A;
	
	char Flag_4;
	char Flag_5;
	char Flag_6;
	char Flag_B;
	
	char Flag_7;
	char Flag_8;
	char Flag_9;
	char Flag_C;
	
	char Flag_Star;
	char Flag_0;
	char Flag_Enter;
	char Flag_D;
}KEY_FLAG;

extern KEY_FLAG Key_Flag;

void Key_deal(void);
void A_Show(void);
void B_Show(void);
void C_Show(void);
void D_Show(void);
void E_Show(void);
void F_Show(void);
void G_Show(void);
void H_Show(void);
void I_Show(void);
void J_Show(void);
void K_Show(void);
void L_Show(void);
void M_Show(void);
void N_Show(void);
void O_Show(void);
void P_Show(void);



#endif

