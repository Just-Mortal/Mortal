#ifndef __CHASSIS_H
#define __CHASSIS_H

#include "device.h"

//机器人速度限制
//#define VX_LIMIT  1.500f   //	X轴速度限值 m/s
//#define VY_LIMIT  1.200f   //	Y轴速度限值 m/s
//#define VW_LIMIT  6.280f   //	W旋转角速度限值 rad/s

typedef struct chassis_info_struct
{
	float front_speed;
	float right_speed;
	float cycle_speed;

	float target_speed[MOTOR_NUM];
//	float measure_speed[MOTOR_NUM];
	int16_t output[MOTOR_NUM];
}chassis_info_t;

typedef struct chassis_struct_t
{
	motor_t *motor[MOTOR_NUM];
	chassis_info_t info;

	void (*Init)(struct chassis_struct_t *chassis);
	void (*Work)(struct chassis_struct_t *chassis);
	void (*Update)(struct chassis_struct_t *chassis);
	void (*Sleep)(struct chassis_struct_t *chassis);
} Chassis_t;

static inline void Chassis_Init(Chassis_t *chassis);
static inline void Chassis_Work(Chassis_t *chassis);
static inline void Chassis_Update(Chassis_t *chassis);
static inline void Chassis_Sleep(Chassis_t *chassis);

extern Chassis_t chassis;

#endif
