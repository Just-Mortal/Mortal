#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"

// 限幅函数
#define LIMIT(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

#define	 PI												3.14159265359f
#define  PID_RATE     						1000.0				//	PID频率	Hz
#define  MEC_WHEEL_BASE           0.19600	     	//	轮距，左右轮的距离	m
#define  MEC_ACLE_BASE            0.16973       //	轴距，前后轮的距离	m
#define  MEC_WHEEL_DIAMETER	      0.09400				//	轮子直径	m
#define  MEC_WHEEL_RESOLUTION     122880.0    	//	编码器分辨率(1024线),减速比30,1024x30x4=122880
#define  MEC_WHEEL_SCALE          (PI*MEC_WHEEL_DIAMETER*PID_RATE/MEC_WHEEL_RESOLUTION) //轮子速度m/s与编码器转换系数


typedef enum motor_enum
{
    MOTOR_LF = 0, // 左前
    MOTOR_RF,     // 右前
    MOTOR_LB,     // 左后
    MOTOR_RB,     // 右后
    MOTOR_NUM,
}Motor_e;

// -------- 结构体定义 begin -------- //
typedef struct 
{
	float   target;
	float   measure;
	float 	err;
	float 	last_err;
	float   integral;
	
	float 	pout;
	float 	iout;
	float 	dout;
    float   last_dout; // 上一次微分输出
	float 	out;

}motor_pid_info_t;

typedef struct 
{
	float   kp;
	float 	ki;
	float 	kd;

	float   derivative_filter; // 微分滤波
	float   integral_limit; // 积分限幅
	float   iout_max;
	float 	out_max;
	
}motor_pid_set_t;

typedef struct motor_struct
{
    TIM_HandleTypeDef *encoder_tim; // 编码器定时器句柄
    TIM_HandleTypeDef *pwm_tim;     // PWM定时器句柄
    uint32_t pwm_channel_a; // PWM通道A
    uint32_t pwm_channel_b; // PWM通道B

    float  encoder_speed;    // 编码速度
		float	 measure_speed;		 // 测量速度
    float  target_speed;     // 目标速度

    motor_pid_info_t pid_info; // PID信息
    motor_pid_set_t pid_set;   // PID设置

    void (*Init)(struct motor_struct *motor); // 初始化函数指针
    void (*UpdateSpeed)(struct motor_struct *motor); // 更新速度函数指针
    void (*PIDControl)(struct motor_struct *motor); // PID控制函数指针
    void (*SetSpeed)(struct motor_struct *motor, int16_t speed); // 设置速度函数指针
    void (*Sleep)(struct motor_struct *motor); // 休眠函数指针

} motor_t;

extern motor_t motor[MOTOR_NUM];

#endif
