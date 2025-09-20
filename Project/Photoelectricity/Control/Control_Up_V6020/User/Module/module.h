#ifndef __MODULE_H
#define __MODULE_H

#include "device.h"
#include "gimbal.h"

// ʵ��û�õ�
typedef enum {
    ROBOT_INIT,

    ROBOT_IMU,
    ROBOT_MEC,
    ROBOT_VISION,
    ROBOT_SLEEP,
} Robot_State;

// ״̬��
typedef enum
{
		GAME_INIT,			//	������ʼ��
    GAME_PREPARE,		//	�����ӵ���
		GAME_KEY_PRESS,	//	��������
		GAME_START,			//	������ʼ
		GAME_OVER_MID,	//	Խ�����ߣ�����Ѳ��
		GAME_FINAL,			//	��������
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
	
	// ��Ҫ�ı�ĽǶ�ֵ
	float yaw_imu_diff;
	float pch_imu_diff;
	
	// �������ݽӿ�
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
