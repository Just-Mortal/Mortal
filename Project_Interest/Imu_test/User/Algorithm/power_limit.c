/* Includes ------------------------------------------------------------------*/
#include "power_limit.h"

// Infantry->cmd->CAP_OPEN_CMD		操作手指令给下主控
// true		超电 30w放电			false 超电放电200w
// CAP_STATE											超电正常工作标志位	下主控发送

extern float cap_u;								// 超电电压
extern bool CAP_STATE, Cap_Switch;						// 超电正常工作标志位	下主控发送
extern uint8_t chassis_acc_flag;	// 判断底盘是否处在加速状态

// 静态功率			5.558902108528935 W

// 无超电功率		2.785736035831687 W
// 平均静态功率	0.696434008957922 W

Power_Limit_t power_limit = 
{
	// 左前轮 3508，实时更新采集数据，拟合效果		R^2 = 91.9882451532339%
	.k1[CHAS_LF] = 2.040347374398268e-06,
	.k2[CHAS_LF] = 1.979921074981745e-07,
	.k3[CHAS_LF] = 1.797481589393829e-07,
	.p0[CHAS_LF] = 0.166886875866940,
	
	// 右前轮 3508，实时更新采集数据，拟合效果		R^2 = 91.7532988744672%
	.k1[CHAS_RF] = 2.432618786758946e-06,			//	0.3
	.k2[CHAS_RF] = 2.004666363000520e-07,
	.k3[CHAS_RF] = 1.882338847220734e-07,
	.p0[CHAS_RF] = 1.451416694290960,
	
	// 左后轮 3508，实时更新采集数据，拟合效果		R^2 = 96.5597738711979%
	.k1[CHAS_LB] = 2.432618786758946e-06,			//	0.3
	.k2[CHAS_LB] = 2.029348269259410e-07,
	.k3[CHAS_LB] = 1.760120921831283e-07,
	.p0[CHAS_LB] = 0.825830195230425,
	
	// 右后轮 3508，实时更新采集数据，拟合效果		R^2 = 93.7478024517859%
	.k1[CHAS_RB] = 2.432618786758946e-06,			//	0.3
	.k2[CHAS_RB] = 1.931883408893043e-07,
	.k3[CHAS_RB] = 2.302214842198609e-07,
	.p0[CHAS_RB] = 1.452853113704662,
};

/* Exported functions --------------------------------------------------------*/
// 此函数基于西交利物浦功率限制模型，用于采集数据拟合系数
// 电机参数不一，通过更改接线和 chassis_motor_num 采集数据
extern bool CHASSIS_POWER_UPDATE_FLAG;
Chassis_Motor_Cnt_t chassis_motor_num = CHAS_RB;
float matlab_x, matlab_y, matlab_z, matlab_p;
void Chassis_Power_Limit_Update(struct Power_Limit_Struct_t* Power_Limit)
{
		switch(chassis_motor_num)
		{
			case CHAS_LF:
				matlab_x =	chassis.dev->chas_motor[CHAS_LF]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_LF]->rx_info.current ;
				matlab_y =	chassis.dev->chas_motor[CHAS_LF]->rx_info.current * \
										chassis.dev->chas_motor[CHAS_LF]->rx_info.current ;
				matlab_z =	chassis.dev->chas_motor[CHAS_LF]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_LF]->rx_info.speed ;
				break;
			
			case CHAS_RF:
				matlab_x =	chassis.dev->chas_motor[CHAS_RF]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_RF]->rx_info.current ;
				matlab_y =	chassis.dev->chas_motor[CHAS_RF]->rx_info.current * \
										chassis.dev->chas_motor[CHAS_RF]->rx_info.current ;
				matlab_z =	chassis.dev->chas_motor[CHAS_RF]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_RF]->rx_info.speed ;
				break;
			
			case CHAS_LB:
				matlab_x =	chassis.dev->chas_motor[CHAS_LB]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_LB]->rx_info.current ;
				matlab_y =	chassis.dev->chas_motor[CHAS_LB]->rx_info.current * \
										chassis.dev->chas_motor[CHAS_LB]->rx_info.current ;
				matlab_z =	chassis.dev->chas_motor[CHAS_LB]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_LB]->rx_info.speed ;
				break;
				
			case CHAS_RB:
				matlab_x =	chassis.dev->chas_motor[CHAS_RB]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_RB]->rx_info.current ;
				matlab_y =	chassis.dev->chas_motor[CHAS_RB]->rx_info.current * \
										chassis.dev->chas_motor[CHAS_RB]->rx_info.current ;
				matlab_z =	chassis.dev->chas_motor[CHAS_RB]->rx_info.speed * \
										chassis.dev->chas_motor[CHAS_RB]->rx_info.speed ;
				break;
				
			default:
				break;
		}
	
	matlab_p = judge.info->power_heat_data.chassis_power;
	
	// 电流范围是 -16384~+16384		->		-20A~+20A
	// 转速单位是 rpm							->		rad / s
	// k1		*=	16384/20*268/17*60/2/pi	=	123323.885	0.3		N·m/A		0.000002433
	// k2		*=	16384/20*16384/20				=	671,088.64	0.194 Ω				0.000000289
	// k3		*=	14*60/2/pi*14*60/2/pi		=	17873.0568	0.097 mH			
}


