/* Includes ------------------------------------------------------------------*/
#include "launch.h"

/* Exported functions --------------------------------------------------------*/
static void Launch_Init(struct Launch_struct_t* Launch);
static void Launch_Work(struct Launch_struct_t* Launch);
static void Launch_Heartbeat(struct Launch_struct_t* Launch);

/* Private function prototypes -----------------------------------------------*/
/*------------ work functions begin ---------------*/
static void Launch_Update(Launch_t* Launch);
static void Launch_Fric_Ctrl(Launch_t* Launch);
static void Launch_Dial_Ctrl(Launch_t* Launch);
static void Launch_Dial_Locked_Check(Launch_t* Launch);
static void Launch_SelfProtect(Launch_t* Launch);
/*------------ work functions end  ----------------*/

/*------------ ctrl functions begin ---------------*/
static void Fric_Speed_Ctrl(Launch_t* Launch);
static void Dial_Position_Ctrl(Launch_t* Launch);
static void Dial_Speed_Ctrl(Launch_t* Launch);
/*------------ ctrl functions end  ---------------*/

/*------------ test functions begin ---------------*/
static void Fric_Measure_Cnt(Launch_t* Launch);
/*------------ test functions end  ---------------*/

/* Exported variables --------------------------------------------------------*/
/*---------------变量定义begin---------------*/
Launch_Dev_t launch_dev ={
	.l_fc = &motor[LAUNCH_FRIC_L],
	.r_fc = &motor[LAUNCH_FRIC_R],
	.dial	= &motor[LAUNCH_DIAL],
};

Launch_Cmd_t	launch_cmd;
Launch_Cnt_t	launch_cnt;
Launch_Info_t	launch_info;
Launch_Flag_t launch_flag;
Launch_Conf_t launch_conf =
{
	.dial_speed_config = LAUNCH_DIAL_SPEED_SET,
	.fric_speed_config = LAUNCH_FRIC_SPEED_30,
	
	.large_buff_cnt_max	=	1000,
	.small_buff_cnt_max	=	650,
	
	.dial_high_heat_conf= 1620,
};

Launch_t launch={
	
	.dev	=	&launch_dev,
	.cmd 	=	&launch_cmd,
	.cnt	=	&launch_cnt,
	.info = &launch_info,
	.flag = &launch_flag,
	.conf	=	&launch_conf,
	
	.init	=	Launch_Init,
	.work = Launch_Work,
	.heartbeat = Launch_Heartbeat,
};
/*---------------变量定义end---------------*/

