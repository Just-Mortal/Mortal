/* Includes ------------------------------------------------------------------*/
#include "module.h"

/* Private function prototypes -----------------------------------------------*/
static void Infantry_Heartbeat(struct Infantry_struct_t* Infantry);	// 心跳
static void Infantry_Update(struct Infantry_struct_t* Infantry);		// 更新
static void Infantry_Work(struct Infantry_struct_t* Infantry);			// 工作


static void Operator_Info_Update(Infantry_t* Infantry);
static void Module_Info_Update(Infantry_t* Infantry);

// 指令控制
static void Module_Status_Set(Infantry_t*	Infantry);
static void Judge_Launch_Ctrl(Infantry_t*	Infantry);
static void Module_Work_Open(Infantry_t*	Infantry);
static void Module_Motor_Ctrl(Infantry_t*	Infantry);
static void Module_Vision_Send(Infantry_t*	Infantry);
static void Module_Vision_Ctrl(Infantry_t* Infantry);
static void Module_Slave_Ctrl(Infantry_t* Infantry);
static void Red_Dot_Switch(void);

#if MEASURE_LAUNCH_DELAY_SWITCH == 1
static void Measure_Delay_Launch(void);
uint32_t test_launch_cnt;
#endif

/* Exported variables --------------------------------------------------------*/
Infantry_Cmd_t 	infantry_cmd;
Infantry_flag_t	infantry_flag;

Infantry_t infantry =
{
	.mode = INIT_MODE,
	
	.cmd	=	&infantry_cmd,
	.flag	=	&infantry_flag,

	.chassis_module		= &chassis,
	.gimbal_module		= &gimbal,
	.launch_module		= &launch,

	.rc_module				= &rc,
	.slave_module			= &slave,
	.judge_module			=	&judge,
	.vision_module		= &vision,
	
	.heartbeat				= Infantry_Heartbeat,
	.update						= Infantry_Update,
	.work							=	Infantry_Work,
};

uint8_t chas_spin_cnt;

/* Exported functions --------------------------------------------------------*/
/*--------------- export functions begin ----------------*/
void Infantry_Heartbeat(struct Infantry_struct_t* Infantry)
{
	rc_t*				RC			= Infantry->rc_module;
	Vision_t*		Vision	=	Infantry->vision_module;
	Judge_t*		Judge		=	Infantry->judge_module;
	
	Chassis_t*	Chassis = Infantry->chassis_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	=	Infantry->launch_module;
	
	Gimbal->heartbeat(Gimbal);
	Launch->heartbeat(Launch);
	Chassis->heartbeat(Chassis);
	
	RC->dev->heartbeat(RC->dev);
	Vision->heartbeat(Vision);
	Judge->heartbeat(Judge);
}

// 数据层
void Infantry_Update(struct Infantry_struct_t* Infantry)
{
	Infantry_Status_Update(Infantry);			// 更新指令
	
	if(Infantry->flag->INFANTRY_INIT_FLAG	== true)
	{
		Operator_Info_Update(Infantry);			// 操作数据
		Module_Info_Update(Infantry);				// 模块数据
	}
}

void Infantry_Work(struct Infantry_struct_t* Infantry)
{
	Vision_t*	Vision = Infantry->vision_module;
	Launch_t* Launch = Infantry->launch_module;
	
	Module_Status_Set(Infantry);
	
	if(Launch->flag->FRIC_SPEED_READY_FLAG == true)
		Judge_Launch_Ctrl(Infantry);
	
	if(Vision->work_state == DEV_ONLINE)
		Module_Vision_Ctrl(Infantry);
	
	Module_Work_Open(Infantry);
	Module_Motor_Ctrl(Infantry);
	Module_Vision_Send(Infantry);
		
	Module_Slave_Ctrl(Infantry);
	Red_Dot_Switch();
	
	// 测量发弹延时
	#if MEASURE_LAUNCH_DELAY_SWITCH == 1
	Measure_Delay_Launch();
	#endif
}
/*---------------- export functions end   ----------------*/


/* Private functions ---------------------------------------------------------*/
/*--------------- private functions begin --------------*/

/*---------------- 数据层 begin ---------------*/

void Operator_Info_Update(Infantry_t* Infantry)
{
	rc_t*				RC 			= Infantry->rc_module;
	Chassis_t*	Chassis = Infantry->chassis_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Vision_t*		Vision	=	Infantry->vision_module;
	
	double gimb_pch, gimb_yaw, chas_f, chas_r;
	
	switch(Infantry->ctrl)
	{
		case RC_CTRL:
			Chassis->info->front_speed = (float)RC->info->CH3.val*(CHASSIS_MOVE_SPEED/660.f);
			Chassis->info->right_speed = (float)RC->info->CH2.val*(CHASSIS_MOVE_SPEED/660.f);
			
			switch(Infantry->mode)
			{
				case MEC_MODE:
					gimb_pch = (float)RC->info->CH1.val*GIMBAL_PCH_MEC_SENSITIVITY/660.f;
					Gimbal->info->target->pch_mec_angle += gimb_pch;
					
					Chassis->info->cycle_speed = (float)RC->info->CH0.val*(CHASSIS_SPIN_SPEED/660.f);
					break;
				
				case IMU_MODE:
					gimb_yaw	= (float)RC->info->CH0.val*GIMBAL_YAW_IMU_SENSITIVITY/660.f;
					gimb_pch	= (float)RC->info->CH1.val*GIMBAL_PCH_IMU_SENSITIVITY/660.f;
					
					Gimbal->info->target->pch_imu_angle -= gimb_pch;
					Gimbal->info->target->yaw_imu_angle -= gimb_yaw;
					
					break;
				
				default:
					break;
			}
			
			break;
			
		case KEY_CTRL:
				Chassis->info->front_speed = (float)RC->info->W.press_time*CHASSIS_MOVE_SPEED/KEY_MOVE_TIME
																	 - (float)RC->info->S.press_time*CHASSIS_MOVE_SPEED/KEY_MOVE_TIME;
				Chassis->info->right_speed = (float)RC->info->D.press_time*CHASSIS_MOVE_SPEED/KEY_MOVE_TIME
																	 - (float)RC->info->A.press_time*CHASSIS_MOVE_SPEED/KEY_MOVE_TIME;
																	 
				switch (Infantry->mode)
				{
					case MEC_MODE:
						gimb_pch = (float)Infantry->rc_module->info->MOUSE_VY.val/GIMBAL_PCH_KEY_MEC_SENSITIVITY;
						Gimbal->info->target->pch_mec_angle -= gimb_pch;
						
						Chassis->info->cycle_speed = (float)RC->info->MOUSE_VX.val * CHASSIS_KEY_SPIN_SENSITIVITY ;
						break;
					
					case IMU_MODE:
						gimb_yaw	= (float)RC->info->MOUSE_VX.val/GIMBAL_YAW_KEY_IMU_SENSITIVITY;
						gimb_pch	= (float)RC->info->MOUSE_VY.val/GIMBAL_PCH_KEY_IMU_SENSITIVITY;
						
						Gimbal->info->target->yaw_imu_angle -= gimb_yaw;
						Gimbal->info->target->pch_imu_angle += gimb_pch;
						break;
					
					default:
						break;
				}
			break;
			
		default:
			break;
	}
	
	// 遥控读入数据限制矢量和 begin //
	chas_f = Chassis->info->front_speed;
	chas_r = Chassis->info->right_speed;
	Chassis->info->move_speed = sqrt(pow(chas_f, 2.f)+pow(chas_r, 2.f));
	if(Chassis->info->move_speed > CHASSIS_MOVE_SPEED)
	{
		Chassis->info->front_speed = chas_f / Chassis->info->move_speed * CHASSIS_MOVE_SPEED;
		Chassis->info->right_speed = chas_r / Chassis->info->move_speed * CHASSIS_MOVE_SPEED;
	}
	Chassis->info->move_speed = sqrt(pow(Chassis->info->front_speed, 2.f)+pow(Chassis->info->right_speed, 2.f));
	// 遥控读入数据限制矢量和 end   //
	
	if(Gimbal->mode == GIM_IMU)
	{
		if(Infantry->cmd->GIM_AIM_ON_CMD == false || Vision->work_state == DEV_OFFLINE ||\
			(Vision->info->is_find_buff	== 0 				&& Vision->info->is_find_target == 0) )
			Infantry->flag->VIS_CTRL_GIMBAL_FLAG = false;
//		else if(Infantry->flag->VIS_BUFF_SHOOT_FLAG == true && Vision->info->is_find_buff	== 1)
//		{
//			Gimbal->info->target->pch_imu_angle 	= Gimbal->info->vis_shoot_pch;
//			Gimbal->info->target->yaw_imu_angle		= Gimbal->info->vis_shoot_yaw;
//			Infantry->flag->VIS_CTRL_GIMBAL_FLAG	= true;
//		}
		else
		{
			Gimbal->info->target->pch_imu_angle 	= (Vision->info->target_pitch_angle - HALF_ECD_RANGE) * ECD_TO_ANGLE;
			Gimbal->info->target->yaw_imu_angle		= Vision->info->target_yaw_angle * ECD_TO_ANGLE - 180.f;
			Infantry->flag->VIS_CTRL_GIMBAL_FLAG	= true;
		}
	}
}

