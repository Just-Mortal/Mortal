/* Includes ------------------------------------------------------------------*/
#include "module.h"

#include "my_pid.h"

#define TARGET_X	2.3f
#define TARGET_Y	0.f

pid_info_t pid_yaw_diff =
{
    .kp = 0.05f,
    .ki = 0.f,
    .kd = 0.0f,
		
		.blind_err = 2.2f,
    .integral_max = 0.f,
    .out_max = 5.f,
};

pid_info_t pid_pch_diff =
{
    .kp = 0.015f,
    .ki = 0.0f,
    .kd = 0.0f,
		
		.blind_err = 2.2f,
    .integral_max = 0.0f,
    .out_max = 5.f,
};

static void Robot_Init(Robot_t* Robot);
static void Robot_State_Update(Robot_t* robot);

// 内部函数
static void Robot_Decision_Set(Robot_t* robot);		//	状态机模式切换
static void Robot_Vision_Ctrl(Robot_t* robot);		//	识别目标自瞄		丢失目标锁定
static void Robot_Find_Target(Robot_t* Robot);		//	未识别目标巡逻
static void Robot_Find_Heartbeat(Robot_t* Robot);	//	识别目标标志位补偿
static void Robot_Buzzer_Ctrl(Robot_t* Robot);		//	蜂鸣器控制		视觉标志位上升沿触发		最多持续三秒
static void Robot_Move_Body(Robot_t* Robot);			//	过中线后未识别到目标横向移动
static void Robot_Strategy_Ctrl(Robot_t* Robot);	//	不同决策过中线后的控制


Robot_t robot = 
{
    .gimbal = &gimbal,
    .vision = &vision,
    
    .init = Robot_Init,
    .state_update = Robot_State_Update,
};



void Robot_Init(Robot_t* Robot)
{
    // Initialize the gimbal and vision systems
//    Gimbal_Init(&robot->gimbal);
//    Vision_Init(&robot->vision);

    Robot->gimbal->init(Robot->gimbal);

    // Set the initial state of the robot
//    Robot->state = ROBOT_INIT;
		
		// 比赛用
		Robot->info.stage = GAME_INIT;
} 

void Robot_State_Update(Robot_t* Robot)
{
	#if 0
    static uint8_t last_game_flag = 0;

    if (robot->info.game_flag != last_game_flag)
    {
        robot->state = ROBOT_INIT;
        return;
    }

    switch (robot->state)
    {
        case ROBOT_INIT:
//            robot->gimbal.mode = GIM_MEC;
						robot->gimbal->mode = GIM_IMU;
            break;

        case ROBOT_IMU:
            if (robot->info.init_ok_flag == true)
                robot->gimbal->mode = GIM_IMU;
            break;

        case ROBOT_SLEEP:
            robot->gimbal->mode = GIM_SLEEP;
            break;

        default:
            break;
    }
		#endif
		
//		static uint32_t game_before_cnt;
		
	Robot->gimbal->mode = GIM_IMU;
	Robot->info.last_key_state = Robot->info.key_state;
//	Robot->info.key_state = HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin);
	Robot->info.key_state = HAL_GPIO_ReadPin(KEY_R_GPIO_Port, KEY_R_Pin);
	
	Robot->info.last_key_strategy = Robot->info.key_strategy;
	Robot->info.key_strategy = HAL_GPIO_ReadPin(KEY_G_GPIO_Port, KEY_G_Pin);
	if(Robot->info.last_key_strategy == 1 && Robot->info.key_strategy == 0)
		Robot->info.strategy ++;
	Robot->info.strategy %= 3;
	
	Robot_Decision_Set(Robot);
	
	
	
//    last_game_flag = robot->info.game_flag;
}

