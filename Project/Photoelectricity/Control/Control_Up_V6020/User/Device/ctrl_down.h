#ifndef __CTRL_DOWN_H
#define __CTRL_DOWN_H

#include "can_drv.h"

#include "math_support.h"

// --------------- 云台接收底盘数据 --------------- //
typedef enum ctrl_status_enum
{
    CTRL_ONLINE = 0,
    CTRL_OFFLINE,
    CTRL_ERROR,
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
	int16_t x_pos;		//	修正为坐标点
	int16_t y_pos;
	int16_t	yaw_a;
	
	struct __attribute__((packed)) bit_control_struct
	{
		uint16_t chassis_switch:	1;		//	底盘是否使能
		uint16_t winner_flag : 		1; 		//	蜂鸣器是否鸣响
		uint16_t chassis_begin : 	1;		//	底盘复位标志位
		uint16_t unused	:					13;
	}bit_control;
	
}ctrl_up_info_t;


// 底盘发送云台数据
typedef struct ctrl_down_info_struct
{
    ctrl_mode_e mode; // 控制模式

//    int16_t yaw_speed;      // 云台偏航速度
//    int16_t pitch_speed;    // 云台俯仰速度
} ctrl_down_info_t;

typedef struct ctrl_struct_t
{
    ctrl_status_e status;			// 主控状态

    uint16_t offline_cnt;			// 离线计数
    uint16_t offline_cnt_max;	// 最大离线计数

		float		x_pos;						//	目标点	x		0			-	4.f
		float		y_pos;						//	目标点	y		-4.f	-	0.f
		float		a_yaw;						//	目标yaw偏角

    ctrl_up_info_t		up_info;			// 云台接收数据
    ctrl_down_info_t	down_info;		// 底盘发送数据

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