//float kd = 120.f;
//float range1 = 200.f, range2 = 450.f;
float yaw_err_i = 0;
float follow_kp = 2.25f, follow_ki = 0.f;
float follow_i_max = 8000.f;
//float rang1 = 300.f;
void Module_Info_Update(Infantry_t* Infantry)
{
	Chassis_t*	Chassis	=	Infantry->chassis_module;
	Gimbal_t*		Gimbal	= Infantry->gimbal_module;
	
	int16_t yaw_err;
	float follow_p;
//	float kd = rang1 / follow_kp;
	
//	float kp1 = range1 / kd;
//	float kp2 = range2 / kd;

	// 开启侧身

	
	// Gimbal	->	Chassis		begin
	yaw_err = Gimbal->info->measure->yaw_mec_angle;	// -2048 ~ 2048
	
	float yaw_delta = yaw_err / 4096.0 * PI ;
	float chas_front_set = Chassis->info->front_speed;
	float chas_right_set = Chassis->info->right_speed;
	
	if(abs(yaw_err) > 2048)
	{
		yaw_err -= 4096 * sgn(yaw_err);
		
		Chassis->info->front_speed *= -1;
		Chassis->info->right_speed *= -1;
	}
	
	yaw_err_i += yaw_err;
	yaw_err_i = constrain(yaw_err_i, -follow_i_max, +follow_i_max);
//	if(yaw_err_i > -1000.f && yaw_err_i < 1000.f)
//		yaw_err_i = 0.f;
	
	if(Infantry->mode == IMU_MODE)
	{
		// 平移
		Chassis->info->front_speed = chas_front_set * cos(yaw_delta) - chas_right_set * sin(yaw_delta);
		Chassis->info->right_speed = chas_front_set * sin(yaw_delta) + chas_right_set * cos(yaw_delta);
	
		// 旋转
		if(	Infantry->cmd->CHAS_SPIN_TOP_CMD == false)
		{
			if(Infantry->flag->GIM_BUFF_MODE_FLAG == false)
			{
				// -------------- 跟随 PID -------------- //
//				if(abs(yaw_err) < rang1)
					follow_p = yaw_err * follow_kp ;
//				else
//					follow_p = yaw_err * yaw_err * sgn(yaw_err)/ kd;
//				else if(abs(yaw_err) > range2)
//					follow_p = kp2 * yaw_err;
				// -------------- 跟随 PID -------------- //
				Chassis->info->cycle_speed = follow_p + yaw_err_i * follow_ki;
			}
			else
			{
				Chassis->info->cycle_speed = 0.f;
				yaw_err_i = 0.f;
			}
		}
		else
			yaw_err_i = 0.f;
	}
	else
		yaw_err_i = 0.f;
	// Gimbal	->	Chassis		end
}