// 用于验证模型正确性，仅测试用
void Chassis_Power_Limit_Verify(struct Power_Limit_Struct_t* Power_Limit)
{
	Power_Limit->power[CHAS_LF] = \
	Power_Limit->k1[CHAS_LF] * chassis.dev->chas_motor[CHAS_LF]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_LF]->rx_info.current	+
	Power_Limit->k2[CHAS_LF] * chassis.dev->chas_motor[CHAS_LF]->rx_info.current * \
	chassis.dev->chas_motor[CHAS_LF]->rx_info.current	+
	Power_Limit->k3[CHAS_LF] * chassis.dev->chas_motor[CHAS_LF]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_LF]->rx_info.speed	+
	Power_Limit->p0[CHAS_LF] ;

	Power_Limit->power[CHAS_LB] = \
	Power_Limit->k1[CHAS_LB] * chassis.dev->chas_motor[CHAS_LB]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_LB]->rx_info.current	+
	Power_Limit->k2[CHAS_LB] * chassis.dev->chas_motor[CHAS_LB]->rx_info.current * \
	chassis.dev->chas_motor[CHAS_LB]->rx_info.current	+
	Power_Limit->k3[CHAS_LB] * chassis.dev->chas_motor[CHAS_LB]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_LB]->rx_info.speed	+
	Power_Limit->p0[CHAS_LB] ;
	
	Power_Limit->power[CHAS_RF] = \
	Power_Limit->k1[CHAS_RF] * chassis.dev->chas_motor[CHAS_RF]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_RF]->rx_info.current	+
	Power_Limit->k2[CHAS_RF] * chassis.dev->chas_motor[CHAS_RF]->rx_info.current * \
	chassis.dev->chas_motor[CHAS_RF]->rx_info.current	+
	Power_Limit->k3[CHAS_RF] * chassis.dev->chas_motor[CHAS_RF]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_RF]->rx_info.speed	+
	Power_Limit->p0[CHAS_RF] ;
	
	Power_Limit->power[CHAS_RB] = \
	Power_Limit->k1[CHAS_RB] * chassis.dev->chas_motor[CHAS_RB]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_RB]->rx_info.current	+
	Power_Limit->k2[CHAS_RB] * chassis.dev->chas_motor[CHAS_RB]->rx_info.current * \
	chassis.dev->chas_motor[CHAS_RB]->rx_info.current	+
	Power_Limit->k3[CHAS_RB] * chassis.dev->chas_motor[CHAS_RB]->rx_info.speed * \
	chassis.dev->chas_motor[CHAS_RB]->rx_info.speed	+
	Power_Limit->p0[CHAS_RB] ;
	
	// 低通滤波，截止频率与裁判系统功率读取频率一致
	Power_Limit->power_last = Power_Limit->power_now;
	Power_Limit->power_now = Power_Limit->power[CHAS_LF] + Power_Limit->power[CHAS_LB] +
														Power_Limit->power[CHAS_RF] + Power_Limit->power[CHAS_RB];
	Power_Limit->power_now = 0.9f * Power_Limit->power_last + 0.1f * Power_Limit->power_now;
}

