#include "device.h"

float motor_pid_param[4][7]	= {	{15.f, 0.0625,	10.f,		0,	10000.f,	1000.f,		9800.f},			//	real	speed
																{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},											//	real	angle
																{15.f, 0.0625,	10.f,		0,	10000.f,	1000.f,		9800.f},			//	virtual	speed
																{0.f,	0.f,	0.f,	0.f,	0.f,	0.f,	0.f},											//	virtual	angle
																																												};

void Device_Init(void)
{
		rc.init();

		for(motor_list_e i = JOINT_REAL; i < MOTOR_LIST; i++)
		{
			motor[i].init(&motor[i]);
			
			motor[i].pid_init(&motor[i].pid.speed,	motor_pid_param[2*i]);
			motor[i].pid_init(&motor[i].pid.angle,	motor_pid_param[2*i+1]);
		}
}
