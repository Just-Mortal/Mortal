#ifndef __JUDGE_H
#define __JUDGE_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "rp_config.h"

#include "judge_def.h"

/* Exported macro ------------------------------------------------------------*/
//#define GAME_STATUS_ID				0x001
#define GAME_ROBOT_HP_ID 			0x003
#define POWER_HEAT_ID 				0x101
#define GAME_ROBOT_STATUS_ID 	0x102
#define SHOOT_DATA_ID 				0x103
//#define ROBOT_HURT_ID 				0x107

#define SLAVE_INFO_ID					0xA5
//#define CHASSIS_POWER_CAN_ID	0x2E

/* Exported types ------------------------------------------------------------*/
#if 0
typedef struct 
{
	uint16_t chassis_power_buffer;
	uint16_t shooter_id1_17mm_cooling_heat;
	float chassis_power;
	
	uint16_t shooter_barrel_heat_limit;
	uint16_t chassis_power_limit;
	uint8_t my_color;
	uint16_t shooter_barrel_cooling_value;
	
	// ÖÐ¶Ï·¢ËÍ
	float bullet_speed;
	
}judge_rx_t;
#endif

typedef struct 
{
//	ext_rfid_status_t 			rfid_status;
//	ext_game_status_t 			game_status;
	ext_game_robot_status_t game_robot_status;
	ext_power_heat_data_t 	power_heat_data;
	ext_shoot_data_t 				shoot_data;
//	ext_robot_hurt_t 				robot_hurt;
	
	uint8_t		my_color;
	uint32_t	judge_measure_cnt;
	
	int16_t	 	offline_cnt;
	int16_t	 	offline_max_cnt;
	
}judge_info_t;

typedef struct judge_struct
{
	judge_info_t*			info;
	
	dev_work_state_t	work_state;
	dev_id_t			    id;
	
	void							(*update)(struct judge_struct *self, uint16_t canId, uint8_t *rxbuf);
	void				     	(*heartbeat)(struct judge_struct *self);

}Judge_t;

extern Judge_t judge;

#endif