int8_t 	high_cnt = 0, 			low_cnt = 0;
void Judge_Launch_Ctrl(Infantry_t*	Infantry)
{
	Launch_t*		Launch	= Infantry->launch_module;
	Judge_t*		Judge		=	Infantry->judge_module;
	
	float last_measure_speed = 0.f;
	float speed_adapt = 0.f, adapt_k = 2.48f;
	float	target_speed = 28.7f;
	
	/* ---------- 射速限制 ---------- */
	#if ADAPT_SHOOT_SPEED_SWITCH == 1
	
	Launch->info->launch_limit_speed = 30.f;
	last_measure_speed = Launch->info->launch_measure_speed;
	Launch->info->launch_measure_speed = Judge->info->shoot_data.bullet_speed;
		
	if (Judge->work_state == DEV_ONLINE)
	{
    if ((Launch->info->launch_measure_speed != last_measure_speed) && 
        (Launch->info->launch_measure_speed > target_speed - 28.6f + 0.f))
    {
			
			#if MEASURE_LAUNCH_DELAY_SWITCH == 1
			test_launch_cnt++;
			#endif

			Launch->cnt->judge_measure_shoot_cnt = Judge->info->judge_measure_cnt;

			if (Launch->info->launch_measure_speed > target_speed - 28.6f + target_speed - 28.6f + 29.55f)     // (29.55, 29.75]
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = -20.f;                                  // -1.2
			}
			else if (Launch->info->launch_measure_speed > target_speed - 28.6f + 29.15f)     // (29.15, 29.35]
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = -9.f;                                   // -0.6
			}
			else if (Launch->info->launch_measure_speed > target_speed - 28.6f + 28.95f)     // (28.95, 29.15]
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = -5.f;                                   // -0.4
			}
			else if (Launch->info->launch_measure_speed > target_speed - 28.6f + 28.85f)     // (28.85, 28.95]
			{
				low_cnt = 0;
				high_cnt++;
				speed_adapt = -3.f;                               // -0.3
			}
			else if (Launch->info->launch_measure_speed > target_speed - 28.6f + 28.75f)     // (28.75, 28.85]
			{
				low_cnt = 0;
				high_cnt++;
				if (high_cnt > 1)
					speed_adapt = -2.f;                               // -0.2
			}
			else if (Launch->info->launch_measure_speed > target_speed - 28.6f + 28.65f)     // (28.65, 28.75]
			{
				low_cnt = 0;
				high_cnt++;
				if (high_cnt > 2)
					speed_adapt = -1.f;                               // -0.1
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 27.65f)     // (-inf, 27.65)
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = +20.f;                                  // +1.5
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 27.85f)     // [27.65, 27.85)
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = +12.f;                                  // +1.2
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 28.05f)     // [27.85, 28.05)
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = +9.f;                                   // +0.9
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 28.25f)     // [28.05, 28.25)
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = +5.f;                                   // +0.6
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 28.35f)     // [28.25, 28.35)
			{
				low_cnt++;
				high_cnt = 0;
				speed_adapt = +3.f;                               // +0.3
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 28.45f)     // [28.35, 28.45)
			{
				low_cnt++;
				high_cnt = 0;
				if (low_cnt > 1)
					speed_adapt = +2.f;                               // +0.2
			}
			else if (Launch->info->launch_measure_speed < target_speed - 28.6f + 28.55f)     // [28.45, 28.55)
			{
				low_cnt++;
				high_cnt = 0;
				if (low_cnt > 2)
					speed_adapt = +1.f;                               // +0.1
			}
			else                                                      // [28.55, 28.75]
			{
				low_cnt = 0;
				high_cnt = 0;
				speed_adapt = 0;
			}
			
			Launch->conf->fric_speed_config += speed_adapt * adapt_k;
		}
	}
	else
	{
		Launch->info->launch_measure_speed = 0.f;
	}
	#endif

	/* ---------- 热量限制 ---------- */
	#if HEAT_LIMIT_SWITCH == 1
	uint8_t heat_low = 0, heat_high = 0;
	
	Launch->info->launch_limit_heat 	= Judge->info->game_robot_status.shooter_barrel_heat_limit;
	Launch->info->launch_measure_heat = Judge->info->power_heat_data.shooter_id1_17mm_cooling_heat;
	Launch->info->launch_cooling_heat	=	Judge->info->game_robot_status.shooter_barrel_cooling_value;
	Launch->info->launch_remain_heat 	= Launch->info->launch_limit_heat - Launch->info->launch_measure_heat;
	
	if (Launch->info->launch_remain_heat > 100)		//	低热量
		heat_low = 1;
	else 
		heat_low = 0;
	
	if (Launch->info->launch_remain_heat <= 20)		//	高热量
		heat_high = 1;
	else
		heat_high = 0;
	
	if (Judge->work_state == DEV_OFFLINE)
	{
		Infantry->flag->LAUNCH_HIGH_HEAT_FLAG = false;
		
		Launch->conf->dial_speed_config = LAUNCH_DIAL_SPEED_SET;
		Launch->info->launch_limit_heat		= 40;
		Launch->info->launch_measure_heat	= 0;
		Launch->info->launch_remain_heat	= Launch->info->launch_limit_heat - Launch->info->launch_measure_heat;
	}
	else
	{
		if (heat_low == 1)
		{
			Launch->conf->dial_speed_config 			= LAUNCH_DIAL_SPEED_SET;
			
			Infantry->flag->LAUNCH_HIGH_HEAT_FLAG = false;
		}
		else if(heat_low == 0)
		{
			if(heat_high == 0)
			{
				// 1 Hz -> 270 rpm
				Launch->conf->dial_high_heat_conf			=	((float)Launch->info->launch_cooling_heat/10.f+2.5f) * 270.f;
				if(Launch->conf->dial_high_heat_conf < LAUNCH_DIAL_SPEED_SET - 2000.f)
					Launch->conf->dial_speed_config 		= Launch->conf->dial_high_heat_conf;
				else
					Launch->conf->dial_speed_config			=	LAUNCH_DIAL_SPEED_SET - 2000.f;
				Infantry->flag->LAUNCH_HIGH_HEAT_FLAG = false;
			}
			else if(heat_high == 1)
			{
				Launch->conf->dial_speed_config 			= +0.f;
				Launch->cmd->LAUNCH_SINGLE						=	false;
				Launch->cmd->LAUNCH_CLEAN_HEAT				=	false;
				Infantry->flag->LAUNCH_HIGH_HEAT_FLAG = true;
			}
		}
	}
	#endif
}
/*---------------- 数据层 end   ---------------*/

