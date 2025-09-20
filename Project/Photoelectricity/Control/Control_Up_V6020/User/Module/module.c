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

// �ڲ�����
static void Robot_Decision_Set(Robot_t* robot);		//	״̬��ģʽ�л�
static void Robot_Vision_Ctrl(Robot_t* robot);		//	ʶ��Ŀ������		��ʧĿ������
static void Robot_Find_Target(Robot_t* Robot);		//	δʶ��Ŀ��Ѳ��
static void Robot_Find_Heartbeat(Robot_t* Robot);	//	ʶ��Ŀ���־λ����
static void Robot_Buzzer_Ctrl(Robot_t* Robot);		//	����������		�Ӿ���־λ�����ش���		����������
static void Robot_Move_Body(Robot_t* Robot);			//	�����ߺ�δʶ��Ŀ������ƶ�
static void Robot_Strategy_Ctrl(Robot_t* Robot);	//	��ͬ���߹����ߺ�Ŀ���


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
		
		// ������
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
	// ��̨���Ӿ�������
	static uint32_t time_cnt;
//	static uint8_t chassis_forward = 0;
	
	switch(Robot->info.stage)
	{
		case GAME_INIT:					//	1
			time_cnt ++;
			if(time_cnt > 1000)		//	�������ϵ�ʱ��
			{
				time_cnt = 0;
				Robot->info.stage = GAME_PREPARE;
			}
			
			Robot->info.strategy_flag = 0;
			Robot->info.strategy_3_step = 0;
			
			ctrl.up_info.bit_control.chassis_switch = 0;
			ctrl.up_info.bit_control.chassis_begin  = 0;
			break;
	
		case GAME_PREPARE :			//	2		�ϵ��δ���°���
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
				ctrl.up_info.bit_control.winner_flag = 1;		//	�������·�������0.1s�����жϰ�������
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
			if(time_cnt >= 1000*1.5f)		//	�����ʱ�Ѿ�Խ������
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
			
			ctrl.x_pos = TARGET_X;		//	��λ m
			ctrl.y_pos = TARGET_Y;		//	��λ m
			ctrl.a_yaw = 0.f;
			break;
		
		case GAME_OVER_MID:			//	��ʱ�Ѿ�Խ�����ߣ��������һ
			if(vision.work_state == DEV_ONLINE)
			{
				Robot_Find_Heartbeat(Robot);
				Robot_Vision_Ctrl(Robot);
			}
			Robot_Find_Target(Robot);		//	Ѳ��
			
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
			
			// ״̬�л� 1
			static uint8_t reset_cnt;
			if(Robot->info.key_strategy == 0)
				reset_cnt ++;
			else
				reset_cnt = 0;
			if(reset_cnt > 250)
			{
				reset_cnt = 0;
				Robot->info.stage = GAME_INIT;	//	���̳�ʼ��
			}
			// ״̬�л� 2
			time_cnt ++;
			if(time_cnt >= 1000*60*6)	//	360s 6min
			{
				time_cnt = 0;
				Robot->info.stage = GAME_FINAL;
			}
			
			break;
			
		case GAME_FINAL:			//	��������		OR		����ģʽ
			Robot_Find_Heartbeat(Robot);
			if(vision.work_state == DEV_ONLINE)
				Robot_Vision_Ctrl(Robot);

			vision.info->tx_info->game_start = 1;
			
			ctrl.up_info.bit_control.chassis_switch = 0;
			Robot_Buzzer_Ctrl(Robot);
			
			// ״̬�л� 1
			if(Robot->info.key_strategy == 0)
				reset_cnt ++;
			else
				reset_cnt = 0;
			if(reset_cnt > 250)
			{
				reset_cnt = 0;
				Robot->info.stage = GAME_INIT;	//	���̳�ʼ��
			}
			break;
			
		default :
			break;
			
	
	
	}

	
}

// ����Ŀ��󱣳ֵ�ǰ�Ƕ� 1s
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

void Robot_Find_Target(Robot_t* Robot)	// Ѳ��ģʽ
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

// �Ӿ���֡��������
void Robot_Find_Heartbeat(Robot_t* Robot)
{
	vision_info_t* Vis_info = Robot->vision->info;
	
	Vis_info->flag_max_cnt = 1000;		//	��֡���� 1 S
	
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

// ����������
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

static void Robot_Move_Body(Robot_t* Robot)		//	�����ߺ�δʶ��Ŀ������ƶ�
{
	uint16_t move_cnt_max = 3000;		//	ÿ���ƶ�ʱ����
	static uint16_t move_cnt = 0;				//	��ֹ��ʱ
	static uint8_t chassis_y_forward = 0;
	
	uint16_t chassis_int_max = 5000;				//	ÿ���ƶ�ʱ����
	static uint16_t chassis_int_cnt;				//	��ֹ��ʱ
	static uint8_t chassis_int_flag = 1;
	
	
	vision_info_t* Vis_info = Robot->vision->info;
	
	if(Vis_info->vision_ctrl_flag == 1)
	{
		//	����ס		//	Խ�������ٵ���
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
					//	����һ
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
					// ���߶�
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
//					//	������
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
			Robot->info.strategy_flag = 1;	//	�������Ѳ��ģʽ
			Strategy_cnt = 0;
			break;
			
		case 1:
			ctrl.y_pos = -1.1f;
			if(Strategy_cnt > 1000)		//	����ʱ��
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
					ctrl.x_pos < 2.f && Strategy_cnt > 5*1000)	//	5s��
			{
				Strategy_cnt = 0;
				ctrl.x_pos = TARGET_X;	//	׼��������߶�
				Robot->info.strategy_3_step = 2;
			}
			
			if(Robot->info.strategy_3_step == 2 && Strategy_cnt > 800)
			{
				Strategy_cnt = 0;
				ctrl.x_pos = TARGET_X;
				Robot->info.strategy = 1;	//	������߶�
			}
			break;
		
		default:
			break;
	
	}

}
