#ifndef __LAUNCH_H
#define __LAUNCH_H

#include "motor.h"

#include "stdbool.h"
#include "rp_math.h"

#include "user_config.h"

/*---------------宏定义begin---------------*/
#define LAUNCH_FRIC_SPEED_30 					7000.f	// 28.7 m/s			//	2024.07.31
#define LAUNCH_FRIC_SPEED_READY				1000.f

#define LAUNCH_FRIC_CHANGE_PID_ERR		2000.f

#define LAUNCH_DIAL_ANGLE_OK					100.f

// 425*36 = 15300
// 25Hz	=>	25/8*36*60	=	6750
// C610 闭环 425*36 = 15300
#define LAUNCH_DIAL_SPEED_SET					5940.f			// 22 Hz
#define LAUNCH_DIAL_SWIFT_SET					6750.f
//#define LAUNCH_DIAL_HIGH_HEAT_SET			1620.f		// 6 Hz

#define LAUNCH_DIAL_ANGLE_SET					36900.f
#define LAUNCH_DIAL_INIT_SPEED				100.f
#define LAUNCH_DIAL_INIT_ERR_TIME			50.f
#define LAUNCH_DIAL_INIT_TIME_MAX			5000.f
#define LAUNCH_DIAL_INIT_SPEED_ERR		10.f

#define LAUNCH_DIAL_STUCK_ANGLE				95.f			// 误差大于此角度判定为堵转
#define LAUNCH_DIAL_STUCK_SPEED				2000.f		// 误差大于此速度判定为堵转
#define LAUNCH_DIAL_STUCK_DEAL_CNT		400.f
#define LAUNCH_DIAL_STUCK_CHECK_CNT 	200.f
#define LAUNCH_DIAL_STUCK_DEAL_SPEED	-3000.f

#define LAUNCH_STABLE_CHECK_TIME			30.f		// 判定摩擦轮转速稳定的时间
/*--------------- 宏定义 end ---------------*/

/*--------------- 变量类型定义 begin ---------------*/
typedef struct
{
	bool FRIC_SPEED_READY_FLAG;
	
	bool DIAL_ANGLE_CHANGE_FLAG;
	bool DIAL_INIT_OK_FLAG;
	bool DIAL_STUCK_FLAG;
	
	bool HEAT_CTRL_FLAG;
	bool VIS_CTRL_FLAG;
	
	bool FRIC_CHECK_CNT_FLAG;			// 摩擦轮测量发弹量标志位
	
	bool TWIN_SHOOT_FLAG;					// 双发检测
	
}Launch_Flag_t;

typedef struct
{
	bool LAUNCH_SINGLE;
	bool LAUNCH_CONTINUOUS;
	bool LAUNCH_CLEAN_HEAT;
	
}Launch_Cmd_t;

typedef struct
{
	uint32_t dial_init_cnt ;
	
	uint32_t dial_stuck_deal_cnt;
	
	uint8_t vis_shoot_cnt;
//	uint32_t vision_shoot_cnt;
//	uint32_t last_vision_shoot_cnt;
	
	uint8_t	 fric_stable_cnt;					// 摩擦轮转速稳定时间
	uint16_t fric_measure_shoot_cnt;
	uint16_t dial_measure_shoot_cnt;
	uint16_t judge_measure_shoot_cnt;
	
	// 用于检测双发
	uint16_t single_cnt;
	uint16_t pre_single_cnt;

}Launch_Cnt_t;

typedef struct Launch_dev_struct_t
{

	motor_t* 	dial;
	motor_t* 	l_fc;
	motor_t* 	r_fc;
	
}Launch_Dev_t;

typedef struct Launch_info_struct_t
{
	float 	l_fc_speed_target;
	float 	r_fc_speed_target;
	float 	dial_speed_target;
	int32_t dial_angle_target;
	
	// test begin
	float 	l_fc_speed_measure;
	float 	r_fc_speed_measure;
	float		dial_speed_measure;
	float 	l_fc_speed_err;
	float 	r_fc_speed_err;
	// test end
	
	int16_t l_fc_motor_set ;	// CAN2	201
  int16_t r_fc_motor_set ;	//			202
  int16_t dial_motor_set ;	// 			203
	
	float    launch_limit_speed;
	float    launch_measure_speed;
	uint16_t launch_limit_heat;
	uint16_t launch_measure_heat;
	uint16_t launch_cooling_heat;
	int32_t	 launch_remain_heat;		// 有符号！！！
	
	uint16_t launch_swift_heat;
	uint16_t launch_swift_dail_angle;

	uint8_t	 swift_enable;

}Launch_Info_t;

typedef struct Launch_Config_struct_t
{
	float 		fric_speed_config;
	float 		dial_speed_config;
	
	motor_pid_set_t	fric_speed_pid;
	motor_pid_set_t	fric_swift_pid;
	
	motor_pid_set_t	dial_speed_pid;
	motor_pid_set_t	dial_swift_pid;
	
	uint16_t large_buff_cnt_max;
	uint16_t small_buff_cnt_max;
	
	float dial_high_heat_conf;

}Launch_Conf_t;

typedef enum 
{
	FRIC_SLEEP,
	
	FRIC_STOP,
	FRIC_SPIN,

}Fric_Mode_e;

typedef enum
{
	DIAL_SLEEP,
	DIAL_INIT,

	DIAL_WORK,
	
}Dial_Mode_e;

typedef struct Launch_struct_t
{
	Launch_Dev_t*		dev;
	
	Launch_Info_t*	info;
	Launch_Flag_t*	flag;
	Launch_Conf_t*	conf;
	Launch_Cmd_t*		cmd;
	Launch_Cnt_t*		cnt;
	
	Fric_Mode_e			fric_mode;
	Dial_Mode_e			dial_mode;
	 
	void	(*init)(struct Launch_struct_t* Launch);
	void	(*work)(struct Launch_struct_t* Launch);
	void	(*heartbeat)(struct Launch_struct_t* Launch);
	
}Launch_t;
/*---------------变量类型定义end---------------*/

extern Launch_t launch ;

#endif
