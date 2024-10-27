#ifndef __INFANTRY_H
#define __INFANTRY_H

/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"
#include "power_limit.h"
#include "user_config.h"
#include "can_protocol.h"

// module 层
#include "chassis.h"
#include "gimbal.h"
#include "launch.h"

// device 层
#include "device.h"

/* Private macro -------------------------------------------------------------*/
#define CHASSIS_MOVE_SPEED 						8900.f		// 遥控键盘目标速度幅值缩放限制
#define CHASSIS_SPIN_SPEED 						3000.f		// 遥控缩放值，机械底盘旋转

#define CHASSIS_KEY_SPIN_SENSITIVITY 	3.f

#define GIMBAL_PCH_MEC_SENSITIVITY		1.f
#define GIMBAL_PCH_IMU_SENSITIVITY		0.05f			// 越小越慢
#define GIMBAL_YAW_IMU_SENSITIVITY		0.20f

#define GIMBAL_YAW_KEY_IMU_SENSITIVITY	1000.f	// 越大越慢
#define GIMBAL_PCH_KEY_IMU_SENSITIVITY	1000.f

#define GIMBAL_PCH_KEY_MEC_SENSITIVITY	500.f

#define VISION_TIME											5				// ms

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{
	RC_CTRL = 0,
	KEY_CTRL,
}Infantry_Ctrl_e;

typedef enum
{
	INIT_MODE = 0,
	SLEEP_MODE,
	
	MEC_MODE,
	IMU_MODE,
}Infantry_Mode_e;

typedef struct Infantry_flag_struct_t
{
	bool RC_CTRL_INIT_FLAG;
	bool KEY_CTRL_INIT_FLAG;
	bool INFANTRY_INIT_FLAG;
	
	bool CHAS_OPEN_LOCK_FLAG;
	
	bool GIM_BUFF_MODE_FLAG;
	
	bool LAUNCH_OPEN_LOCK_FLAG;
	bool LAUNCH_HIGH_HEAT_FLAG;
	
	bool VIS_CTRL_LAUNCH_FLAG;
	bool VIS_CTRL_GIMBAL_FLAG;
	bool VIS_BUFF_SHOOT_FLAG;
	bool VIS_BUFF_CUT_FLAG;
	
	bool TEST_VIS_CTRL_FLAG;
	
}Infantry_flag_t;

typedef struct Infantry_Command_struct_t
{
	// operator command
	bool CHAS_SPIN_TOP_CMD;
	
	bool GIM_AIM_ON_CMD;
	bool GIM_TURN_180_CMD;
	bool GIM_TRUN_LEFT_45_CMD;
	bool GIM_TURN_RIGHT_45_CMD;
	
	bool LNCH_CLEAN_HEAT_CMD;
	bool LNCH_NO_HEAT_LIM_CMD;
	
	// vision command
	bool VIS_OUTPOST_CMD;
	bool VIS_FAST_BUFF_CMD;
	bool VIS_LARGE_BUFF_CMD;
	bool VIS_SMALL_BUFF_CMD;
	bool VIS_CHANGE_TARGET_CMD;
	
	// cap command
	bool CAP_OPEN_CMD;
	
	// test command
	bool TEST_GIM_AIM_ON_CMD;

}Infantry_Cmd_t;

typedef struct Infantry_struct_t
{
	Infantry_Ctrl_e 	ctrl;
	Infantry_Mode_e 	mode;
	
	Infantry_Cmd_t* 	cmd;
	Infantry_flag_t*	flag;

	Chassis_t* 	chassis_module;
	Gimbal_t*		gimbal_module;
	Launch_t*		launch_module;

	rc_t*				rc_module;
	Judge_t*		judge_module;
	Slave_t*		slave_module;
	Vision_t*		vision_module;
	
	void				(*heartbeat)(struct Infantry_struct_t* Infantry);
	void 				(*update)(struct Infantry_struct_t* Infantry);
	void 				(*work)(struct Infantry_struct_t* Infantry);

}Infantry_t;

/* Exported variables --------------------------------------------------------*/
extern Infantry_t infantry;

// 操作指令
void Infantry_Status_Update(Infantry_t* Infantry);		// 整车状态更新，是否开控唤醒

#endif