/*---------------- 控制层 begin ---------------*/
#if 1
extern float cap_u;
static void Module_Status_Set(Infantry_t*	Infantry)
{
	Chassis_t*	Chassis = Infantry->chassis_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	=	Infantry->launch_module;
	
	Judge_t*		Judge		=	Infantry->judge_module;

	switch(Infantry->mode)
	{
		case INIT_MODE:
			Chassis->init(Chassis);
			Gimbal->init(Gimbal);
			Launch->init(Launch);
			
			Gimbal->mode 	= GIM_MEC;
			
			break;
			
		case SLEEP_MODE:
			Chassis->mode 		= CHAS_SLEEP;
			Gimbal->mode			= GIM_SLEEP;
			Launch->dial_mode	=	DIAL_SLEEP;
			
			if(abs(Launch->dev->l_fc->rx_info.speed) >	1000.f	||
				 abs(Launch->dev->r_fc->rx_info.speed) >  1000.f)
				Launch->fric_mode = FRIC_STOP;
			else
				Launch->fric_mode = FRIC_SLEEP;
			
			Infantry->cmd->CHAS_SPIN_TOP_CMD = false;
			break;
		
		case IMU_MODE:
			Chassis->mode 		= CHAS_NORMAL;
		
			// ------------ 设置原地小陀螺最大转速 begin ---------- //
			float power_limit, spin_max, spin_conf;
			power_limit= Judge->info->game_robot_status.chassis_power_limit;
			
			if(Infantry->cmd->CAP_OPEN_CMD == true && cap_u > 15.f)
				spin_max = 7000.f;
			else
			{
				spin_max =	60.f * power_limit + 500.f;
				spin_max = spin_max<3000.f?3000.f:(spin_max>7000.f?7000.f:spin_max);
			}
			
			Chassis->conf->chas_spin_max = spin_max;
			// ------------ 设置原地小陀螺最大转速 begin ---------- //
			
			if(Infantry->cmd->CHAS_SPIN_TOP_CMD == true)
			{
				// ---------- 小陀螺移动降低旋转转速 begin ---------- //
				if(Chassis->info->move_speed < CHASSIS_TOP_SPEED - 2500.f)
				{
					//	move_speed	0			-	6000
					//	spin_speed	7000	-	1000
					spin_conf = CHASSIS_TOP_SPEED - Chassis->info->move_speed;
				}
				else
					spin_conf = 2500.f;					//	小陀螺最低转速
					
				spin_conf = spin_conf>spin_max?spin_max:(spin_conf<2500.f?2500.f:spin_conf);
				Infantry->chassis_module->conf->chas_spin_conf = spin_conf;
				// ---------- 小陀螺移动降低旋转转速 end   ---------- //
					
				if(chas_spin_cnt % 2 == 1)
					Chassis->info->cycle_speed = -Infantry->chassis_module->conf->chas_spin_conf;
				else
					Chassis->info->cycle_speed = Infantry->chassis_module->conf->chas_spin_conf;
			}
			
			if(Infantry->cmd->GIM_TRUN_LEFT_45_CMD == true)
			{
				Gimbal->info->target->yaw_imu_angle += 	45.f;
				Infantry->cmd->GIM_TRUN_LEFT_45_CMD = 	false;
			}
			
			if(Infantry->cmd->GIM_TURN_RIGHT_45_CMD == true)
			{
				Gimbal->info->target->yaw_imu_angle -= 	45.f;
				Infantry->cmd->GIM_TURN_RIGHT_45_CMD = 	false;
			}
			
			if(Infantry->cmd->GIM_TURN_180_CMD == true)
			{
				if(Gimbal->cnt->head_swap_cnt == 0)
				{
					Gimbal->info->target->yaw_imu_angle += 180.f;
					Gimbal->cnt->head_swap_cnt++;
				}
				else
				{
					Gimbal->cnt->head_swap_cnt++;
					
					if(abs(Gimbal->dev->yaw->pid.imu_angle.info.err) <= GIMBAL_HEAD_SWAP_OK_ANGLE)
					{
						Gimbal->cnt->head_swap_cnt = 0;
						Infantry->cmd->GIM_TURN_180_CMD = false;
					}
					else if(Gimbal->cnt->head_swap_cnt >= GIMBAL_HEAD_SWAP_TIME)
					{
						Gimbal->cnt->head_swap_cnt					= 0;
						Infantry->cmd->GIM_TURN_180_CMD 		= false;
						
						Gimbal->info->target->yaw_imu_angle = Gimbal->info->measure->yaw_imu_angle;
					}
				}
			}
			
			break;
		
		case MEC_MODE:
			Chassis->mode 		= CHAS_NORMAL;
		
			if(Infantry->cmd->GIM_TURN_180_CMD == true)
			{
				if(Gimbal->cnt->head_swap_cnt == 0)
				{
					Gimbal->info->target->yaw_mec_angle += 4096.f;
					Gimbal->cnt->head_swap_cnt++;
				}
				else
				{
					Gimbal->cnt->head_swap_cnt++;
					
					if(abs(Gimbal->dev->yaw->pid.imu_angle.info.err) <= GIMBAL_HEAD_SWAP_MEC_OK)
					{
						Gimbal->cnt->head_swap_cnt = 0;
						Infantry->cmd->GIM_TURN_180_CMD = false;
					}
					else if(Gimbal->cnt->head_swap_cnt >= GIMBAL_HEAD_SWAP_TIME)
					{
						Gimbal->cnt->head_swap_cnt = 0;
						Infantry->cmd->GIM_TURN_180_CMD = false;
					}
				}
			}
			break;
			
		default:
			break;
	}
	
	if(Infantry->cmd->LNCH_CLEAN_HEAT_CMD == true &&
			Launch->info->launch_remain_heat > 20.f		)
	{
		Launch->cmd->LAUNCH_CLEAN_HEAT	= true;

		Launch->cmd->LAUNCH_CONTINUOUS	=	false;
		Launch->cmd->LAUNCH_SINGLE			= false;
		
		Launch->info->launch_swift_heat = Launch->info->launch_remain_heat - 20.f;
	}
	else
		Launch->cmd->LAUNCH_CLEAN_HEAT	= false;
}
static void Module_Work_Open(Infantry_t*	Infantry)
{
	Chassis_t*	Chassis = Infantry->chassis_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	=	Infantry->launch_module;
	
	Gimbal->work(Gimbal);
	
	if(Infantry->ctrl == RC_CTRL)
	{
		if(Infantry->flag->LAUNCH_OPEN_LOCK_FLAG == false)
			Launch->work(Launch);
		else
		{
			Launch->info->l_fc_motor_set = 0;
			Launch->info->r_fc_motor_set = 0;
			Launch->info->dial_motor_set = 0;
		}
	}
	else
		Launch->work(Launch);				// 键盘模式无开控锁
		
	if(	Gimbal->flag->GIMBAL_INIT_OK_FLAG		== true && 
			Infantry->cmd->GIM_TURN_180_CMD			== false	&&
			Infantry->flag->CHAS_OPEN_LOCK_FLAG	== false)
		Chassis->work(Chassis);
}

extern bool Cap_Switch;
static void Module_Motor_Ctrl(Infantry_t*	Infantry)
{
	#if GIMBAL_SWITCH == 1
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	
	if(Judge_if_nan(Gimbal->info->pch_motor_set))
		Gimbal->info->pch_motor_set = 0;
	if(Judge_if_nan(Gimbal->info->yaw_motor_set))
		Gimbal->info->yaw_motor_set = 0;							
	
	int16_t pch = Gimbal->info->pch_motor_set ;
	int16_t yaw = Gimbal->info->yaw_motor_set ;
	if(Infantry->rc_module->dev->work_state == DEV_ONLINE)
		CAN2_Send_1FF_Motor(yaw, pch, 0, 0);
	else
		CAN2_Send_1FF_Motor(0, 0, 0, 0);
	#endif
	
	
	#if LAUNCH_SWITCH == 1
	Launch_t*		Launch	=	Infantry->launch_module;
	
	if(Judge_if_nan(Launch->info->l_fc_motor_set))
		Launch->info->l_fc_motor_set = 0;
	if(Judge_if_nan(Launch->info->r_fc_motor_set))
		Launch->info->r_fc_motor_set = 0;	
	if(Judge_if_nan(Launch->info->dial_motor_set))
		Launch->info->dial_motor_set = 0;	
	
	int16_t l_f = Launch->info->l_fc_motor_set ;
	int16_t r_f = Launch->info->r_fc_motor_set ;
	int16_t dal = Launch->info->dial_motor_set ;
	
	if(Infantry->rc_module->dev->work_state == DEV_ONLINE)
		CAN2_Send_200_Motor(l_f, r_f, dal, 0);
	else if(abs(Launch->dev->l_fc->rx_info.speed) >	1000	&&
					abs(Launch->dev->r_fc->rx_info.speed) > 1000)
	{
		CAN2_Send_200_Motor(l_f, r_f, 0, 0);
	}
	else			//	只要一个摩擦轮降速就全卸力
	{
		CAN2_Send_200_Motor(0, 0, 0, 0);
		Launch->info->l_fc_motor_set = 0;
		Launch->info->r_fc_motor_set = 0;
		Launch->info->dial_motor_set = 0;
	}
	#endif
	
	
	#if CHASSIS_SWITCH == 1
	Chassis_t*	Chassis = Infantry->chassis_module;
	
	#if CHASSIS_POWER_LIMIT_MODE == 1
	Chassis_Motor_Power_Limit(&Chassis->info->chas_lf_set, &Chassis->info->chas_rf_set, \
														&Chassis->info->chas_lb_set, &Chassis->info->chas_rb_set, 60.f);
	#endif
	
	#if CHASSIS_POWER_LIMIT_MODE == 2
	Cap_Switch = Infantry->cmd->CAP_OPEN_CMD;
	Chassis_Power_Limit_Ctrl(&power_limit);

	// 保险基于 Buffer 限制 25 buffer
	Chassis_Motor_Power_Limit(&Chassis->info->chas_lf_set, &Chassis->info->chas_rf_set, \
														&Chassis->info->chas_lb_set, &Chassis->info->chas_rb_set, 25.f);
	#endif

//	Chassis_Power_Limit_Update(&power_limit);
//	Chassis_Power_Limit_Verify(&power_limit);

	if(Judge_if_nan(Chassis->info->chas_lf_set ))
		Chassis->info->chas_lf_set  = 0;
	if(Judge_if_nan(Chassis->info->chas_rf_set))
		Chassis->info->chas_rf_set = 0;	
	if(Judge_if_nan(Chassis->info->chas_lb_set))
		Chassis->info->chas_lb_set = 0;	
	if(Judge_if_nan(Chassis->info->chas_rb_set))
		Chassis->info->chas_rb_set = 0;	
														
	int16_t lf = Chassis->info->chas_lf_set ;
	int16_t rf = Chassis->info->chas_rf_set ;
	int16_t lb = Chassis->info->chas_lb_set ;
	int16_t rb = Chassis->info->chas_rb_set ;

	if(Infantry->rc_module->dev->work_state == DEV_ONLINE)
		CAN1_Send_200_Motor(lf, rf, lb, rb);
	else
		CAN1_Send_200_Motor(0, 0, 0, 0);
	#endif

}

