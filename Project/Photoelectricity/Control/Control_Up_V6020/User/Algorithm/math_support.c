/**
  ******************************************************************************
  * @file           : math_support.c\h
  * @brief          : 
  * @note           : 2022-3-3 15:27:58
  ******************************************************************************
  */
	
#include "math_support.h"
#include "math.h"



/**
  * @brief  µÍÍ¨ÂË²¨
  * @param  
  * @retval 
  */
float Lowpass(float X_last, float X_new, float K)
{
	return (X_last + (X_new - X_last) * K);
}

// ------------------------------------- //
float int16_to_float(int16_t a, int16_t a_max, int16_t a_min, float b_max, float b_min)
{
    int32_t a_32 = a, a_max_32 = a_max, a_min_32 = a_min;
    int32_t diff_a = a_max_32 - a_min_32;
    
    if (diff_a == 0) return (b_max + b_min) / 2.0f; // ´¦Àí³ıÁã
    
    float ratio = (float)(a_32 - a_min_32) / (float)diff_a;
    return ratio * (b_max - b_min) + b_min;
}

int16_t float_to_int16(float b, float b_max, float b_min, int16_t a_max, int16_t a_min)
{
    if (b_max == b_min) return (int16_t)((a_max + a_min) / 2);
    float ratio = (b - b_min) / (b_max - b_min);
    int32_t a = (int32_t)(ratio * (a_max - a_min) + a_min + 0.5f);
		
    a = (a < a_min) ? a_min : (a > a_max) ? a_max : a;
    
    return (int16_t)a;
}