/*---------------exported functions begin---------------*/
void Launch_Init(struct Launch_struct_t* Launch)
{
	Launch->dev->dial->rx_info.angle_sum = 0;
	
	Launch->dial_mode	= DIAL_INIT;

	Launch->info->dial_motor_set = 0;
	Launch->info->l_fc_motor_set = 0;
	Launch->info->r_fc_motor_set = 0;
	
	Launch->cnt->vis_shoot_cnt						=	0;
	Launch->cnt->dial_init_cnt				= 0;
//	Launch->cnt->vision_shoot_cnt			= 0;
	Launch->cnt->dial_stuck_deal_cnt	= 0;
//	Launch->cnt->last_vision_shoot_cnt= 0;

	Launch->cmd->LAUNCH_SINGLE			=	false;
	Launch->cmd->LAUNCH_CONTINUOUS 	= false;
	
//	Launch->flag->HEAT_CTRL_FLAG					= false;
	Launch->flag->DIAL_STUCK_FLAG 				= false;
	Launch->flag->DIAL_INIT_OK_FLAG				= false;
	Launch->flag->FRIC_SPEED_READY_FLAG 	= false;
	Launch->flag->DIAL_ANGLE_CHANGE_FLAG	=	false;
	
	// 摩擦轮分段 pid 控制。
	Launch->conf->fric_speed_pid.kp					=	10.f;
	Launch->conf->fric_speed_pid.ki					=	0.125f;
	Launch->conf->fric_speed_pid.kd					=	0.f;
	Launch->conf->fric_speed_pid.blind_err	=	0.f;
	Launch->conf->fric_speed_pid.integral_max	=	6000.f;
	Launch->conf->fric_speed_pid.iout_max		=	750.f;
	Launch->conf->fric_speed_pid.out_max		=	12000.f;
	
	Launch->conf->fric_swift_pid.kp					=	20.f;
	Launch->conf->fric_swift_pid.ki					=	0.5f;
	Launch->conf->fric_swift_pid.kd					=	0.5f;
	Launch->conf->fric_swift_pid.blind_err	=	0.f;
	Launch->conf->fric_swift_pid.integral_max	=	6000.f;
	Launch->conf->fric_swift_pid.iout_max		=	6000.f;
	Launch->conf->fric_swift_pid.out_max		=	12000.f;

	Launch->conf->dial_speed_pid.kp					=	20.f;
	Launch->conf->dial_speed_pid.ki					=	0.25;
	Launch->conf->dial_speed_pid.kd					=	0.f;
	Launch->conf->dial_speed_pid.blind_err	=	0.f;
	Launch->conf->dial_speed_pid.integral_max	=	6000.f;
	Launch->conf->dial_speed_pid.iout_max		=	3000.f;
	Launch->conf->dial_speed_pid.out_max		=	9800.f;
	
	Launch->conf->dial_swift_pid.kp					=	60.f;
	Launch->conf->dial_swift_pid.ki					=	1.f;
	Launch->conf->dial_swift_pid.kd					=	20.f;
	Launch->conf->dial_swift_pid.blind_err	=	0.f;
	Launch->conf->dial_swift_pid.integral_max	=	100000.f;
	Launch->conf->dial_swift_pid.iout_max		=	9000.f;
	Launch->conf->dial_swift_pid.out_max		=	9800.f;
}

void Launch_Work(struct Launch_struct_t* Launch)
{
	Launch_Update(Launch);
	
	Launch_Fric_Ctrl(Launch);
	Launch_Dial_Ctrl(Launch);
	
	// test code begin
	Fric_Measure_Cnt(Launch);
	// test code end
	
	Launch_SelfProtect(Launch);
}

void Launch_Heartbeat(struct Launch_struct_t* Launch)
{
	Launch->dev->l_fc->heartbeat(Launch->dev->l_fc);
	Launch->dev->r_fc->heartbeat(Launch->dev->r_fc);
	Launch->dev->dial->heartbeat(Launch->dev->dial);
}
/*---------------exported functions end---------------*/

/* Private functions ---------------------------------------------------------*/
/*--------------- work functions begin ---------------*/
void Launch_SelfProtect(Launch_t* Launch)
{
//	if (Launch->dev->l_fc->state.work_state == M_OFFLINE ||
//			Launch->dev->r_fc->state.work_state == M_OFFLINE ||
//			Launch->dev->dial->state.work_state == M_OFFLINE)
//	{
//		Launch->info->dial_motor_set = 0;
//		Launch->info->l_fc_motor_set = 0;
//		Launch->info->r_fc_motor_set = 0;
//	}

	if (Launch->dev->l_fc->state.work_state == M_OFFLINE ||
			Launch->dev->r_fc->state.work_state == M_OFFLINE)
	{
		Launch->info->dial_motor_set = 0;
		Launch->info->l_fc_motor_set = 0;
		Launch->info->r_fc_motor_set = 0;
	}
}

