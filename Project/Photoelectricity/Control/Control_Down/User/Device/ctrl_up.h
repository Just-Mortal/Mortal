#ifndef __CTRL_UP_H
#define __CTRL_UP_H

#include "can_drv.h"

// --------------- 底盘接收云台数据 --------------- //
typedef enum ctrl_status_enum
{
    CTRL_ONLINE = 0, // 云台在线
    CTRL_OFFLINE,    // 云台离线
    CTRL_ERROR,      // 云台错误
} ctrl_status_e;


// 遥控模式     巡逻模式
typedef enum
{
    REMOTE_CTRL_MODE = 0, // 遥控模式
    PATROL_CTRL_MODE,     // 巡逻模式

    CTRL_MODE_NUM         // 控制模式数量

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

// 底盘发送云台数据
typedef struct ctrl_down_info_struct
{
    ctrl_mode_e mode; // 控制模式

    // 遥控模式的数据
    int16_t yaw_speed;      // 云台偏航速度
    int16_t pitch_speed;    // 云台俯仰速度
} ctrl_down_info_t;

typedef struct ctrl_struct_t
{
    ctrl_status_e status;   // 云台状态

    uint16_t offline_cnt; // 离线计数
    uint16_t offline_cnt_max; // 最大离线计数

		float		x_pos;
		float		y_pos;
		float		a_yaw;

    ctrl_up_info_t up_info;         // 云台接收数据
    ctrl_down_info_t down_info;     // 底盘发送数据

    void (*Init)(struct ctrl_struct_t *ctrl); // 初始化函数
    void (*Work)(struct ctrl_struct_t *ctrl); // 工作函数
    void (*Heartbeat)(struct ctrl_struct_t *ctrl); // 心跳函数
    void (*Update)(struct ctrl_struct_t *ctrl, uint8_t *buff); // 更新函数
} Ctrl_t;

static inline void Ctrl_Init(Ctrl_t *ctrl);
static inline void Ctrl_Work(Ctrl_t *ctrl);
static inline void Ctrl_Heartbeat(Ctrl_t *ctrl);
static inline void Ctrl_Update(Ctrl_t *ctrl, uint8_t *buff);

extern Ctrl_t ctrl;

#endif // !__CTRL_UP_H