uint8_t vision_fps ;
static void Module_Vision_Send(Infantry_t*	Infantry)
{
	vision_fps ++ ;
	
	Vision_t* 	Vision 		= Infantry->vision_module ;
	Gimbal_t*		Gimbal		= Infantry->gimbal_module;
	
	Vision->info->cmd_mode = AIM_ON;
	
	// is_change_target
	if(Infantry->cmd->VIS_CHANGE_TARGET_CMD == true)
	{
		Vision->info->tx_info->is_change_target ++;
		Infantry->cmd->VIS_CHANGE_TARGET_CMD = false;
	}
	
	// angle
	Vision->info->tx_info->yaw_angle 	= \
	Gimbal->info->measure->yaw_imu_angle * ANGLE_TO_ECD + HALF_ECD_RANGE;

	Vision->info->tx_info->pitch_angle=	\
	(Gimbal->info->measure->pch_imu_angle + 180.f)  * ANGLE_TO_ECD;
	
//	Vision->info->tx_info->roll_angle	=	imu.data.rpy.roll * ANGLE_TO_ECD;
//	
	// speed // test code begin
//	Vision->info->tx_info->yaw_speed		= Gimbal->info->measure->yaw_imu_speed;
	
	if(Key_GPIO_Update(1) == GPIO_PIN_SET)
		Vision->info->tx_info->size_3 = 1;
	else
		Vision->info->tx_info->size_3 = 0;
		
	if(Key_GPIO_Update(2) == GPIO_PIN_SET)
		Vision->info->tx_info->size_4 = 1;
	else
		Vision->info->tx_info->size_4 = 0;
	
	if(Key_GPIO_Update(3) == GPIO_PIN_SET)
		Vision->info->tx_info->size_5 = 1;
	else
		Vision->info->tx_info->size_5 = 0;
	
	// judge
	// color blood 更新在 judge.update()
	
	// vision mode
	if(Infantry->cmd->VIS_SMALL_BUFF_CMD == true )
	{
		Vision->info->tx_info->mode = AIM_SMALL_BUFF;
		Vision->info->mode = AIM_SMALL_BUFF;
	}
	else if(Infantry->cmd->VIS_LARGE_BUFF_CMD == true )
	{
		Vision->info->tx_info->mode = AIM_BIG_BUFF;
		Vision->info->mode = AIM_BIG_BUFF;
	}
	else if(Infantry->cmd->VIS_OUTPOST_CMD == true )
	{
		Vision->info->tx_info->mode = AIM_OUTPOST;
		Vision->info->mode = AIM_OUTPOST;
	}
	else
	{
		Vision->info->tx_info->mode = AIM_ON;
		Vision->info->mode = AIM_ON;
	}

	// send
	if(vision_fps % VISION_TIME == 0)
	{
		vision_fps = 0;
		vision_send_data();
	}
}

