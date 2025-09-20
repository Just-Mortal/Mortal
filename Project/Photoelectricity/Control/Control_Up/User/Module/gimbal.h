#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "imu.h"
#include "motor.h"

// 8192 / 360 = 22.755

// -31.4			19.54
// 618				-558

#define GIMBAL_YAW_MIDDLE_ANGLE 	3325.f	// ��˳����
#define GIMBAL_PCH_MIDDLE_ANGLE 	0.349033833f	// ��������

#define GIMBAL_PCH_MEC_UP_ANGLE 	0.209870815f		// 最下
#define GIMBAL_PCH_MEC_DOWN_ANGLE -0.210350513f	// 最上
#define GIMBAL_PCH_IMU_UP_ANGLE 	17.1295662	// 最上
#define GIMBAL_PCH_IMU_DOWN_ANGLE -15.1924477	// 最下

#define GIMBAL_HEAD_SWAP_OK_ANGLE	1.2f
#define GIMBAL_HEAD_SWAP_TIME			800.f

#define GIMBAL_HEAD_SWAP_MEC_OK		0.004f

#define GIMBAL_INIT_RPOCESS_TIME 	500.f
#define GIMBAL_INIT_ANGLE_ERR 		0.008f


/*---------------------------- Private typedef begin-------------------------------*/
typedef enum
{
	GIM_INIT,
	GIM_SLEEP,
	
	GIM_IMU,
	GIM_MEC,

}Gimbal_Mode_t;

typedef struct 
{
	float yaw_mec_angle;
	float pch_mec_angle;
	
	float yaw_imu_angle;
	float pch_imu_angle;
	
}Gimbal_Target_t;

typedef struct
{
	float yaw_mec_angle; 		// -4096~4096 ˳ʱ��Ϊ��
	float pch_mec_angle; 		// -4096~4096 ����Ϊ��
	float yaw_mec_speed; 		// ˳ʱ��Ϊ��
	float pch_mec_speed; 		// ����Ϊ��
	
	float yaw_imu_angle; 		// -180~180 ˳ʱ��Ϊ��
	float pch_imu_angle; 		// -180~180 ����Ϊ��
	float yaw_imu_speed; 		// ˳ʱ��Ϊ��
	float pch_imu_speed; 		// ����Ϊ��
	
	float pch_imu_angle_pre;
	
}Gimbal_Measure_t;

typedef struct
{
	uint32_t init_cnt;
	
	uint32_t head_swap_cnt;
	
}Gimbal_Cnt_t;

typedef struct
{
	bool GIMBAL_INIT_OK_FLAG;
}Gimbal_Flag_t;

typedef struct
{
	Gimbal_Target_t* 	target;
	Gimbal_Measure_t* measure;
	
	float yaw_motor_set ;
	float pch_motor_set ;
	
}Gimbal_Info_t;

typedef struct
{
	motor_t*	yaw;
	motor_t* 	pch;
	
	imu_t*		imu;

}Gimbal_Dev_t;

typedef struct Gimbal_Struct_t
{
	Gimbal_Mode_t 		mode;
	
	Gimbal_Dev_t*			dev;
	Gimbal_Cnt_t* 		cnt;
	Gimbal_Info_t* 		info;
	Gimbal_Flag_t*		flag;
	
	void	(*init)(struct Gimbal_Struct_t* Gimbal);
	void	(*work)(struct Gimbal_Struct_t* Gimbal);
	void	(*heartbeat)(struct Gimbal_Struct_t* Gimbal);
	
}Gimbal_t;
/*---------------------------- Private typedef end-------------------------------*/

extern Gimbal_t gimbal;

#endif
