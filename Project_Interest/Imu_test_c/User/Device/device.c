#include "device.h"

//float chassis_lf_speed_pid_param[7] 		= {15.f	,	0.5f,			0,		0,	20000.f,	10000.f,		16000.f};
//float chassis_rf_speed_pid_param[7] 		= {20.f	,	0.75f,		0,		0,	16000.f,	12000.f,		16000.f};
//float chassis_lb_speed_pid_param[7] 		= {20.f	,	0.75f,		0,		0,	16000.f,	12000.f,		16000.f};
//float chassis_rb_speed_pid_param[7]			= {15.f	,	0.5f,			0,		0,	20000.f,	10000.f,		16000.f};

float chassis_lf_speed_pid_param[7] 		= {12.f	,	0.75f,		1.f,		0,	8000.f,		6000.f,			12000.f};
float chassis_rf_speed_pid_param[7] 		= {12.f	,	0.75f,		1.f,		0,	8000.f,		6000.f,			12000.f};
float chassis_lb_speed_pid_param[7] 		= {12.f	,	0.75f,		1.f,		0,	8000.f,		6000.f,			12000.f};
float chassis_rb_speed_pid_param[7]			= {12.f	,	0.75f,		1.f,		0,	8000.f,		6000.f,			12000.f};


float gimbal_yaw_mec_in_pid_param[7]		= {520.f,		0.f,				0.f,	0,	0.f,		0.f,		28000.f};
float gimbal_yaw_mec_pid_param[7]				= {-0.75,	-0.015625f,	0,		0,	1000.f,	5.f,		2000.f};
float gimbal_pitch_mec_in_pid_param[7]	=	{320.f,		0.5,				0.f,	0,	5000.f,	2500.f,	28000.f};
float gimbal_pitch_mec_pid_param[7]			=	{-0.75f,	-0.125,			0,		0,	100.f,	0.f,		2000.f};

float gimbal_yaw_imu_in_pid_param[7]		= {520.f,	5.f,		0.f,		0,		100.f,		500.f,	28000.f};
float gimbal_yaw_imu_pid_param[7]				= {18.f,	0.125f,	0.f,		0,			8.f,			1.f,	2000.f};
float gimbal_pitch_imu_in_pid_param[7]	=	{320.f,	2.5f,		0.f,		0,	10000.f,	25000.f,	28000.f};
float gimbal_pitch_imu_pid_param[7]			= {18.f,	0.125f,	0.f,		0,			8.f,			1.f,	2000.f};

float launch_firc_speed_pid_param[7]  	= {40.f, 	1.f, 		0.f,		0, 	6000.f, 	6000.f, 	12000.f};

float launch_dial_speed_pid_param[7]		=	{15.f, 0.0625,	10.f,		0,	10000.f,	1000.f,		9800.f};
float launch_dial_angle_in_pid_param[7]	= {30.f,  2.f, 		0, 			0, 	2000.f, 	4000.f, 	9800.f};
float launch_dial_angle_pid_param[7]		=	{0.2f,	0, 		1.f, 			0, 		0.f, 				0.f, 	7020.f};	//	26

void Device_Init(void)
{
	rc.init();
	vision.init(&vision);
	
	// Motor Init Begin
	motor[CHASSIS_LF].init(&motor[CHASSIS_LF]);
	motor[CHASSIS_RF].init(&motor[CHASSIS_RF]);
	motor[CHASSIS_LB].init(&motor[CHASSIS_LB]);
	motor[CHASSIS_RB].init(&motor[CHASSIS_RB]);
	motor[GIMBAL_YAW].init(&motor[GIMBAL_YAW]);
	motor[GIMBAL_PITCH].init(&motor[GIMBAL_PITCH]);
	motor[LAUNCH_FRIC_R].init(&motor[LAUNCH_FRIC_R]);
	motor[LAUNCH_FRIC_L].init(&motor[LAUNCH_FRIC_L]);
	motor[LAUNCH_DIAL].init(&motor[LAUNCH_DIAL]);
	// Motor Init End
	
	// Motor PID Init Begin
	motor[CHASSIS_LF].pid_init(&motor[CHASSIS_LF].pid.speed, chassis_lf_speed_pid_param);
	motor[CHASSIS_RF].pid_init(&motor[CHASSIS_RF].pid.speed, chassis_rf_speed_pid_param);
	motor[CHASSIS_LB].pid_init(&motor[CHASSIS_LB].pid.speed, chassis_lb_speed_pid_param);
	motor[CHASSIS_RB].pid_init(&motor[CHASSIS_RB].pid.speed, chassis_rb_speed_pid_param);
	
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.mec_angle_in,gimbal_yaw_mec_in_pid_param);
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.mec_angle,gimbal_yaw_mec_pid_param);
	motor[GIMBAL_PITCH].pid_init(&motor[GIMBAL_PITCH].pid.mec_angle_in,gimbal_pitch_mec_in_pid_param);
	motor[GIMBAL_PITCH].pid_init(&motor[GIMBAL_PITCH].pid.mec_angle,gimbal_pitch_mec_pid_param);
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.imu_angle_in,gimbal_yaw_imu_in_pid_param);
	motor[GIMBAL_YAW].pid_init(&motor[GIMBAL_YAW].pid.imu_angle,gimbal_yaw_imu_pid_param);
	motor[GIMBAL_PITCH].pid_init(&motor[GIMBAL_PITCH].pid.imu_angle_in,gimbal_pitch_imu_in_pid_param);
	motor[GIMBAL_PITCH].pid_init(&motor[GIMBAL_PITCH].pid.imu_angle,gimbal_pitch_imu_pid_param);
	motor[LAUNCH_FRIC_R].pid_init(&motor[LAUNCH_FRIC_R].pid.speed,launch_firc_speed_pid_param);
	motor[LAUNCH_FRIC_L].pid_init(&motor[LAUNCH_FRIC_L].pid.speed,launch_firc_speed_pid_param);
	motor[LAUNCH_DIAL].pid_init(&motor[LAUNCH_DIAL].pid.speed,launch_dial_speed_pid_param);
	motor[LAUNCH_DIAL].pid_init(&motor[LAUNCH_DIAL].pid.position_in,launch_dial_angle_in_pid_param);
	motor[LAUNCH_DIAL].pid_init(&motor[LAUNCH_DIAL].pid.position,launch_dial_angle_pid_param);
	// Motor PID Init End
}
