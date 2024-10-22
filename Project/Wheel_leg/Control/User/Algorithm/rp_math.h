#ifndef __RP_MATH_H
#define __RP_MATH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "math.h"
#include "stdbool.h"

/* Exported macro ------------------------------------------------------------*/
#define PI 3.14159265358979f
/* Exported types ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/* 位操作函数 */
#define SET_EVENT(EVENT, FLAG)      ((EVENT) |= FLAG)     
#define CLEAR_EVENT(EVENT, FLAG)    ((EVENT) &= ~(FLAG))
#define GET_EVENT(EVENT, FLAG)      ((EVENT) & (FLAG))
/* 数值函数 */

#define sgn(x) (((x)>0.f)?1.f:((x)<0.f?-1.f:0.f))

#define constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define abs(x) 									((x)>0.f? (x):(-(x)))
#define one(x)									((x)>0.f? (1.f):(-1.f))
/* 斜坡函数 */
int16_t RampInt		(int16_t final, int16_t now, int16_t ramp);
float 	RampFloat	(float final, float now, float ramp);
/* 死区函数 */
float 	DeathZoom	(float input, float center, float death);

// 检测 nan
bool Judge_if_nan(float x);

#endif

