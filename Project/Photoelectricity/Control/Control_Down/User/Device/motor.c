#include "motor.h"

static void Motor_Init(motor_t *motor);
static void Motor_PIDControl(motor_t *motor);
static void Motor_SetSpeed(motor_t *motor, int16_t speed);
static void Motor_UpdateSpeed(motor_t *motor);
static void Motor_Sleep(motor_t *motor);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;
extern TIM_HandleTypeDef htim12;

float motor_pid_set[7] = {6.f, 0.5f, 0.f, 0.f, 2000.f, 1000.f, 4200.f}; // PID参数数组

motor_t motor[MOTOR_NUM] =
{
	[MOTOR_RB] = {
		.encoder_tim = &htim2,
		.pwm_tim = &htim1,
		.pwm_channel_a = TIM_CHANNEL_1,
		.pwm_channel_b = TIM_CHANNEL_2,

		.Init = Motor_Init,
	},
	[MOTOR_LB] = {
		.encoder_tim = &htim3,
		.pwm_tim = &htim1,
		.pwm_channel_a = TIM_CHANNEL_3,
		.pwm_channel_b = TIM_CHANNEL_4,

		.Init = Motor_Init,
	},

	[MOTOR_RF] = {
		.encoder_tim = &htim5,
		.pwm_tim = &htim9,
		.pwm_channel_a = TIM_CHANNEL_1,
		.pwm_channel_b = TIM_CHANNEL_2,

		.Init = Motor_Init,
	},

	[MOTOR_LF] = {
		.encoder_tim = &htim4,
		.pwm_tim = &htim12,
		.pwm_channel_a = TIM_CHANNEL_1,
		.pwm_channel_b = TIM_CHANNEL_2,

		.Init = Motor_Init,
	},

};

// --------- static function 定义 begin --------- //
void Morot_PIDInit(motor_t *motor, float *buff)
{
	motor->pid_info.target = 0.0f;
	motor->pid_info.measure = 0.0f;
	motor->pid_info.err = 0.0f;
	motor->pid_info.last_err = 0.0f;
	motor->pid_info.integral = 0.0f;

	motor->pid_set.kp = buff[0];
	motor->pid_set.ki = buff[1];
	motor->pid_set.kd = buff[2];
	motor->pid_set.derivative_filter = buff[3];
	motor->pid_set.integral_limit = buff[4];
	motor->pid_set.iout_max = buff[5];
	motor->pid_set.out_max = buff[6];
}

void Motor_Init(motor_t *motor)
{
	// 初始化函数指针
	motor->PIDControl = Motor_PIDControl;
	motor->SetSpeed = Motor_SetSpeed;
	motor->UpdateSpeed = Motor_UpdateSpeed;
	motor->Sleep = Motor_Sleep;

	// 启动编码器接口和PWM接口
	HAL_TIM_Encoder_Start(motor->encoder_tim, TIM_CHANNEL_ALL); 
	HAL_TIM_PWM_Start(motor->pwm_tim, motor->pwm_channel_a);
	HAL_TIM_PWM_Start(motor->pwm_tim, motor->pwm_channel_b);

	// 初始化PID参数
	Morot_PIDInit(motor, motor_pid_set);
}

void Motor_PIDControl(motor_t *motor)
{
//	motor->pid_info.measure = motor->measure_speed;
	motor->pid_info.measure = motor->encoder_speed;

	motor->pid_info.target	=	motor->target_speed;
	motor->pid_info.err = motor->pid_info.measure - motor->pid_info.target;
	motor->pid_info.integral += motor->pid_info.err;
	motor->pid_info.integral = LIMIT(motor->pid_info.integral, -motor->pid_set.integral_limit, motor->pid_set.integral_limit);

	motor->pid_info.pout = motor->pid_set.kp * motor->pid_info.err;
	motor->pid_info.iout = motor->pid_set.ki * motor->pid_info.integral;
	motor->pid_info.dout = motor->pid_set.kd * (motor->pid_info.err - motor->pid_info.last_err);
	motor->pid_info.dout = 	motor->pid_info.dout * (1.0f - motor->pid_set.derivative_filter) + \
													motor->pid_info.last_dout * motor->pid_set.derivative_filter;
	motor->pid_info.iout = LIMIT(motor->pid_info.iout, -motor->pid_set.iout_max, motor->pid_set.iout_max);
	motor->pid_info.out = motor->pid_info.pout + motor->pid_info.iout + motor->pid_info.dout;
	motor->pid_info.out = LIMIT(motor->pid_info.out, -motor->pid_set.out_max, motor->pid_set.out_max);

	motor->pid_info.last_err = motor->pid_info.err;
	motor->pid_info.last_dout = motor->pid_info.dout;
}

void Motor_UpdateSpeed(motor_t *motor)
{
	static int16_t last_count = 0;
	int16_t current_count = 0;

	last_count = current_count;
	current_count = __HAL_TIM_GET_COUNTER(motor->encoder_tim);
	motor->encoder_speed = (float)(current_count - last_count) ;
	__HAL_TIM_SetCounter(motor->encoder_tim, 0);
	
	motor->measure_speed = motor->encoder_speed*MEC_WHEEL_SCALE;
}

void Motor_SetSpeed(motor_t *motor, int16_t speed)
{
	speed = LIMIT(speed, -4200, 4200);

	if (speed > 0)
	{
		__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_a, 4200);  // 100%占空比
		__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_b, 4200 - speed);
	}
	else
	{
		__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_a, 4200 + speed);
		__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_b, 4200);  // 100%占空比
	}
}

void Motor_Sleep(motor_t *motor)
{
	__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_a, 0);  // 停止PWM输出
	__HAL_TIM_SET_COMPARE(motor->pwm_tim, motor->pwm_channel_b, 0);  // 停止PWM输出
}
