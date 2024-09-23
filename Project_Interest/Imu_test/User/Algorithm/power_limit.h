#ifndef __POWER_LIMIT_H
#define __POWER_LIMIT_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"
#include "arm_math.h"
#include "rp_math.h"

#include "judge.h"
#include "chassis.h"

/* Private macro -------------------------------------------------------------*/
#define CHAS_SP_MAX_OUT 12000.f

/* Private typedef -----------------------------------------------------------*/
typedef struct Power_Limit_Struct_t{
	
	float power[CHAS_MOTOR_CNT];	//	用于验证模型正确性，仅测试用
	float power_last;							//	用于验证模型正确性，仅测试用
	float power_now;							//	用于验证模型正确性，仅测试用
	
	// 用于 matlab 拟合数据
	double matlab_x2;				//	k1
	double matlab_y2;				//	k2
	double matlab_z2;				//	k3
	double matlab_p2;				//	p_measure
	
	// 拟合结果
	double k1[CHAS_MOTOR_CNT];
	double k2[CHAS_MOTOR_CNT];
	double k3[CHAS_MOTOR_CNT];
	double p0[CHAS_MOTOR_CNT];
	
} Power_Limit_t ;



/* Exported functions --------------------------------------------------------*/
void Chassis_Power_Limit_Ctrl(struct Power_Limit_Struct_t* Power_Limit);
void Chassis_Power_Limit_Update(struct Power_Limit_Struct_t* Power_Limit);
void Chassis_Power_Limit_Verify(struct Power_Limit_Struct_t* Power_Limit);
float cal_new_current(float w, float p, float k1, float k2, float k3, float p0, float i);
float cal_power(Power_Limit_t* Power_Limit, Chassis_Motor_Cnt_t num);

void Chassis_Motor_Power_Limit(int16_t* lf, int16_t* rf, int16_t* lb, int16_t* rb,float limit);

extern Power_Limit_t	power_limit;

#endif