void Robot_Decision_Set(Robot_t* Robot)
{
	// 云台、视觉、底盘
	static uint32_t time_cnt;
//	static uint8_t chassis_forward = 0;
	
	switch(Robot->info.stage)
	{
		case GAME_INIT:					//	1
			time_cnt ++;
			if(time_cnt > 1000)		//	下主控上电时间
			{
				time_cnt = 0;
				Robot->info.stage = GAME_PREPARE;
			}
			
			Robot->info.strategy_flag = 0;
			Robot->info.strategy_3_step = 0;
			
			ctrl.up_info.bit_control.chassis_switch = 0;
			ctrl.up_info.bit_control.chassis_begin  = 0;
			break;
	
		case GAME_PREPARE :			//	2		上电后未按下按键
			if(Robot->info.last_key_state ==1 && Robot->info.key_state == 0)
				Robot->info.stage = GAME_KEY_PRESS;
			
			if(vision.work_state == DEV_ONLINE)
			{
				Robot_Find_Heartbeat(Robot);
				Robot_Vision_Ctrl(Robot);
			}
			
			vision.info->tx_info->game_start = 0;
			
			ctrl.up_info.bit_control.chassis_switch = 1;
			ctrl.up_info.bit_control.chassis_begin  = 1;
			ctrl.up_info.bit_control.winner_flag		=	0;
			
			ctrl.x_pos = 0.f;
			ctrl.y_pos = 0.f;
			ctrl.a_yaw = 0.f;
			
			break;
			
		case GAME_KEY_PRESS:		//	3
			time_cnt ++;
			if(time_cnt <= 100)
				ctrl.up_info.bit_control.winner_flag = 1;		//	按键按下蜂鸣器响0.1s用于判断按键按下
			else
			{
				time_cnt = 0;
				ctrl.up_info.bit_control.winner_flag = 0;
				Robot->info.stage = GAME_START;
			}
			
			if(vision.work_state == DEV_ONLINE)
			{
				Robot_Find_Heartbeat(Robot);
				Robot_Vision_Ctrl(Robot);
			}
			
			vision.info->tx_info->game_start = 0;
			
			ctrl.up_info.bit_control.chassis_switch = 1;
			ctrl.up_info.bit_control.chassis_begin  = 1;
			
			ctrl.x_pos = TARGET_X;
			ctrl.y_pos = TARGET_Y;
			ctrl.a_yaw = 0.f;
			break;
			
		case GAME_START:				//	4
			time_cnt ++;
			if(time_cnt >= 1000*1.5f)		//	假设此时已经越过中线
			{
				time_cnt = 0;
				Robot->info.stage = GAME_OVER_MID;
			}
			
			// -------------------------- 
			if(vision.work_state == DEV_ONLINE)
			{
				Robot_Find_Heartbeat(Robot);
				Robot_Vision_Ctrl(Robot);
			}
			
			vision.info->tx_info->game_start = 0;
			ctrl.up_info.bit_control.chassis_switch = 1;
			ctrl.up_info.bit_control.chassis_begin  = 1;
			ctrl.up_info.bit_control.winner_flag = 0;
			
			ctrl.x_pos = TARGET_X;		//	单位 m
			ctrl.y_pos = TARGET_Y;		//	单位 m
			ctrl.a_yaw = 0.f;
			break;
		
		case GAME_OVER_MID:			//	此时已经越过中线！到坐标点一
			if(vision.work_state == DEV_ONLINE)
			{
				Robot_Find_Heartbeat(Robot);
				Robot_Vision_Ctrl(Robot);
			}
			Robot_Find_Target(Robot);		//	巡逻
			
			Robot_Buzzer_Ctrl(Robot);
			if(Robot->info.strategy_flag == 0)
				Robot_Strategy_Ctrl(Robot);
			else if(Robot->info.strategy_flag == 1)
				Robot_Move_Body(Robot);
	
			
			vision.info->tx_info->game_start = 1;
			ctrl.up_info.bit_control.chassis_switch = 1;
			ctrl.up_info.bit_control.chassis_begin  = 1;
			
//			ctrl.x_pos = TARGET_X;
//			ctrl.y_pos = TARGET_Y;
			ctrl.a_yaw = 0.f;
			
			// 状态切换 1
			static uint8_t reset_cnt;
			if(Robot->info.key_strategy == 0)
				reset_cnt ++;
			else
				reset_cnt = 0;
			if(reset_cnt > 250)
			{
				reset_cnt = 0;
				Robot->info.stage = GAME_INIT;	//	底盘初始化
			}
			// 状态切换 2
			time_cnt ++;
			if(time_cnt >= 1000*60*6)	//	360s 6min
			{
				time_cnt = 0;
				Robot->info.stage = GAME_FINAL;
			}
			
			break;
			
		case GAME_FINAL:			//	比赛结束		OR		测试模式
			Robot_Find_Heartbeat(Robot);
			if(vision.work_state == DEV_ONLINE)
				Robot_Vision_Ctrl(Robot);

			vision.info->tx_info->game_start = 1;
			
			ctrl.up_info.bit_control.chassis_switch = 0;
			Robot_Buzzer_Ctrl(Robot);
			
			// 状态切换 1
			if(Robot->info.key_strategy == 0)
				reset_cnt ++;
			else
				reset_cnt = 0;
			if(reset_cnt > 250)
			{
				reset_cnt = 0;
				Robot->info.stage = GAME_INIT;	//	底盘初始化
			}
			break;
			
		default :
			break;
			
	
	
	}

	
}