uint16_t buff_cnt = 0;
uint16_t test_buff_cnt, frist_buff_cnt = 650;
uint16_t large_buff = 650;
uint8_t buff_shoot_cnt, buff_gimbal_cnt;
float pch_err, yaw_err;
void Module_Vision_Ctrl(Infantry_t* Infantry)
{
	int8_t vis_fire_max, judge_fire_max;

	Vision_t* 	Vision 		= Infantry->vision_module;
	Launch_t*		Launch		=	Infantry->launch_module;
//	Gimbal_t*		Gimbal		=	Infantry->gimbal_module;
	
	pch_err = abs(Vision->info->rx_info->pitch_angle - Vision->info->tx_info->pitch_angle);
	yaw_err	=	abs(Vision->info->rx_info->yaw_angle - Vision->info->tx_info->yaw_angle);
	
	// test code begin
//	if(Vision->info->is_find_buff == 0)
//		test_buff_cnt ++;
//	else
//		test_buff_cnt = 0;
//		
//	if(test_buff_cnt > 1000)
//	{
//		test_buff_cnt = 0;
//		Launch->conf->large_buff_cnt_max = frist_buff_cnt;
//	}
	// test code end
	Launch->conf->large_buff_cnt_max = large_buff;
	
	if((Infantry->cmd->GIM_AIM_ON_CMD 				== true 	|| \
		  Infantry->cmd->TEST_GIM_AIM_ON_CMD		== true ) && \
		 (Infantry->flag->VIS_CTRL_LAUNCH_FLAG	== true 	|| \
		 Infantry->flag->TEST_VIS_CTRL_FLAG			== true ) && \
		 Launch->flag->FRIC_SPEED_READY_FLAG		== true		&& \
		 Vision->info->is_hit_enable						== 1		)
	{
		Launch->flag->VIS_CTRL_FLAG = true;
	
		if(Vision->info->is_find_buff 					== true	&& \
			(Infantry->cmd->VIS_LARGE_BUFF_CMD		== true	|| \
			 Infantry->cmd->VIS_SMALL_BUFF_CMD		== true))			// is_find_target == 0
		{
		/* --- 打符模式 begin --- */
			Infantry->flag->GIM_BUFF_MODE_FLAG	= true;
	
			if(Infantry->cmd->VIS_SMALL_BUFF_CMD == true )
			{
				if (++buff_cnt >= Launch->conf->small_buff_cnt_max)
				{
					buff_cnt = 0;
					Launch->cnt->vis_shoot_cnt = 1;
					Launch->cmd->LAUNCH_SINGLE = true ;
				}
			}
			else if(Infantry->cmd->VIS_LARGE_BUFF_CMD == true )
			{
				// 快速大符
				if(Infantry->cmd->VIS_FAST_BUFF_CMD == true)
				{
					if (++buff_cnt >= 750)
					{
						buff_cnt = 0;
						Launch->cnt->vis_shoot_cnt = 1;
						Launch->cmd->LAUNCH_SINGLE = true ;
					}
				}
				// 快速大符
				// 慢速大符
				else if(yaw_err < 15.f && pch_err < 15.f)
				{
					if (++buff_cnt >= Launch->conf->large_buff_cnt_max)
					{
						buff_cnt			= 0;
						Launch->cnt->vis_shoot_cnt = 1;
						Launch->cmd->LAUNCH_SINGLE = true ;
					}
					else if(yaw_err > 25.f || pch_err > 25.f)		// 排除云台响应的影响
					{
						if(buff_cnt < large_buff - 300)						// 防止云台响应打出
							buff_cnt ++;
					}
				}
				// 慢速大符
			}
		/* --- 打符模式 end   --- */
		}
		else if(Vision->info->is_find_target)
		{
			Infantry->flag->GIM_BUFF_MODE_FLAG	= false;
			
		/* --- 自瞄模式 begin --- */
			// shoot type			0 连发	1 单发
			if(Vision->info->shoot_type == 0)
			{
				Launch->cmd->LAUNCH_CONTINUOUS = true ;
				Launch->cmd->LAUNCH_SINGLE		 = false;
			}
			else if(Vision->info->shoot_type == 1)
			{
				Launch->cnt->vis_shoot_cnt = Vision->info->rx_info->fire_cnt;
				
				// 操作手反应自瞄限制不了热量
				judge_fire_max = Launch->info->launch_remain_heat / 10 - 2;		// int8_t
				vis_fire_max = judge_fire_max>6?6:judge_fire_max;
				if(vis_fire_max < 0) vis_fire_max = 0;
				
				// vis_shoot_cnt uint8_t 型 数据
				if(Launch->cnt->vis_shoot_cnt >= vis_fire_max)
					Launch->cnt->vis_shoot_cnt = vis_fire_max;
			
				if (Launch->cnt->vis_shoot_cnt > 0 && Launch->cmd->LAUNCH_SINGLE == false)
				{
					Launch->cmd->LAUNCH_SINGLE 			= true;
					Launch->cmd->LAUNCH_CONTINUOUS 	= false;
				}
			}

		/* --- 自瞄模式 end   --- */
		}
	}
	else
	{
		buff_cnt = 0;
		Launch->cnt->vis_shoot_cnt = 0;
		Infantry->flag->GIM_BUFF_MODE_FLAG	= false;
		Infantry->flag->VIS_BUFF_SHOOT_FLAG = false;
		Infantry->flag->VIS_BUFF_CUT_FLAG 	= false;
		Launch->flag->VIS_CTRL_FLAG = false;
	}
}

#if 0
void Module_Slave_Ctrl(Infantry_t* Infantry)
{
	Slave_t*		Slave		= Infantry->slave_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	= Infantry->launch_module;
	Vision_t*		Vision	= Infantry->vision_module;
	Chassis_t*	Chassis	= Infantry->chassis_module;
	
	int16_t yaw_angle		=	Gimbal->dev->yaw->rx_info.angle;
	int16_t spin_speed	=	Chassis->conf->chas_spin_conf;		// float -> int16_t
	
	uint8_t* slave_data_tx_buf = Slave->tx_buf;
	
	// for example:
	// 1111 1110	// if false -> 0 -> OFF
	// 0000 0001	// if true	->
	
	// 0000 0001	// 1111 1110	-> 0xFE
	slave_data_tx_buf[0] &= 0xFE;				// 开关小陀螺 0关			1开
	if (Infantry->cmd->CHAS_SPIN_TOP_CMD == true)
		slave_data_tx_buf[0] |= 0x01;
	
	// 0000 0010	// 1111 1101	-> 0xFD
	slave_data_tx_buf[0] &= 0xFD;				// 整车模式 	0陀螺仪 1机械
	if (Infantry->mode == MEC_MODE)
		slave_data_tx_buf[0] |= 0x02;
	
	// 0000 0100	// 1111 1011	-> 0xFB
	slave_data_tx_buf[0] &= 0xFB;				// 识别到目标 0无 		1有
	if ((Vision->work_state == DEV_ONLINE) && 
		 ((Vision->info->is_find_target == 1) ||\
			(Vision->info->is_find_buff == 1)))
		slave_data_tx_buf[0] |= 0x04;
	
	// 0000 1000	// 1111 0111	-> 0xF7
	slave_data_tx_buf[0] &= 0xF7;				// 前哨模式		0无			1有
	if (Infantry->cmd->VIS_OUTPOST_CMD == true)
		slave_data_tx_buf[0] |= 0x08;

	// 0001 0000	// 1110 1111	-> 0xEF
	slave_data_tx_buf[0] &= 0xEF;				// 发射开关		0关			1开
	if (Launch->flag->FRIC_SPEED_READY_FLAG == true)
		slave_data_tx_buf[0] |= 0x10;
	
	// 0010 0000	// 1101 1111	-> 0xDF
	slave_data_tx_buf[0] &= 0xDF;				// 发射电机		0在线		1离线
	if ((Launch->dev->dial->state.work_state == M_OFFLINE)	||
			(Launch->dev->l_fc->state.work_state == M_OFFLINE)	||
			(Launch->dev->r_fc->state.work_state == M_OFFLINE))
		slave_data_tx_buf[0] |= 0x20;
	
	// 0100 0000	// 1011 1111	-> 0xBF
	slave_data_tx_buf[0] &= 0xBF;				//	云台电机	0在线		1离线
	if((Gimbal->dev->yaw->state.work_state == M_OFFLINE) || 
		 (Gimbal->dev->pch->state.work_state == M_OFFLINE))
		slave_data_tx_buf[1] |= 0x40;
	
	// 1000 0000	// 0111 1111	-> 0x7F
	slave_data_tx_buf[0] &= 0x7F;				// 底盘电机		0在线		1离线
	if ((Chassis->dev->chas_motor[CHAS_LF]->state.work_state == M_OFFLINE) || 
			(Chassis->dev->chas_motor[CHAS_RF]->state.work_state == M_OFFLINE) ||
			(Chassis->dev->chas_motor[CHAS_LB]->state.work_state == M_OFFLINE) || 
			(Chassis->dev->chas_motor[CHAS_RB]->state.work_state == M_OFFLINE))
		slave_data_tx_buf[0] |= 0x80;
	
	// 0000 0001	// 1111 1110	-> 0xFE
	slave_data_tx_buf[1] &= 0xFE;				// 视觉状态		0在线		1离线
	if (Vision->work_state == DEV_OFFLINE)
		slave_data_tx_buf[1] |= 0x01;
	
	// 0000 1000	// 1111 0111	-> 0xF7
	slave_data_tx_buf[1] &= 0xF7;				// 自瞄状态		0开			1关
	if(Infantry->flag->VIS_CTRL_GIMBAL_FLAG == true)
		slave_data_tx_buf[1] |= 0x08;
		
	// 0001 0000	// 1110 1111	-> 0xEF
	slave_data_tx_buf[1] &= 0xEF;				// 自动打弹		0关			1开
	if(Infantry->flag->VIS_CTRL_LAUNCH_FLAG == true)
		slave_data_tx_buf[1] |= 0x10;
	
	// 0010 0000	// 1101	1111	-> 0xDF
	slave_data_tx_buf[1] &= 0xDF;				// 大符开关		0关			1开
	if(Infantry->flag->GIM_BUFF_MODE_FLAG == true && 
		 Infantry->cmd->VIS_LARGE_BUFF_CMD	== true)
		slave_data_tx_buf[1] |= 0x20;
		
	// 0100 0000	// 1011	1111	-> 0xBF
	slave_data_tx_buf[1] &= 0xBF;				// 小符开关		0关			1开
	if(Infantry->flag->GIM_BUFF_MODE_FLAG == true && 
		 Infantry->cmd->VIS_SMALL_BUFF_CMD	== true)
		slave_data_tx_buf[1] |= 0x40;
		
	// 1000 0000	// 0111	1111	-> 0x7F
	slave_data_tx_buf[1] &= 0x7F;				// 超电开关		0关			1开
	if(Infantry->cmd->CAP_OPEN_CMD ==	true)
		slave_data_tx_buf[1] |= 0x80;
	
	slave_data_tx_buf[2] = yaw_angle;				// 高八位
	slave_data_tx_buf[3] = yaw_angle >> 8;	// 低八位
	
	slave_data_tx_buf[4] = spin_speed;			// 高八位
	slave_data_tx_buf[5] = spin_speed >> 8;	// 低八位
		
	Slave->send(Slave);
}
#endif

