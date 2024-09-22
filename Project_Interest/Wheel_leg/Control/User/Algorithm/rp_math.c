/* Includes ------------------------------------------------------------------*/
#include "rp_math.h"

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
int16_t RampInt(int16_t final, int16_t now, int16_t ramp)
{
    int32_t buffer = 0;
    
    buffer = final - now;
    if (buffer > 0)
    {
        if (buffer > ramp)
            now += ramp;
        else
            now += buffer;
    }
    else
    {
        if (buffer < -ramp)
            now += -ramp;
        else
            now += buffer;
    }

    return now;
}

float RampFloat(float final, float now, float ramp)
{
    float buffer = 0;
    
    buffer = final - now;
    if (buffer > 0)
    {
        if (buffer > ramp)
            now += ramp;
        else
            now += buffer;
    }
    else
    {
        if (buffer < -ramp)
            now += -ramp;
        else
            now += buffer;
    }

    return now;    
}

float DeathZoom(float input, float center, float death)
{
    if(abs(input - center) < death)
        return center;
    return input;
}

bool Judge_if_nan(float x)
{
	bool res = false;
	res = (bool)__ARM_isnan((double)x);
	return res;
}