// 丢掉目标后保持当前角度 1s
void Robot_Vision_Ctrl(Robot_t* robot)
{
	if(vision.info->rx_info->find_target == 1)
	{
			pid_yaw_diff.measure = vision.info->x_diff;
			pid_pch_diff.measure = vision.info->y_diff;
	
			single_pid_cal(&pid_yaw_diff);
			single_pid_cal(&pid_pch_diff);
			robot->info.yaw_imu_diff = pid_yaw_diff.out;
			robot->info.pch_imu_diff = pid_pch_diff.out;

			gimbal.info->target->yaw_imu_angle = gimbal.info->measure->yaw_imu_angle + robot->info.yaw_imu_diff;
			gimbal.info->target->pch_imu_angle = gimbal.info->measure->pch_imu_angle + robot->info.pch_imu_diff;
	}
	else if(vision.info->vision_ctrl_flag == 1)
	{
			gimbal.info->target->yaw_imu_angle = gimbal.info->measure->yaw_imu_angle;
			gimbal.info->target->pch_imu_angle = gimbal.info->measure->pch_imu_angle;
	}
	else
	{
			pid_yaw_diff.measure = 0;
			pid_pch_diff.measure = 0;
	}
}

void Robot_Find_Target(Robot_t* Robot)	// 巡逻模式
{
	float now_yaw = gimbal.info->measure->yaw_imu_angle;
	float ramp, mec_yaw = gimbal.info->measure->yaw_mec_angle;

	if(vision.info->vision_ctrl_flag == 0 || vision.work_state == DEV_OFFLINE)
	{
		if(mec_yaw > -3150 && mec_yaw < -2150)
			gimbal.info->target->yaw_imu_angle = now_yaw + 1.f;
		else if(mec_yaw > -2150 && mec_yaw < -1150)
		{
			ramp = 1.f + (mec_yaw+2150.f) / 1000.f * 10.f;
			gimbal.info->target->yaw_imu_angle = now_yaw + ramp;
		}
		else if(mec_yaw > -4050 && mec_yaw < -3150)
		{
			ramp = 1.f - (mec_yaw+3150.f) / 900.f * 10.f;
			gimbal.info->target->yaw_imu_angle = now_yaw + ramp;
		}
		else
			gimbal.info->target->yaw_imu_angle = now_yaw + 11.f;
		
		
		gimbal.info->target->pch_imu_angle = -2.f;
	}
}

// 视觉掉帧补偿控制
void Robot_Find_Heartbeat(Robot_t* Robot)
{
	vision_info_t* Vis_info = Robot->vision->info;
	
	Vis_info->flag_max_cnt = 1000;		//	掉帧补偿 1 S
	
	if(Vis_info->rx_info->find_target == 1)
	{
		Vis_info->flag_cnt = 0;		
		
		Vis_info->vision_ctrl_flag = 1;
	}
	else if(Vis_info->rx_info->find_target == 0)
	{
		Vis_info->flag_cnt ++;
		
		if(Vis_info->flag_cnt > Vis_info->flag_max_cnt)
		{
			Vis_info->flag_cnt = Vis_info->flag_max_cnt;
		
			Vis_info->vision_ctrl_flag = 0;
		}
	}
}

// 蜂鸣器控制
void Robot_Buzzer_Ctrl(Robot_t* Robot)
{
	static uint8_t last_winner_flag;
	static uint16_t winner_cnt;
	
	if(last_winner_flag == 0 && vision.info->rx_info->winner_flag == 1)
		ctrl.up_info.bit_control.winner_flag = 1;
	
	if(ctrl.up_info.bit_control.winner_flag == 1)
	{
		winner_cnt ++;
		
		if(winner_cnt > 3000)
			ctrl.up_info.bit_control.winner_flag = 0;
	}
	else
		winner_cnt = 0;

	last_winner_flag = vision.info->rx_info->winner_flag;
}