void Launch_Update(Launch_t* Launch)
{
	if(Launch->fric_mode != FRIC_SLEEP)
	{
		#if DIAL_RESET_SWITCH == 1
		if(Launch->flag->DIAL_INIT_OK_FLAG != true)
		{
			Launch->fric_mode	=	FRIC_STOP;
			Launch->dial_mode	=	DIAL_INIT;
		}
		else
		{
			Launch->dial_mode	=	DIAL_WORK;
		}
		
		#endif
		
		#if DIAL_RESET_SWITCH == 0
		Launch->dial_mode	=	DIAL_WORK;
		#endif
		
		if(abs(Launch->dev->l_fc->rx_info.speed) >= LAUNCH_FRIC_SPEED_READY &&
			 abs(Launch->dev->r_fc->rx_info.speed) >= LAUNCH_FRIC_SPEED_READY)
			Launch->flag->FRIC_SPEED_READY_FLAG = true;
		else
			Launch->flag->FRIC_SPEED_READY_FLAG = false;
		
		if(Launch->flag->FRIC_SPEED_READY_FLAG != true)
		{
			Launch->cmd->LAUNCH_CONTINUOUS = false;
			Launch->cmd->LAUNCH_SINGLE		 = false;
			Launch->cmd->LAUNCH_CLEAN_HEAT = false;
		}
	}
	
	// test code
	Launch->info->l_fc_speed_measure = Launch->dev->l_fc->rx_info.speed;
	Launch->info->r_fc_speed_measure = -Launch->dev->r_fc->rx_info.speed;
	Launch->info->dial_speed_measure = Launch->dev->dial->rx_info.speed;
}

void Launch_Fric_Ctrl(Launch_t* Launch)
{
	switch (Launch->fric_mode)
	{
		case FRIC_SLEEP:
			Launch->info->l_fc_motor_set = 0;
			Launch->info->r_fc_motor_set = 0;
			
			Launch->info->l_fc_speed_target = 0.f;
			Launch->info->r_fc_speed_target = 0.f;
			break;
		
		case FRIC_STOP:
			Launch->info->l_fc_speed_target = 0;
			Launch->info->r_fc_speed_target = 0;
			Fric_Speed_Ctrl(Launch);
		
			Launch->cmd->LAUNCH_CONTINUOUS = false;
			Launch->cmd->LAUNCH_SINGLE		 = false;
		
			break;

		case FRIC_SPIN:

			Launch->info->l_fc_speed_target = Launch->conf->fric_speed_config;
			Launch->info->r_fc_speed_target = -Launch->conf->fric_speed_config;
			
			// 快速响应
//			if(abs(Launch->dev->l_fc->pid.speed.info.err) > LAUNCH_FRIC_CHANGE_PID_ERR	&&\
//				 abs(Launch->dev->r_fc->pid.speed.info.err) > LAUNCH_FRIC_CHANGE_PID_ERR)
//			{
//				Launch->dev->l_fc->pid.speed.set = Launch->conf->fric_swift_pid;
//				Launch->dev->r_fc->pid.speed.set = Launch->conf->fric_swift_pid;
//			}
//			else
//			{
//				Launch->dev->l_fc->pid.speed.set = Launch->conf->fric_speed_pid;
//				Launch->dev->r_fc->pid.speed.set = Launch->conf->fric_speed_pid;
//			}
			
			Fric_Speed_Ctrl(Launch);
			break;
		
		default:
			break;
	}
}

