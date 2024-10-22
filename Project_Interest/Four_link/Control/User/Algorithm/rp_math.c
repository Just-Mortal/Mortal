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

// pid ¼ÆËãº¯Êý
void pid_init(pid_t *pid)
{
  pid->target = 0;
  pid->measure = 0;
  pid->err = 0;
  pid->last_err = 0;
  pid->integral = 0;
  pid->pout = 0;
  pid->iout = 0;
  pid->dout = 0;
  pid->last_dout = 0;
  pid->out = 0;

  pid->blind_err = 0;
  pid->integral_bias = 0;
  pid->kp = 0;
  pid->ki = 0;
  pid->kd = 0;
  pid->d_lpf = 0;
  pid->integral_max = 0;
  pid->out_max = 0;
}

void single_pid_cal(pid_t *pid)
{
  pid->err = pid->target - pid->measure;

  if (abs(pid->err) < pid->blind_err)
    pid->err = 0;

  pid->integral += pid->err;
  pid->integral = constrain(pid->integral, -pid->integral_max, pid->integral_max);
  pid->pout = pid->kp * pid->err;
  pid->iout = pid->ki * pid->integral;
  pid->dout = pid->kd * (pid->err - pid->last_err);
  pid->dout = pid->dout * pid->d_lpf + pid->last_dout * (1 - pid->d_lpf);
  pid->out = pid->pout + pid->iout + pid->dout;

  pid->out = constrain(pid->out, -pid->out_max, pid->out_max);

  pid->last_err = pid->err;
  pid->last_dout = pid->dout;
}

