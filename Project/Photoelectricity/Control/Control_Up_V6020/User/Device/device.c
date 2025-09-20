#include "device.h"

float gimbal_yaw_imu_in_pid_param[7]		= {90.f,	1.f,		0.f,		0,		1000.f,		1000.f,	28000.f};
float gimbal_yaw_imu_pid_param[7]				= {22.f,	0.125f,	0.f,		0,			8.f,			1.f,	2000.f};

float gimbal_pch_imu_in_pid_param[7]	=	{25.f,	0.5f,		0.f,		0,		500.f,	250.f,		28000.f};
float gimbal_pch_imu_pid_param[7]			= {60.f,	0.5,	0.f,		0,			100.f,		50.0f,	2000.f};

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
	
	// OLED 初始化
	OLED_Init();    // 初始化OLED
	
	// ADC 采样初始化
	Battery_Init();
}