void Launch_Dial_Ctrl(Launch_t* Launch)
{
	switch (Launch->dial_mode)
	{
		case DIAL_SLEEP:
			Launch->info->dial_motor_set = 0;
			break;
			
		case DIAL_INIT:
			#if DIAL_RESET_SWITCH == 1
			Launch->info->dial_speed_target = LAUNCH_DIAL_INIT_SPEED;
			Dial_Speed_Ctrl(Launch);
			
			Launch->cnt->dial_init_cnt ++;
			
			if(	Launch->cnt->dial_init_cnt > LAUNCH_DIAL_INIT_TIME_MAX || 
					Launch->dev->dial->c_stuck_flag(Launch->dev->dial, 1, \
																					LAUNCH_DIAL_INIT_SPEED_ERR, LAUNCH_DIAL_INIT_ERR_TIME))
			{
				Launch->flag->DIAL_INIT_OK_FLAG = true;
				
				Launch->cnt->dial_init_cnt = 0;
				Launch->dev->dial->rx_info.angle_sum = 0.f;
			}
			else
			{
				Launch->flag->DIAL_INIT_OK_FLAG = false;
			}
			#endif
			
			Launch->dev->dial->pid.speed.info.integral = 0.f;
			Launch->dev->dial->pid.speed.info.iout 		 = 0.f;
			break;

		case DIAL_WORK:
			if(Launch->flag->DIAL_STUCK_FLAG == false)				// 未堵转，正常发弹。接收指令
			{
				if(Launch->cmd->LAUNCH_CLEAN_HEAT	== true)			// LAUNCH_CLEAN_HEAT cmd
				{
					Launch->info->dial_speed_target = LAUNCH_DIAL_SWIFT_SET;
					Dial_Speed_Ctrl(Launch);
					
					if(Launch->flag->DIAL_ANGLE_CHANGE_FLAG == false)
						Launch->info->launch_swift_dail_angle = Launch->dev->dial->rx_info.angle_sum;
					
					if(abs(Launch->dev->dial->rx_info.angle_sum - ( Launch->info->launch_swift_dail_angle + 
						LAUNCH_DIAL_ANGLE_SET * Launch->info->launch_swift_heat/10.f)) < LAUNCH_DIAL_ANGLE_OK )
					{
						Launch->flag->DIAL_ANGLE_CHANGE_FLAG  = true;
						Launch->info->launch_swift_heat 			= 0.f;
					}
				}
				else if(Launch->cmd->LAUNCH_CONTINUOUS == true)		// LAUNCH_CONTINUOUS cmd
				{
					// 连发突然切换单发要清除标志位			2024.07.23
					Launch->flag->DIAL_ANGLE_CHANGE_FLAG = false ;	// 准备下一次单发
				
					#if LAUNCH_CONTINUOUS_MODE == 0
					Launch->info->dial_speed_target = Launch->conf->dial_speed_config;
					Dial_Speed_Ctrl(Launch);
					#endif
					
					#if LAUNCH_CONTINUOUS_MODE == 1
					if(Launch->flag->DIAL_ANGLE_CHANGE_FLAG == false)
					{
							Launch->info->dial_angle_target = \
							Launch->dev->dial->rx_info.angle_sum + LAUNCH_DIAL_ANGLE_SET ;
							
							Launch->flag->DIAL_ANGLE_CHANGE_FLAG = true ;
					}
					
					if(	Launch->dev->dial->rx_info.angle_sum <= \
							Launch->info->dial_angle_target + LAUNCH_DIAL_ANGLE_OK
					&& 	Launch->dev->dial->rx_info.angle_sum >= \
							Launch->info->dial_angle_target - LAUNCH_DIAL_ANGLE_OK)
							Launch->flag->DIAL_ANGLE_CHANGE_FLAG = false ;

					Dial_Position_Ctrl(Launch);
					#endif
				}	//	-------------------- 单发 begin ---------------------- //
				else if(Launch->cmd->LAUNCH_SINGLE == true)			// LAUNCH_SINGLE cmd
				{		
					if(Launch->flag->DIAL_ANGLE_CHANGE_FLAG == false)
					{
//						if((Launch->cnt->vis_shoot_cnt <= 0) && (Launch->flag->VIS_CTRL_FLAG != true))	// 单发指令
//						if(Launch->cnt->vis_shoot_cnt <= 0)	// 单发指令
						if(Launch->flag->VIS_CTRL_FLAG == false)
						{
							Launch->info->dial_angle_target = \
							Launch->dev->dial->rx_info.angle_sum + LAUNCH_DIAL_ANGLE_SET;
							
							Launch->flag->DIAL_ANGLE_CHANGE_FLAG = true ;
						}
						else if(Launch->cnt->vis_shoot_cnt > 0)					// 视觉发弹量
						{
							Launch->info->dial_angle_target = \
							Launch->dev->dial->rx_info.angle_sum + LAUNCH_DIAL_ANGLE_SET * Launch->cnt->vis_shoot_cnt;
							
							Launch->flag->DIAL_ANGLE_CHANGE_FLAG = true ;
//							Launch->cnt->vis_shoot_cnt = 0;							// 联盟赛的傻逼代码
						}
					}
					
					if(	Launch->dev->dial->rx_info.angle_sum <= \
							Launch->info->dial_angle_target + LAUNCH_DIAL_ANGLE_OK
					&&	Launch->dev->dial->rx_info.angle_sum >= \
							Launch->info->dial_angle_target - LAUNCH_DIAL_ANGLE_OK)
					{
						Launch->cmd->LAUNCH_SINGLE = false;							// 结束单发状态
						Launch->flag->DIAL_ANGLE_CHANGE_FLAG = false ;	// 准备下一次单发
						Launch->cnt->vis_shoot_cnt = 0;
						
						
						// 检测双发
						if(Launch->flag->VIS_CTRL_FLAG == false || Launch->cnt->vis_shoot_cnt == 1)
						{
							Launch->cnt->single_cnt = Launch->cnt->judge_measure_shoot_cnt;
							
							if(Launch->cnt->single_cnt - Launch->cnt->pre_single_cnt >= 2)
								Launch->flag->TWIN_SHOOT_FLAG = true;
						}
						Launch->cnt->pre_single_cnt = Launch->cnt->judge_measure_shoot_cnt;
						// 检测双发
					}

					Dial_Position_Ctrl(Launch);
				}
				//	-------------------- 单发 end  ---------------------- //
				else
				{
					Launch->flag->DIAL_ANGLE_CHANGE_FLAG = false;		// 2024.05.18
					// 无发弹指令，为下一次发弹做准备
					// 视觉控制打弹。发弹量为 0 时，进入此。
					
					Launch->info->dial_speed_target = 0;
					Dial_Speed_Ctrl(Launch);
				}
			}
			else		// 堵转处理
			{
				Launch->flag->DIAL_ANGLE_CHANGE_FLAG = false ;	// 清除标志位
				
				Launch->cnt->dial_stuck_deal_cnt ++;
		
				Launch->info->dial_speed_target = LAUNCH_DIAL_STUCK_DEAL_SPEED;
				Dial_Speed_Ctrl(Launch);
				
				if(Launch->cnt->dial_stuck_deal_cnt >= LAUNCH_DIAL_STUCK_DEAL_CNT)
				{
					Launch->cnt->dial_stuck_deal_cnt 	= 0;
					Launch->flag->DIAL_STUCK_FLAG 		= false;
				}
			}
			
			Launch_Dial_Locked_Check(Launch);
			
			break;
		
		default:
			break;
	}
}

