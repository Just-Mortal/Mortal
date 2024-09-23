#ifndef __SLAVE_H
#define __SLAVE_H

#include "devices_config.h"
#include "can_drv.h"

typedef struct {

	union
	{
		uint16_t flag;
		
		struct slave_tx_info_t
		{
			uint16_t launch_state :	 		1;	// ����״̬		0����		1����
			uint16_t gimbal_state : 		1;	// ��̨״̬		0����		1����
			uint16_t vision_state :			1;	// �Ӿ�״̬		0����		1����
		
			uint16_t cap_switch:				1;	// ���翪��		0��			1��
			uint16_t spin_switch : 			1;	// С���ݿ��� 0�� 		1��
			uint16_t infantry_state :		1;	// ����ģʽ 	0������ 1��е
			uint16_t fric_ready :				1;	// Ħ���ֿ��� 0�� 		1��
		
			uint16_t outpost_mode :		  1;	// ����ǰ��		0��			1��
			uint16_t is_find_target :		1;	// ʶ��Ŀ�� 0�� 		1��
			uint16_t large_buff_mode :	1;	// ���ģʽ		0�� 		1��
			uint16_t small_buff_mode :	1;	// С��ģʽ		0�� 		1��
			uint16_t vis_ctrl_launch :	1;	// �Ӿ���		0��			1��
			uint16_t aim_on_switch :		1;	// ����ģʽ		0��			1��
	
		}	bit;
		
	}	flag_info;
	
	int16_t	yaw_angle;
	int16_t pitch_imu;
	int16_t	pitch_mec;

} Salve_Tx_Info_t;

typedef struct{
	int16_t	 offline_cnt;
	int16_t	 offline_max_cnt;
	
	Salve_Tx_Info_t tx_info;
	
} Slave_Info_t;

typedef struct Slave_Struct_t{
	Slave_Info_t*			info;
	dev_work_state_t	work_state;
	
	uint8_t tx_buf[8];
	
	void	(*send)(struct Slave_Struct_t *Slave);
	void	(*heartbeat)(struct Slave_Struct_t *Slave);
	
} Slave_t;

extern Slave_t slave;

#endif
