#ifndef __PID_CAL_H
#define __PID_CAL_H

#include "my_math.h"

typedef struct 
{
	float	  target;
	float	  measure;
	float 	err;
	float 	last_err;
	float	  integral;
	float 	pout;
	float 	iout;
	float 	dout;
	float 	out;
	/* ≈‰÷√ */
	float   blind_err;
	float   integral_bias;
	float	  kp;
	float 	ki;
	float 	kd;
	float 	integral_max;
	float 	out_max;
}pid_info_t;


void pid_init(pid_info_t *pid);
void single_pid_cal(pid_info_t *pid);

#endif
