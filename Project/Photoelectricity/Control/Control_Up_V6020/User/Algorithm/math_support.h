/**
  ******************************************************************************
  * @file           : math_support.c\h
  * @brief          : 
  * @note           : 2022-3-3 15:29:25
	*                   增加str_to_num函数
  ******************************************************************************
  */
	
#ifndef __MATH_SUPPORT_H
#define __MATH_SUPPORT_H

#include "stm32f4xx_hal.h"

#define abs(x) ((x)>0? (x):(-(x)))
#define sgn(x) (((x)>0)?1:((x)<0?-1:0))

#ifndef PI
#define PI			3.14159265358979f
#endif

float Lowpass(float X_last, float X_new, float K);		//低通滤波

float int16_to_float(int16_t a, int16_t a_max, int16_t a_min, float b_max, float b_min);
int16_t float_to_int16(float b, float b_max, float b_min, int16_t a_max, int16_t a_min);

/* 数值函数 */
#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define abs(x) 					((x)>0? (x):(-(x)))
#define one(x)					((x)>0? (1):(-1))
#define square(x)	  		((x)*(x))

#endif