void Module_Slave_Ctrl(Infantry_t* Infantry)
{
	Slave_t*		Slave		= Infantry->slave_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	= Infantry->launch_module;
	Vision_t*		Vision	= Infantry->vision_module;
//	Chassis_t*	Chassis	= Infantry->chassis_module;
	
	Slave->info->tx_info.yaw_angle	=	Gimbal->dev->yaw->rx_info.angle;
	Slave->info->tx_info.pitch_imu	=	Gimbal->info->measure->pch_imu_angle * ANGLE_TO_ECD;
	Slave->info->tx_info.pitch_mec	=	Gimbal->info->measure->pch_mec_angle;
	
	// 发射状态
	if( Launch->dev->l_fc->state.work_state == M_OFFLINE ||
			Launch->dev->r_fc->state.work_state == M_OFFLINE ||
			Launch->dev->dial->state.work_state == M_OFFLINE )
		Slave->info->tx_info.flag_info.bit.launch_state = 1;
	else
		Slave->info->tx_info.flag_info.bit.launch_state = 0;
		
	// 云台状态
	if( Gimbal->dev->pch->state.work_state	== M_OFFLINE ||
			Gimbal->dev->yaw->state.work_state	== M_OFFLINE )
		Slave->info->tx_info.flag_info.bit.gimbal_state = 1;
	else
		Slave->info->tx_info.flag_info.bit.gimbal_state = 0;
			
	// 视觉状态
	if( Vision->work_state == DEV_OFFLINE)
		Slave->info->tx_info.flag_info.bit.vision_state = 1;
	else
		Slave->info->tx_info.flag_info.bit.vision_state = 0;
		
	// 超电开关
	if( Infantry->cmd->CAP_OPEN_CMD == true)
		Slave->info->tx_info.flag_info.bit.cap_switch = 1;
	else
		Slave->info->tx_info.flag_info.bit.cap_switch = 0;
	
	// 陀螺开关
	if( Infantry->cmd->CHAS_SPIN_TOP_CMD == true)
		Slave->info->tx_info.flag_info.bit.spin_switch = 1;
	else
		Slave->info->tx_info.flag_info.bit.spin_switch = 0;
	
	// 整车模式
	if( Infantry->mode == MEC_MODE)
		Slave->info->tx_info.flag_info.bit.infantry_state	=	1;
	else
		Slave->info->tx_info.flag_info.bit.infantry_state	=	0;
		
	// 发射就绪
	if( Launch->flag->FRIC_SPEED_READY_FLAG == true)
		Slave->info->tx_info.flag_info.bit.fric_ready = 1;
	else
		Slave->info->tx_info.flag_info.bit.fric_ready = 0;
		
	// 前哨模式
	if(	Vision->work_state == DEV_ONLINE && \
			Infantry->cmd->VIS_OUTPOST_CMD == true)
		Slave->info->tx_info.flag_info.bit.outpost_mode = 1;
	else
		Slave->info->tx_info.flag_info.bit.outpost_mode = 0;
	
	// 发现目标
	if( Vision->work_state == DEV_ONLINE && \
			Vision->info->is_find_target == 1)
		Slave->info->tx_info.flag_info.bit.is_find_target = 1;
	else
		Slave->info->tx_info.flag_info.bit.is_find_target = 0;
	
	// 大符模式
	if(	Vision->work_state == DEV_ONLINE && \
			Infantry->flag->GIM_BUFF_MODE_FLAG == true && \
			Infantry->cmd->VIS_LARGE_BUFF_CMD	== true)
		Slave->info->tx_info.flag_info.bit.large_buff_mode = 1;
	else
		Slave->info->tx_info.flag_info.bit.large_buff_mode = 0;
	
	// 小符模式
	if(	Vision->work_state == DEV_ONLINE && \
			Infantry->flag->GIM_BUFF_MODE_FLAG == true && \
			Infantry->cmd->VIS_SMALL_BUFF_CMD	== true)
		Slave->info->tx_info.flag_info.bit.small_buff_mode = 1;
	else
		Slave->info->tx_info.flag_info.bit.small_buff_mode = 0;
	
	// 视觉控制发射
	if(	Vision->work_state == DEV_ONLINE && \
			Infantry->flag->VIS_CTRL_LAUNCH_FLAG == true)
		Slave->info->tx_info.flag_info.bit.vis_ctrl_launch = 1;
	else
		Slave->info->tx_info.flag_info.bit.vis_ctrl_launch = 0;
		
	// 自瞄开关
	if(	Vision->work_state == DEV_ONLINE && \
			Infantry->flag->VIS_CTRL_GIMBAL_FLAG == true)
		Slave->info->tx_info.flag_info.bit.aim_on_switch = 1;
	else
		Slave->info->tx_info.flag_info.bit.aim_on_switch = 0;
		
	memcpy(Slave->tx_buf, &Slave->info->tx_info, sizeof(Salve_Tx_Info_t));

	Slave->send(Slave);
}