static void Robot_Move_Body(Robot_t* Robot)		//	过中线后未识别到目标横向移动
{
	uint16_t move_cnt_max = 3000;		//	每次移动时间间隔
	static uint16_t move_cnt = 0;				//	静止计时
	static uint8_t chassis_y_forward = 0;
	
	uint16_t chassis_int_max = 5000;				//	每次移动时间间隔
	static uint16_t chassis_int_cnt;				//	静止计时
	static uint8_t chassis_int_flag = 1;
	
	
	vision_info_t* Vis_info = Robot->vision->info;
	
	if(Vis_info->vision_ctrl_flag == 1)
	{
		//	立马定住		//	越过中线再调用
		ctrl.x_pos = ctrl.x_pos;
		ctrl.y_pos = ctrl.y_pos;
		
		move_cnt = 0;
	}
	else
	{
		move_cnt ++;
		
		if(move_cnt > move_cnt_max)
		{
			move_cnt = 0;
		
			if(chassis_int_flag == 1)
			{
			
				if(Robot->info.strategy == 0)
				{
					//	决策一
					if(chassis_y_forward == 0)
					{
						ctrl.y_pos -= 1.f;
						chassis_y_forward = 1;
					}
					else if(chassis_y_forward == 1)
					{
						ctrl.y_pos += 1.f;
						chassis_y_forward = 0;
					}
				}
				else if(Robot->info.strategy == 1)
				{
					// 决策二
					if(chassis_y_forward == 0)
					{
						ctrl.y_pos += 1.f;
						chassis_y_forward = 1;
					}
					else if(chassis_y_forward == 1)
					{
						ctrl.y_pos -= 1.f;
						chassis_y_forward = 0;
					}
				}
//				else
//				{
//					//	决策三
//					if(chassis_y_forward == 0)
//					{
//						ctrl.x_pos -= 1.4f;
//						chassis_y_forward = 1;
//					}
//					else if(chassis_y_forward == 1)
//					{
//						ctrl.x_pos += 1.4f;
//						chassis_y_forward = 0;
//					}
//					if(ctrl.x_pos > 2.4f)		ctrl.x_pos = 2.4f;
//					if(ctrl.x_pos < 0.5f)		ctrl.x_pos = 0.5f;
//				}
				
				chassis_int_flag = 0;
			}
		}
		
		if(ctrl.y_pos > 0.f)		ctrl.y_pos = 0.f;
		if(ctrl.y_pos < -1.2f)		ctrl.y_pos = -1.2f;
	}
	

	if(chassis_int_flag == 0)
	{
		chassis_int_cnt ++;
		if(chassis_int_cnt > chassis_int_max)
		{
			chassis_int_flag = 1;
			chassis_int_cnt = 0;
		}
	}
	else
		chassis_int_cnt = 0;
}


void Robot_Strategy_Ctrl(Robot_t* Robot)
{
	static uint16_t	Strategy_cnt;
//	uint8_t strategy_3 = Robot->info.strategy_3_step;

	uint8_t stragegy = Robot->info.strategy;

	Strategy_cnt ++;
	
	switch(stragegy)
	{
		case 0:
			Robot->info.strategy_flag = 1;	//	立马进入巡逻模式
			Strategy_cnt = 0;
			break;
			
		case 1:
			ctrl.y_pos = -1.1f;
			if(Strategy_cnt > 1000)		//	左移时间
			{
				Strategy_cnt = 0;
				Robot->info.strategy_flag = 1;
			}
			break;
			
		case 2:
			if(	Robot->info.strategy_3_step == 0 && \
					ctrl.x_pos > 2.f && Strategy_cnt > 800)
			{
				Strategy_cnt = 0;
				
				ctrl.x_pos = +1.8f;
				Robot->info.strategy_3_step = 1;
			}
			
			
			if(	Robot->info.strategy_3_step == 1 && \
					ctrl.x_pos < 2.f && Strategy_cnt > 5*1000)	//	5s内
			{
				Strategy_cnt = 0;
				ctrl.x_pos = TARGET_X;	//	准备进入决策二
				Robot->info.strategy_3_step = 2;
			}
			
			if(Robot->info.strategy_3_step == 2 && Strategy_cnt > 800)
			{
				Strategy_cnt = 0;
				ctrl.x_pos = TARGET_X;
				Robot->info.strategy = 1;	//	进入决策二
			}
			break;
		
		default:
			break;
	
	}

}
