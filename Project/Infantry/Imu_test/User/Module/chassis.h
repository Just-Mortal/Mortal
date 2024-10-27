#ifndef __CHASSIS_H
#define __CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "device.h"
#include "rp_config.h"

#include "math.h"
#include "rp_math.h"
#include "arm_math.h"

// 小陀螺转速与底盘功率关系式 (matlab拟合)
// power = 0.014726007725787 * conf  - 6.862580662068793
// conf	 = 67.90706745650285 * power + 466.0177279447975
// conf	 = 49.286743089516410* power + 1389.612380472540
//		功率				转速
//		50					3861
//		100					7257

/* Private macro -------------------------------------------------------------*/
#define CHASSIS_SPEED_MAX				8700.f	// 每个电机的最大转速
#define CHASSIS_TOP_SPEED 			8000.f	// 原地小陀螺到每个电机转速
#define CHASSIS_TUNNEL_LIMIT		3000.f	// 过隧道模式速度限制

/* Private typedef -----------------------------------------------------------*/
typedef enum {
	CHAS_LF,
	CHAS_RF,
	CHAS_LB,
	CHAS_RB,
	
	CHAS_MOTOR_CNT,
} Chassis_Motor_Cnt_t;

typedef enum
{
	CHAS_SLEEP,
	
	CHAS_NORMAL,
	
}Chassis_Mode_e;

typedef struct {
	motor_t					*chas_motor[CHAS_MOTOR_CNT];
} Chassis_Dev_t;

typedef struct {

	int16_t chas_measure_lf;
	int16_t chas_measure_rf;
	int16_t chas_measure_lb;
	int16_t chas_measure_rb;
	
	float front;
	float right;
	float cycle;
	float move;
	
	float test_current;
	
}	Chassis_Measure_t;

typedef struct {
	
	// 遥控解析速度
	double front_speed;
	double right_speed;
	double cycle_speed;
	
	double move_speed;

	// 电机目标速度
	int16_t chas_target_lf;
	int16_t chas_target_rf;
	int16_t chas_target_lb;
	int16_t chas_target_rb;

	// CAN发送电流
	int16_t chas_lf_set;
  int16_t chas_rf_set;
  int16_t chas_lb_set;
  int16_t chas_rb_set;
	
	Chassis_Measure_t	*measure;
	
} Chassis_Info_t ;

typedef struct 
{
	float chas_spin_conf;
	float chas_spin_max;	//	功率限制，超电电压，操作指令	限制最大转速
	
	float	 chas_speed_limit;
	
} Chassis_Conf_t;

typedef struct Chassis_Struct_t{
	Chassis_Mode_e	mode;
	
	Chassis_Dev_t		*dev;
	Chassis_Info_t	*info;
	Chassis_Conf_t	*conf;

	void	(*init)(struct Chassis_Struct_t* Chassis);
	void	(*work)(struct Chassis_Struct_t* Chassis);
	void 	(*heartbeat)(struct Chassis_Struct_t* Chassis);
}Chassis_t;

/* Exported variables --------------------------------------------------------*/
extern Chassis_t chassis;

#endif