void Launch_Dial_Locked_Check(Launch_t* Launch)
{
	motor_t* Dial = Launch->dev->dial;
	
	if(Launch->cmd->LAUNCH_SINGLE == true)
	{
		if(Dial->c_stuck_flag(Dial, 2, LAUNCH_DIAL_STUCK_ANGLE, LAUNCH_DIAL_STUCK_CHECK_CNT))
			Launch->flag->DIAL_STUCK_FLAG = true;
	}
	else if(Launch->cmd->LAUNCH_CONTINUOUS == true || Launch->cmd->LAUNCH_CLEAN_HEAT == true)
	{
		if(Dial->c_stuck_flag(Dial, 1, LAUNCH_DIAL_STUCK_SPEED, LAUNCH_DIAL_STUCK_CHECK_CNT))
			Launch->flag->DIAL_STUCK_FLAG = true;
	}
}
/*--------------- work functions end   ---------------*/

/*--------------- test functions begin ---------------*/
void Fric_Measure_Cnt(Launch_t* Launch)
{
	// 摩擦轮掉速检测
	if(	Launch->dev->l_fc->pid.speed.info.err < -200.f &&
			Launch->dev->r_fc->pid.speed.info.err < -200.f &&
			Launch->flag->FRIC_CHECK_CNT_FLAG == true)
	{
		Launch->flag->FRIC_CHECK_CNT_FLAG = false;
		Launch->cnt->fric_measure_shoot_cnt ++;
	}
	
	if(	abs(Launch->dev->l_fc->pid.speed.info.err) < 200 &&
			abs(Launch->dev->r_fc->pid.speed.info.err) < 200 )
	{
		Launch->cnt->fric_stable_cnt ++;
		
		if(Launch->cnt->fric_stable_cnt > LAUNCH_STABLE_CHECK_TIME)
		{
			Launch->flag->FRIC_CHECK_CNT_FLAG = true;					// 可以检测下一发弹丸
			Launch->cnt->fric_stable_cnt = 0;
		}
	}
	
	// 拨盘旋转角度检测
	Launch->cnt->dial_measure_shoot_cnt = Launch->dev->dial->rx_info.angle_sum / LAUNCH_DIAL_ANGLE_SET;
	
	// 裁判系统弹速检测
	// 在 module.c Judge_Launch_Ctrl
	
	// 裁判系统剩余发弹量检测
	
	// 拨盘角度矫正
	// if(... ...)			dial_target_position ->
	// Launch->cnt->dial_measure_shoot_cnt = 0;
	// Launch->cnt->fric_measure_shoot_cnt = 0;
	// Launch->cnt->judge_measure_shoot_cnt = 0;
	
}
/*--------------- test functions end   ---------------*/

