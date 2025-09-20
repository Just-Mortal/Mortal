#include "device.h"

float gimbal_yaw_imu_in_pid_param[7]		= {90.f,	1.f,		0.f,		0,		1000.f,		1000.f,	28000.f};
float gimbal_yaw_imu_pid_param[7]				= {20.f,	0.125f,	0.f,		0,			8.f,			1.f,	2000.f};

float gimbal_pch_imu_in_pid_param[7]	=	{0.01f,	0.f,		0.f,		0,		0.f,	0.f,		6.5f};
float gimbal_pch_imu_pid_param[7]			= {20.f,	0.1,	0.f,		0,			100.f,		10.0f,	1000.f};

void Device_Init(void)
{
	vision.init(&vision);
	
	// Motor Init Begin
	motor[GIMBAL_YAW].init(&motor[GIMBAL_YAW]);
	motor[GIMBAL_PCH].init(&motor[GIMBAL_PCH]);
	// Motor Init End
	
	// Motor PID Init Begin
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.imu_angle_in,gimbal_yaw_imu_in_pid_param);
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.imu_angle,gimbal_yaw_imu_pid_param);
	motor[GIMBAL_PCH].pid_init(&motor[GIMBAL_PCH].pid.imu_angle_in,gimbal_pch_imu_in_pid_param);
	motor[GIMBAL_PCH].pid_init(&motor[GIMBAL_PCH].pid.imu_angle,gimbal_pch_imu_pid_param);
	// Motor PID Init End
	
	// 下主控初始化
	ctrl.Init(&ctrl);
}