float chas_power_max, k_change;
float new_set_lf, new_set_rf, new_set_lb, new_set_rb, k_change_last, judge_power_max;
void Chassis_Power_Limit_Ctrl(struct Power_Limit_Struct_t* Power_Limit)
{
	float power_lf, power_rf, power_lb, power_rb;
	float p_pre_lf, p_pre_rf, p_pre_lb, p_pre_rb;
	float p_lim_lf, p_lim_rf, p_lim_lb, p_lim_rb;
	float predict_p;
	
	// 系数用于补偿模型稳态误差
	judge_power_max = (judge.info->game_robot_status.chassis_power_limit) * 0.95f;
	
	if(CAP_STATE == 1 && Cap_Switch ==	true)			// 超电正常工作
	{
		if(cap_u > 15.0f )			// 大于 40 %
			chas_power_max =  judge_power_max + 190.f;		//	240 - 290
		else if(cap_u > 10.f)	// 大于 5.5 %							// 要 > 8V
			chas_power_max =  judge_power_max + 38.f * (cap_u - 10.f);		// 此系数需再考虑
		else
			chas_power_max =  judge_power_max;
	}
	else
		chas_power_max = judge_power_max ;
	
	// -------------------- model fit begin -------------------- //
	power_lf = cal_power(Power_Limit, CHAS_LF);
	power_rf = cal_power(Power_Limit, CHAS_RF);
	power_lb = cal_power(Power_Limit, CHAS_LB);
	power_rb = cal_power(Power_Limit, CHAS_RB);
	
	// 低通滤波，注意截止频率
	power_lf = 0.6f * p_pre_lf + 0.4f * power_lf;
	power_rf = 0.6f * p_pre_rf + 0.4f * power_rf;
	power_lb = 0.6f * p_pre_lb + 0.4f * power_lb;
	power_rb = 0.6f * p_pre_rb + 0.4f * power_rb;
	
	predict_p = power_lf + power_rf + power_lb + power_rb;
	// -------------------- model fit end   -------------------- //
	
	if(predict_p < chas_power_max)
	{
		k_change_last = k_change;
		k_change = 1.f;
	}
	else
	{
		k_change_last = k_change;
		k_change =  chas_power_max / predict_p;
		
		if(k_change > 0.f)
		{
			// 注意截止频率
			k_change = 0.8f * k_change_last + 0.2f * k_change;
			
			// 更新
			if(power_lf > 0.f)
			{
				p_lim_lf	= power_lf * k_change;
			
				new_set_lf	= cal_new_current(chassis.dev->chas_motor[CHAS_LF]->rx_info.speed,\
											p_lim_lf, Power_Limit->k1[CHAS_LF], Power_Limit->k2[CHAS_LF], \
											Power_Limit->k3[CHAS_LF], Power_Limit->p0[CHAS_LF], chassis.info->chas_lf_set);
																		
				chassis.info->chas_lf_set = new_set_lf;
			}
			
			if(power_rf > 0.f)
			{
				p_lim_rf	= power_rf * k_change;
			
				new_set_rf	= cal_new_current(chassis.dev->chas_motor[CHAS_RF]->rx_info.speed,\
											p_lim_rf, Power_Limit->k1[CHAS_RF], Power_Limit->k2[CHAS_RF], \
											Power_Limit->k3[CHAS_RF], Power_Limit->p0[CHAS_RF], chassis.info->chas_rf_set);
																		
				chassis.info->chas_rf_set = new_set_rf;
			}
			
			if(power_lb > 0.f)
			{
				p_lim_lb	= power_lb * k_change;
			
				new_set_lb	= cal_new_current(chassis.dev->chas_motor[CHAS_LB]->rx_info.speed,\
											p_lim_lb, Power_Limit->k1[CHAS_LB], Power_Limit->k2[CHAS_LB], \
											Power_Limit->k3[CHAS_LB], Power_Limit->p0[CHAS_LB], chassis.info->chas_lb_set);
																		
				chassis.info->chas_lb_set = new_set_lb;
			}
			
			if(power_rb > 0.f)
			{
				p_lim_rb	= power_rb * k_change;
			
				new_set_rb	= cal_new_current(chassis.dev->chas_motor[CHAS_RB]->rx_info.speed,\
											p_lim_rb, Power_Limit->k1[CHAS_RB], Power_Limit->k2[CHAS_RB], \
											Power_Limit->k3[CHAS_RB], Power_Limit->p0[CHAS_RB], chassis.info->chas_rb_set);
																		
				chassis.info->chas_rb_set = new_set_rb;
			}
		}
		else
		{
			k_change	= 0.f;
			
			new_set_lf = 0.f;
			new_set_rf = 0.f;
			new_set_lb = 0.f;
			new_set_rb = 0.f;
			
			chassis.info->chas_lf_set = new_set_lf;
			chassis.info->chas_rf_set = new_set_rf;
			chassis.info->chas_lb_set = new_set_lb;
			chassis.info->chas_rb_set = new_set_rb;
		}
	}

}

