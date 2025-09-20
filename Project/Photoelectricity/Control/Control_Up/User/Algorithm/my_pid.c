/**
  ******************************************************************************
  * @file           : pid.c\h
  * @brief          : 
  * @note           : finish 2022-2-13 19:19:47
  ******************************************************************************
  */

#include "my_pid.h"
#include "math.h"
#include "math_support.h"



void pid_init(pid_info_t *pid)
{
	pid->target = 0;
	pid->measure = 0;
	pid->err = 0;
	pid->last_err = 0;
	pid->pout = 0;
	pid->iout = 0;
	pid->dout = 0;
	pid->out = 0;
	pid->integral = 0;
}

void single_pid_cal(pid_info_t *pid)
{
	pid->err = pid->target - pid->measure;
	if(abs(pid->err)<=(pid->blind_err))
		pid->err = 0;
	// ����
	pid->integral += pid->err;
	pid->integral = constrain(pid->integral, -pid->integral_max+pid->integral_bias, pid->integral_max+pid->integral_bias);
	// p i d ��������
	pid->pout = pid->kp * pid->err;
	pid->iout = pid->ki * pid->integral;
	pid->dout = pid->kd * (pid->err - pid->last_err);
	// �ۼ�pid���ֵ
	pid->out = pid->pout + pid->iout + pid->dout;
	pid->out = constrain(pid->out, -pid->out_max, pid->out_max);
	// ��¼�ϴ����ֵ
	pid->last_err = pid->err;
}


