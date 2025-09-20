#ifndef __CTRL_UP_H
#define __CTRL_UP_H

#include "can_drv.h"

// --------------- ���̽�����̨���� --------------- //
typedef enum ctrl_status_enum
{
    CTRL_ONLINE = 0, // ��̨����
    CTRL_OFFLINE,    // ��̨����
    CTRL_ERROR,      // ��̨����
} ctrl_status_e;


// ң��ģʽ     Ѳ��ģʽ
typedef enum
{
    REMOTE_CTRL_MODE = 0, // ң��ģʽ
    PATROL_CTRL_MODE,     // Ѳ��ģʽ

    CTRL_MODE_NUM         // ����ģʽ����

}   ctrl_mode_e;

typedef struct __attribute__((packed)) ctrl_up_info_struct
{
	int16_t front;
	int16_t right;
	int16_t	cycle;
	
	struct __attribute__((packed)) bit_control_struct
	{
		uint16_t chassis_switch:	1;
		uint16_t winner_flag : 		1; 
		uint16_t chassis_begin : 	1;
		uint16_t unused	:					13;
	}bit_control;
	
}ctrl_up_info_t;

// ���̷�����̨����
typedef struct ctrl_down_info_struct
{
    ctrl_mode_e mode; // ����ģʽ

    // ң��ģʽ������
    int16_t yaw_speed;      // ��̨ƫ���ٶ�
    int16_t pitch_speed;    // ��̨�����ٶ�
} ctrl_down_info_t;

typedef struct ctrl_struct_t
{
    ctrl_status_e status;   // ��̨״̬

    uint16_t offline_cnt; // ���߼���
    uint16_t offline_cnt_max; // ������߼���

		float		x_pos;
		float		y_pos;
		float		a_yaw;

    ctrl_up_info_t up_info;         // ��̨��������
    ctrl_down_info_t down_info;     // ���̷�������

    void (*Init)(struct ctrl_struct_t *ctrl); // ��ʼ������
    void (*Work)(struct ctrl_struct_t *ctrl); // ��������
    void (*Heartbeat)(struct ctrl_struct_t *ctrl); // ��������
    void (*Update)(struct ctrl_struct_t *ctrl, uint8_t *buff); // ���º���
} Ctrl_t;

static inline void Ctrl_Init(Ctrl_t *ctrl);
static inline void Ctrl_Work(Ctrl_t *ctrl);
static inline void Ctrl_Heartbeat(Ctrl_t *ctrl);
static inline void Ctrl_Update(Ctrl_t *ctrl, uint8_t *buff);

extern Ctrl_t ctrl;

#endif // !__CTRL_UP_H