/*---PID计算函数begin---*/
void Fric_Speed_Ctrl(Launch_t* Launch)
{
	motor_t *motor_l = Launch->dev->l_fc;
	motor_t *motor_r = Launch->dev->r_fc;
	
	Launch->info->l_fc_speed_err = Launch->info->l_fc_speed_measure - Launch->info->l_fc_speed_target;
	Launch->info->r_fc_speed_err = Launch->info->r_fc_speed_measure - Launch->info->r_fc_speed_target;
	
	if(abs(Launch->info->l_fc_speed_err)	>	50.f || abs(Launch->info->r_fc_speed_err)	>	50.f)
	{
		motor_l->pid.speed.set	=	Launch->conf->fric_swift_pid;
		motor_r->pid.speed.set	=	Launch->conf->fric_swift_pid;
	}
	else
	{
		motor_l->pid.speed.set	=	Launch->conf->fric_speed_pid;
		motor_r->pid.speed.set	=	Launch->conf->fric_speed_pid;
	}
	
	Launch->info->l_fc_motor_set = motor_l->c_speed(motor_l,Launch->info->l_fc_speed_target);
	Launch->info->r_fc_motor_set = motor_r->c_speed(motor_r,Launch->info->r_fc_speed_target);
}

void Dial_Position_Ctrl(Launch_t* Launch)
{
	motor_t* motor = Launch->dev->dial;
	
	Launch->info->dial_motor_set = motor->c_posit(motor,Launch->info->dial_angle_target);
}

void Dial_Speed_Ctrl(Launch_t* Launch)
{
	motor_t* motor = Launch->dev->dial;
	
	float err = Launch->dev->dial->rx_info.speed - Launch->info->dial_speed_target;
	
	if(abs(err)	>	800.f)
		motor->pid.speed.set	=	Launch->conf->dial_swift_pid;
	else
		motor->pid.speed.set	=	Launch->conf->dial_speed_pid;
	
	Launch->info->dial_motor_set = motor->c_speed(motor,Launch->info->dial_speed_target);
}
/*---PID计算函数 end ---*/
/*---------------函数定义end---------------*/