// w 当前转速		// i 即将发出的目标电流		// p 期望功率
float a, b, c, d;
float cal_new_current(float w, float p, float k1, float k2, float k3, float p0, float i)
{
	float i_new;
	
	a = k2;
	b = w*k1;
	c = k3*pow(w,2)+p0-p;
	d = pow(b,2)-4*k2*c;
	
	if(d >= 0)
	{
		d = sqrt(d);
		
		if(i >= 0)
			i_new = (-b+d)	/	(2.f*a);
		else
			i_new = (-b-d)	/	(2.f*a);
	}
	else
		i_new = i;

//	i_new = constrain(i_new, -16000.f, +16000.f);
	if(i_new > 12000.f)
		i_new = 12000.f;
	else if(i_new < -12000.f)
		i_new = -12000.f;

	return i_new;
}

float cal_power(Power_Limit_t* Power_Limit, Chassis_Motor_Cnt_t num)
{
	float power;
	
	switch(num)
	{
		case CHAS_LF:
			power =	\
			Power_Limit->k1[CHAS_LF]*chassis.dev->chas_motor[CHAS_LF]->rx_info.speed*chassis.info->chas_lf_set+
			Power_Limit->k2[CHAS_LF]*chassis.info->chas_lf_set*chassis.info->chas_lf_set	+
			Power_Limit->k3[CHAS_LF]*chassis.dev->chas_motor[CHAS_LF]->rx_info.speed*chassis.dev->chas_motor[CHAS_LF]->rx_info.speed+
			Power_Limit->p0[CHAS_LF];
			break;
			
		case CHAS_RF:
			power =	\
			Power_Limit->k1[CHAS_RF]*chassis.dev->chas_motor[CHAS_RF]->rx_info.speed*chassis.info->chas_rf_set+
			Power_Limit->k2[CHAS_RF]*chassis.info->chas_rf_set*chassis.info->chas_rf_set	+
			Power_Limit->k3[CHAS_RF]*chassis.dev->chas_motor[CHAS_RF]->rx_info.speed*chassis.dev->chas_motor[CHAS_RF]->rx_info.speed+
			Power_Limit->p0[CHAS_RF];
			break;
			
		case CHAS_LB:
			power =	\
			Power_Limit->k1[CHAS_LB]*chassis.dev->chas_motor[CHAS_LB]->rx_info.speed*chassis.info->chas_lb_set+
			Power_Limit->k2[CHAS_LB]*chassis.info->chas_lb_set*chassis.info->chas_lb_set	+
			Power_Limit->k3[CHAS_LB]*chassis.dev->chas_motor[CHAS_LB]->rx_info.speed*chassis.dev->chas_motor[CHAS_LF]->rx_info.speed	+
			Power_Limit->p0[CHAS_LB];
			break;
			
		case CHAS_RB:
			power =	\
			Power_Limit->k1[CHAS_RB]*chassis.dev->chas_motor[CHAS_RB]->rx_info.speed*chassis.info->chas_rb_set	+
			Power_Limit->k2[CHAS_RB]*chassis.info->chas_rb_set*chassis.info->chas_rb_set	+
			Power_Limit->k3[CHAS_RB]*chassis.dev->chas_motor[CHAS_RB]->rx_info.speed*chassis.dev->chas_motor[CHAS_RB]->rx_info.speed	+
			Power_Limit->p0[CHAS_RB];
			break;
			
		default:
			break;
	}

	return power;
}

void Chassis_Motor_Power_Limit(int16_t* lf, int16_t* rf, int16_t* lb, int16_t* rb,float limit)
{
	float buffer = judge.info->power_heat_data.chassis_power_buffer;
	float heat_rate, Limit_k, CHAS_LimitOutput, CHAS_TotalOutput;
	
	float OUT_MAX = 0.f;
	
	OUT_MAX = CHAS_SP_MAX_OUT * 4.f;
	
	// limit = 60.f
	
	if(buffer > limit)
		buffer = limit;								//防止飞坡之后缓冲250J变为正增益系数
	
	Limit_k = buffer / limit;
	
	if(buffer < limit)
		Limit_k = Limit_k * Limit_k ;
	else
		Limit_k = 1.f;	
	
	if(buffer < limit)
		CHAS_LimitOutput = Limit_k * OUT_MAX;
	else 
		CHAS_LimitOutput = OUT_MAX;    
	
	CHAS_TotalOutput = abs(*lf) + abs(*rf) + abs(*lb) + abs(*rb) ;
	
	heat_rate = CHAS_LimitOutput / CHAS_TotalOutput;
	
  if(CHAS_TotalOutput >= CHAS_LimitOutput)
  {
		*lf = (int16_t)(*lf * heat_rate);	
		*rf = (int16_t)(*rf * heat_rate);	
		*lb = (int16_t)(*lb * heat_rate);	
		*rb = (int16_t)(*rb * heat_rate);	
	}
}
