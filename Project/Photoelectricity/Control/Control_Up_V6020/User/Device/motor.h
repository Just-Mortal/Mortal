#ifndef __MOTOR_H
#define __MOTOR_H

#include "driver.h"
#include "motor_def.h"

#define m_abs(x) 											((x)>0? (x):(-(x)))
#define m_constrain(x, min, max)			((x>max)?max:(x<min?min:x))
#define m_anti_constrain(x, min, max)	(((x<max) && (x>min))?(0):x)

// CAN 1
#define GIM_YAW	0x205
#define GIM_PCH	0x206

typedef enum
{
	GIMBAL_YAW,
	GIMBAL_PCH,

	MOTOR_LIST,
	
}motor_list_e;

typedef struct motor_class_t
{
	motor_rx_info_t    	rx_info;		// 电机数据+角度相关计算
	motor_state_info_t	state;			// 包括电机的ID和状态信息
	motor_id_info_t    	id;					// 包括电机接发ID和位置，通信方式和电机型号
  motor_mec_info_t		mec_info;
	motor_pid_all_t    	pid;
	
	float				 motor_set;
	
	void (*init)(struct motor_class_t *motor);
	void (*heartbeat)(struct motor_class_t *motor);
  void (*pid_init)(motor_pid_t *pid, float *buff);	
	
	uint8_t (*rx)(struct motor_class_t *motor, uint8_t *buff,uint8_t len);

	float (*c_posit)(struct motor_class_t *motor,float target);
	float (*c_angle)(struct motor_class_t *motor,float target);	
	float (*c_step)(struct motor_class_t *motor,float target,char step);	
	float (*c_speed)(struct motor_class_t *motor,float target);	
	
	float (*c_pid2)(motor_pid_t *out, motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode);	
	float (*c_pid1)(motor_pid_t *out, float meas1, float tar);	
	
	void  (*c_judge_dir)(struct motor_class_t *motor,uint16_t range);	
  void  (*c_offset)(struct motor_class_t *motor, uint16_t range);	
	
  uint8_t (*c_stuck_flag)(struct motor_class_t *motor, uint8_t pid_type, float err_limit, uint16_t lock_cnt_max);
	uint8_t (*c_idling_flag)(struct motor_class_t *motor, uint16_t speed_limit, int16_t i_min, uint8_t t_limit);
	
}	motor_t;

void motor_class_init(struct motor_class_t *motor);
void motor_class_pid_init(struct motor_pid *pid, float *buff);
void motor_class_heartbeat(struct motor_class_t *motor);
uint8_t motor_class_stucking_flag(struct motor_class_t *motor, uint8_t pid_type, float err_limit, uint16_t lock_cnt_max);
uint8_t motor_class_idling_flag(struct motor_class_t *motor, uint16_t speed_limit, int16_t i_min, uint8_t t_limit);
void motor_offset(struct motor_class_t *motor, uint16_t range);
void motor_judge_dir(struct motor_class_t *motor,uint16_t range);

static void get_rm_info(struct motor_class_t *motor, uint8_t *rxBuf);
uint8_t can_rx_buff(struct motor_class_t *motor, uint8_t *buff,uint8_t len);

float motor_pid_double(motor_pid_t *out, motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode);
float motor_pid_single(motor_pid_t *out, float meas1, float tar);
float motor_pid_position(struct motor_class_t *motor,float target);
float motor_pid_angle(struct motor_class_t *motor,float target);
float motor_pid_speed(struct motor_class_t *motor,float target);

extern motor_t motor[MOTOR_LIST];

#endif
