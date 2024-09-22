#ifndef __GIMBAL_H
#define __GIMBAL_H

#include "rp_math.h"

#include "imu.h"
#include "motor.h"

// 8192 / 360 = 22.755

// -31.4			19.54
// 618				-558

#define GIMBAL_YAW_MIDDLE_ANGLE 	3274.f	// 负顺正逆
#define GIMBAL_PCH_MIDDLE_ANGLE 	4722.f	// 负下正上

#define GIMBAL_PCH_MEC_UP_ANGLE 	550.f		// 最上
#define GIMBAL_PCH_MEC_DOWN_ANGLE -376.f	// 最下
#define GIMBAL_PCH_IMU_UP_ANGLE 	16.54f	// 最下
#define GIMBAL_PCH_IMU_DOWN_ANGLE -24.16f	// 最上

#define GIMBAL_HEAD_SWAP_OK_ANGLE	1.2f
#define GIMBAL_HEAD_SWAP_TIME			800.f

#define GIMBAL_HEAD_SWAP_MEC_OK		10.f

#define GIMBAL_INIT_RPOCESS_TIME 	500.f
#define GIMBAL_INIT_ANGLE_ERR 		20.f


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
	float yaw_mec_angle; 		// -4096~4096 顺时针为正
	float pch_mec_angle; 		// -4096~4096 向上为正
	float yaw_mec_speed; 		// 顺时针为正
	float pch_mec_speed; 		// 向上为正
	
	float yaw_imu_angle; 		// -180~180 顺时针为正
	float pch_imu_angle; 		// -180~180 向上为正
	float yaw_imu_speed; 		// 顺时针为正
	float pch_imu_speed; 		// 向上为正
	
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
	
	int16_t yaw_motor_set ;
	int16_t pch_motor_set ;
	
	float vis_shoot_yaw;		// 打符瞬间急停角度
	float vis_shoot_pch;
	
	float	pch_feedforward;

}Gimbal_Info_t;

typedef struct
{
	float yaw_mid_angle;
}Gimbal_Conf_t; 

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
	
	Gimbal_Conf_t*		conf;
	
	void	(*init)(struct Gimbal_Struct_t* Gimbal);
	void	(*work)(struct Gimbal_Struct_t* Gimbal);
	void	(*heartbeat)(struct Gimbal_Struct_t* Gimbal);
	
}Gimbal_t;
/*---------------------------- Private typedef end-------------------------------*/

extern Gimbal_t gimbal;

#endif