#endif
/*---------------- 控制层 end   ---------------*/
/*---------------- private functions end   ---------------*/
uint8_t dot_key;
void Red_Dot_Switch(void)
{
	dot_key = Key_GPIO_Update(4);

	if(Key_GPIO_Update(4) == GPIO_PIN_SET)
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);		// 开
	else
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET);	// 关
}

#if MEASURE_LAUNCH_DELAY_SWITCH == 1
typedef struct
{
	uint16_t shoot_num;
	
	uint8_t delay_40;		//	40	-	45
	uint8_t delay_45;		//	45	-	50
	uint8_t delay_50;		//	50	-	55
	uint8_t delay_55;		//	55	-	60
	uint8_t delay_60;		//	60	-	65
	uint8_t delay_65;		//	65	-	70
	uint8_t delay_70;		//	70	-	75
	uint8_t delay_75;		//	75	-	80
	uint8_t delay_80;		//	80	-	85
	uint8_t delay_85;		//	85	-	90
	uint8_t delay_90;		//	90	-	95
	uint8_t delay_95;		//	95	-	100
	uint8_t delay_100;	//	100	-	105
	uint8_t delay_105;	//	105	-	110
	uint8_t delay_110;	//	110	-	115
	uint8_t delay_115;	//	115	-	120
	uint8_t delay_120;	//	120	-	125
	uint8_t delay_125;	//	125	-	130
	uint8_t delay_130;	//	130	-	135
	uint8_t delay_135;	//	135	-	140
	uint8_t delay_140;	//	140	-	145

	float mean;
	float variance;
	
}shoot_delay_data_t;

bool delay_cali_flag = true;
shoot_delay_data_t shoot_delay;
uint32_t test_launch_t1, test_launch_t2, test_launch_tmp, shoot_delay_ms;
uint32_t test_time_all,	test_time_num, test_time_avg;
void Measure_Delay_Launch(void)
{
	static uint32_t last_cnt = 0;
	float delay_ms;

	if(rc.info->SW1.flip == SW_UP_TO_MID)
		test_launch_t1 = micros();
		
	if(last_cnt  !=	test_launch_cnt)
	{
		last_cnt = test_launch_cnt;
		
		test_launch_t2	= micros();
		test_launch_tmp = test_launch_t2 - test_launch_t1;
		shoot_delay_ms	=	test_launch_tmp / 1000.f;
		if(shoot_delay_ms < 500 && shoot_delay_ms > 20)
		{
			test_time_all += test_launch_tmp;
			test_time_num ++;
			test_time_avg = test_time_all / test_time_num;

			shoot_delay.shoot_num ++;
		}
		
		
		
		
		
		// 计算均值
		delay_ms = shoot_delay_ms;

		if (delay_ms >= 40 && delay_ms < 45)
			shoot_delay.delay_40 ++;
		else if (delay_ms >= 45 && delay_ms < 50)
			shoot_delay.delay_45 ++;
		else if (delay_ms >= 50 && delay_ms < 55)
			shoot_delay.delay_50 ++;
		else if (delay_ms >= 55 && delay_ms < 60)
			shoot_delay.delay_55 ++;
		else if (delay_ms >= 60 && delay_ms < 65)
			shoot_delay.delay_60 ++;
		else if (delay_ms >= 65 && delay_ms < 70)
			shoot_delay.delay_65 ++;
		else if (delay_ms >= 70 && delay_ms < 75)
			shoot_delay.delay_70 ++;
		else if (delay_ms >= 75 && delay_ms < 80)
			shoot_delay.delay_75 ++;
		else if (delay_ms >= 80 && delay_ms < 85)
			shoot_delay.delay_80 ++;
		else if (delay_ms >= 85 && delay_ms < 90)
			shoot_delay.delay_85 ++;
		else if (delay_ms >= 90 && delay_ms < 95)
			shoot_delay.delay_90 ++;
		else if (delay_ms >= 95 && delay_ms < 100)
			shoot_delay.delay_95 ++;
		else if (delay_ms >= 100 && delay_ms < 105)
			shoot_delay.delay_100 ++;
		else if (delay_ms >= 105 && delay_ms < 110)
			shoot_delay.delay_105 ++;
		else if (delay_ms >= 110 && delay_ms < 115)
			shoot_delay.delay_110 ++;
		else if (delay_ms >= 115 && delay_ms < 120)
			shoot_delay.delay_115 ++;
		else if (delay_ms >= 120 && delay_ms < 125)
			shoot_delay.delay_120 ++;
		else if (delay_ms >= 125 && delay_ms < 130)
			shoot_delay.delay_125 ++;
		else if (delay_ms >= 130 && delay_ms < 135)
			shoot_delay.delay_130 ++;
		else if (delay_ms >= 135 && delay_ms < 140)
			shoot_delay.delay_135 ++;
		else if (delay_ms >= 140 && delay_ms < 145)
			shoot_delay.delay_140 ++;

		if(delay_cali_flag)
		{
			shoot_delay.mean = ( 
				(shoot_delay.delay_40 * 42.5f) +
				(shoot_delay.delay_45 * 47.5f) +
				(shoot_delay.delay_50 * 52.5f) +
				(shoot_delay.delay_55 * 57.5f) +
				(shoot_delay.delay_60 * 62.5f) +
				(shoot_delay.delay_65 * 67.5f) +
				(shoot_delay.delay_70 * 72.5f) +
				(shoot_delay.delay_75 * 77.5f) +
				(shoot_delay.delay_80 * 82.5f) +
				(shoot_delay.delay_85 * 87.5f) +
				(shoot_delay.delay_90 * 92.5f) +
				(shoot_delay.delay_95 * 97.5f) +
				(shoot_delay.delay_100 * 102.5f) +
				(shoot_delay.delay_105 * 107.5f) +
				(shoot_delay.delay_110 * 112.5f) +
				(shoot_delay.delay_115 * 117.5f) +
				(shoot_delay.delay_120 * 122.5f) +
				(shoot_delay.delay_125 * 127.5f) +
				(shoot_delay.delay_130 * 132.5f) +
				(shoot_delay.delay_135 * 137.5f) +
				(shoot_delay.delay_140 * 142.5f) ) / (float)shoot_delay.shoot_num;
			
			// Calculate the variance
			float sum_of_squares = \
				(shoot_delay.delay_40 * pow(42.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_45 * pow(47.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_50 * pow(52.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_55 * pow(57.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_60 * pow(62.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_65 * pow(67.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_70 * pow(72.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_75 * pow(77.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_80 * pow(82.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_85 * pow(87.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_90 * pow(92.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_95 * pow(97.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_100 * pow(102.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_105 * pow(107.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_110 * pow(112.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_115 * pow(117.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_120 * pow(122.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_125 * pow(127.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_130 * pow(132.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_135 * pow(137.5f - shoot_delay.mean, 2)) +
				(shoot_delay.delay_140 * pow(142.5f - shoot_delay.mean, 2));

			shoot_delay.variance = sum_of_squares / (float)shoot_delay.shoot_num;
		}
	}
}
#endif

