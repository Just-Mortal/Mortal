#ifndef __MODULE_H
#define __MODULE_H

#include "device.h"
#include "gimbal.h"

// 实际没用到
typedef enum {
    ROBOT_INIT,

    ROBOT_IMU,
    ROBOT_MEC,
    ROBOT_VISION,
    ROBOT_SLEEP,
} Robot_State;

// 状态机
typedef enum
{
		GAME_INIT,			//	比赛初始化
    GAME_PREPARE,		//	三分钟调试
		GAME_KEY_PRESS,	//	按键按下
		GAME_START,			//	比赛开始
		GAME_OVER_MID,	//	越过中线，定点巡逻
		GAME_FINAL,			//	比赛结束
} Robot_Stage;

typedef struct Robot_Info_Struct
{
	Robot_Stage stage;
	
	uint8_t key_state;
	uint8_t last_key_state;	
	
	uint8_t key_strategy;
	uint8_t last_key_strategy;	
	uint8_t strategy;
	
	uint8_t strategy_flag;
	uint8_t strategy_3_step;
	
	// 需要改变的角度值
	float yaw_imu_diff;
	float pch_imu_diff;
	
	// 底盘数据接口
	float front;
	float right;
	float cycle;
    
} Robot_Info_t;

typedef struct Robot_Struct {
    Gimbal_t* gimbal;
    Vision_t* vision;

    Robot_Info_t info;

    void (*init)(struct Robot_Struct* Robot);
    void (*state_update)(struct Robot_Struct* Robot);

} Robot_t;

extern Robot_t robot;


#endif
