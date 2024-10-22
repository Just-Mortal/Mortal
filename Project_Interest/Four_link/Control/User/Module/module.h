#ifndef __MODULE_H
#define __MODULE_H

#include "motor.h"
#include "rc_sensor.h"

#include "four_link.h"

#include "can_protocol.h"

typedef enum Module_Mode_Struct_t
{
	INIT,
	SLEEP,
	
	WORK,
	
}Module_Mode_e;

typedef struct Module_Flag_Struct_t
{
	bool REAL_INIT_OK;
	bool VIRT_INIT_OK;
	bool INIT_OK;
	
}Module_Flag_t;

typedef struct Module_Info_Struct_t
{
	int16_t	joint_virt_set;
	int16_t	joint_true_set;
	
	float real_i_measure;
	float virt_i_measure;
	
}Module_Info_t;

typedef struct Module_Struct_t
{
	Module_Mode_e mode;
	
	Module_Flag_t*	flag;
	Module_Info_t*	info;

}Module_t;

void Module_Ctrl(void);

#endif
