#ifndef __RP_MATH_H
#define __RP_MATH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "math.h"

#ifndef PI
#define PI 3.14159265358979f
#endif

#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define abs(x) 					((x)>0? (x):(-(x)))

float int16_to_float(int16_t a, int16_t a_max, int16_t a_min, float b_max, float b_min);
int16_t float_to_int16(float b, float b_max, float b_min, int16_t a_max, int16_t a_min);

#endif
