/* Includes ------------------------------------------------------------------*/
#include "infantry.h"

// 操作指令，输出函数为 Infantry_Status_Update
extern uint8_t chas_spin_cnt;
/* Private function prototypes -----------------------------------------------*/
static void Operator_Cmd_Update(Infantry_t* Infantry);

static void Key_Update(Infantry_t* Infantry);
static void Rc_S2_Update(Infantry_t* Infantry);
static void Rc_S1_Update(Infantry_t* Infantry);
static void Rc_Tb_Update(Infantry_t* Infantry);
static void Key_Tb_Update(Infantry_t* Infantry);
static void Rc_Open_Lock(Infantry_t*	Infantry);
static void	Key_Q_Status_Check(Infantry_t* Infantry);
static void Key_E_Status_Check(Infantry_t* Infantry);
static void Key_R_Status_Check(Infantry_t* Infantry);
static void Key_F_Status_Check(Infantry_t* Infantry);
static void Key_G_Status_Check(Infantry_t* Infantry);
static void Key_Z_Status_Check(Infantry_t* Infantry);
static void Key_X_Status_Check(Infantry_t* Infantry);
static void Key_C_Status_Check(Infantry_t* Infantry);
static void Key_V_Status_Check(Infantry_t* Infantry);
static void Key_B_Status_Check(Infantry_t* Infantry);
static void Key_CTRL_Status_Check(Infantry_t* Infantry);
static void Key_SHIFT_Status_Check(Infantry_t* Infantry);
static void Key_MOUSE_L_Status_Check(Infantry_t* Infantry);
static void Key_MOUSE_R_Status_Check(Infantry_t* Infantry);
static void Key_Infantry_Reset_Check(Infantry_t* Infantry);

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  整车状态更新
  * @param  Infantry_t* Infantry
  * @retval None
  */
void Infantry_Status_Update(Infantry_t* Infantry)
{
	Infantry_flag_t* 	Flag 	= Infantry->flag ;
	rc_t*							RC		=	Infantry->rc_module;
	
	if(Flag->INFANTRY_INIT_FLAG != true && RC->dev->work_state == DEV_ONLINE)
	{
		Infantry->mode 						= INIT_MODE;	// 开控唤醒 Infantry
		Flag->INFANTRY_INIT_FLAG	= true;
	}
	else
	{
		if(RC->dev->work_state == DEV_ONLINE)		// 正常指令更新
		{
			RC->update();
			Operator_Cmd_Update(Infantry);
		}
		else																		// 遥控失联
		{
			Infantry->mode 						=	SLEEP_MODE;
			Infantry->ctrl 						= RC_CTRL;
			Flag->INFANTRY_INIT_FLAG	= false;
			
			Infantry->cmd->CAP_OPEN_CMD	=	false;
			
			Flag->KEY_CTRL_INIT_FLAG	= false;
			Flag->RC_CTRL_INIT_FLAG		= false;
			Flag->TEST_VIS_CTRL_FLAG	= false;
			Flag->VIS_CTRL_GIMBAL_FLAG= false;
			Flag->VIS_CTRL_LAUNCH_FLAG= false;
			Flag->GIM_BUFF_MODE_FLAG	= false;
			Flag->LAUNCH_HIGH_HEAT_FLAG=false;
			
			Infantry->cmd->VIS_OUTPOST_CMD			=	false;
			Infantry->cmd->VIS_FAST_BUFF_CMD		= false;
			Infantry->cmd->VIS_LARGE_BUFF_CMD 	= false;
			Infantry->cmd->VIS_SMALL_BUFF_CMD 	= false;
		}
	}
}


void Operator_Cmd_Update(Infantry_t* Infantry)
{
	Rc_Open_Lock(Infantry);
	Rc_S2_Update(Infantry);	
	Rc_S1_Update(Infantry);
	
	switch(Infantry->ctrl)
	{
		case RC_CTRL:
			Infantry->flag->KEY_CTRL_INIT_FLAG = false;
			Rc_Tb_Update(Infantry);
			break;
			
		case KEY_CTRL:
			Key_Update(Infantry);
			Key_Tb_Update(Infantry);
			break;
			
		default:
			break;
	}
}

void Rc_Open_Lock(Infantry_t*	Infantry)
{
	rc_t*	RC	=	Infantry->rc_module;

	// 关控底盘和发射机构上锁
	if(Infantry->flag->RC_CTRL_INIT_FLAG == false)
	{
		Infantry->flag->RC_CTRL_INIT_FLAG 			= true;
		
		Infantry->flag->LAUNCH_OPEN_LOCK_FLAG		= true;
		Infantry->flag->CHAS_OPEN_LOCK_FLAG			=	true;
	}
	
	// 左右摇杆值未全部归零，底盘上锁
	if(RC->info->CH0.val == 0 && RC->info->CH1.val == 0 && \
		 RC->info->CH2.val == 0 && RC->info->CH3.val == 0 )
		Infantry->flag->CHAS_OPEN_LOCK_FLAG			=	false;
	
	// 遥控模式下左拨杆未跳变，发射机构上锁
	if(RC->info->SW1.flip != SW_NONE_FLIP)
		Infantry->flag->LAUNCH_OPEN_LOCK_FLAG		= false;
}

void Rc_S2_Update(Infantry_t* Infantry)
{
	rc_t*				RC			=	Infantry->rc_module;
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;
	Launch_t*		Launch	= Infantry->launch_module;
	
	switch(RC->info->SW2.state)
	{
		case RC_SW_UP:
			Infantry->ctrl	= KEY_CTRL;
		
			if(Gimbal->flag->GIMBAL_INIT_OK_FLAG == false)
				Gimbal->mode	=	GIM_MEC;
			else
			{
				if(Infantry->flag->KEY_CTRL_INIT_FLAG == false)
				{
					Infantry->mode 		= IMU_MODE;
					Gimbal->mode			= GIM_IMU;
					Launch->fric_mode = FRIC_STOP;
					
					Launch->cmd->LAUNCH_SINGLE 					=	false;
					Launch->cmd->LAUNCH_CONTINUOUS			=	false;
					Launch->cmd->LAUNCH_CLEAN_HEAT			=	false;
					Infantry->cmd->CAP_OPEN_CMD					=	false;
					Infantry->cmd->GIM_AIM_ON_CMD				= false;
					Infantry->cmd->CHAS_SPIN_TOP_CMD 		= false;
					Infantry->cmd->VIS_OUTPOST_CMD			=	false;
					Infantry->cmd->VIS_FAST_BUFF_CMD		= false;
					Infantry->cmd->VIS_LARGE_BUFF_CMD 	= false;
					Infantry->cmd->VIS_SMALL_BUFF_CMD 	= false;
					Infantry->cmd->TEST_GIM_AIM_ON_CMD	=	false;
					Infantry->flag->TEST_VIS_CTRL_FLAG	=	false;
					Infantry->flag->VIS_CTRL_LAUNCH_FLAG= false;
					Infantry->flag->GIM_BUFF_MODE_FLAG  = false;
	
					Infantry->flag->KEY_CTRL_INIT_FLAG 	= true;
				}
			}
			break;
		
		case RC_SW_MID:
			Infantry->ctrl	= RC_CTRL;
			Infantry->mode 	= IMU_MODE;
			
			if(Gimbal->flag->GIMBAL_INIT_OK_FLAG == true)
				Gimbal->mode	=	GIM_IMU;
			else
				Gimbal->mode	=	GIM_MEC;
			
			#if RC_CAP_SWITCH == 1
			Infantry->cmd->CAP_OPEN_CMD					=	true;
			#endif
			#if RC_CAP_SWITCH == 0
			Infantry->cmd->CAP_OPEN_CMD					=	false;
			#endif
			
			Infantry->cmd->GIM_AIM_ON_CMD				= true;
			Infantry->flag->KEY_CTRL_INIT_FLAG	= false;
			break;
		
		case RC_SW_DOWN:
			Infantry->ctrl	= RC_CTRL;
			Infantry->mode	=	MEC_MODE;
			Gimbal->mode		=	GIM_MEC;
			
			#if RC_CAP_SWITCH == 1
			Infantry->cmd->CAP_OPEN_CMD					=	true;
			#endif
			#if RC_CAP_SWITCH == 0
			Infantry->cmd->CAP_OPEN_CMD					=	false;
			#endif
			
			Infantry->cmd->GIM_AIM_ON_CMD				= false;
			Infantry->cmd->CHAS_SPIN_TOP_CMD 		= false;
			Infantry->flag->VIS_CTRL_GIMBAL_FLAG= false;
			Infantry->flag->KEY_CTRL_INIT_FLAG	= false;
			break;
		
		default:
			break;
	}
}

void Rc_S1_Update(Infantry_t*	Infantry)
{
	Launch_t*	Launch = Infantry->launch_module;

	if(Infantry->ctrl == RC_CTRL)
	{
		switch(Infantry->rc_module->info->SW1.state)
		{
			case RC_SW_UP:
				Launch->fric_mode	=	FRIC_SPIN;
				break;
			
			case RC_SW_MID:
				Launch->fric_mode	=	FRIC_SPIN;
				break;
			
			case RC_SW_DOWN:
				Launch->fric_mode = FRIC_STOP;
				
				Launch->cmd->LAUNCH_SINGLE			=	false;
				Launch->cmd->LAUNCH_CONTINUOUS 	= false;
				break;
			
			default:
				break;
		}
	}

	if(Infantry->rc_module->info->SW1.flip == SW_UP_TO_MID	&&\
		 Infantry->flag->LAUNCH_HIGH_HEAT_FLAG == false)
		Launch->cmd->LAUNCH_SINGLE = true;
}


void Rc_Tb_Update(Infantry_t*	Infantry)
{
	int8_t thumbwheel_filp = Infantry->rc_module->info->THUMBWHEEL.thumbwheel_filp;
	
	Launch_t*		Launch	= Infantry->launch_module;
//	Chassis_t*	Chassis	=	Infantry->chassis_module;
	
	switch(thumbwheel_filp)
	{
		case SW_MID_TO_UP:
			switch(Infantry->mode)
			{
				case IMU_MODE:
					if(!Infantry->cmd->CHAS_SPIN_TOP_CMD)
						chas_spin_cnt ++ ;
					Infantry->cmd->CHAS_SPIN_TOP_CMD = !Infantry->cmd->CHAS_SPIN_TOP_CMD;
					break;
					
				case MEC_MODE:
					Infantry->cmd->GIM_TURN_180_CMD = true;
					break;
				
				default:
					break;
			}
			break;
		
		case SW_MID_TO_DOWN:
			if(Launch->flag->FRIC_SPEED_READY_FLAG == true)
			{
				if(Launch->cmd->LAUNCH_CONTINUOUS == true)
					Launch->cmd->LAUNCH_CONTINUOUS = false;
				else
				{
					if(Infantry->flag->LAUNCH_HIGH_HEAT_FLAG == false)
						Launch->cmd->LAUNCH_CONTINUOUS = true;
				}
			}
			
			break;
		
		default:
			break;
	}
}

void Key_Tb_Update(Infantry_t* Infantry)
{
	rc_t* RC = Infantry->rc_module;
	
	switch(RC->info->SW1.state)
	{
		case RC_SW_UP:
			switch(Infantry->rc_module->info->THUMBWHEEL.thumbwheel_filp)
			{
				case SW_MID_TO_DOWN:
					Infantry->cmd->VIS_OUTPOST_CMD	= !Infantry->cmd->VIS_OUTPOST_CMD;
					break;
					
				case SW_MID_TO_UP:
					Infantry->mode = SLEEP_MODE;
		
					HAL_Delay(10);
					CAN1_Send_1FF_Motor(0, 0, 0, 0);
					CAN1_Send_200_Motor(0, 0, 0, 0);
					CAN2_Send_1FF_Motor(0, 0, 0, 0);
					CAN2_Send_200_Motor(0, 0, 0, 0);
					HAL_Delay(10);
							
					__set_FAULTMASK(1);
					NVIC_SystemReset();
					break;
					
				default:
					break;
			}
			break;
			
		case RC_SW_MID:
			switch(Infantry->rc_module->info->THUMBWHEEL.thumbwheel_filp)
			{
				case SW_MID_TO_DOWN:
					if(Infantry->cmd->VIS_SMALL_BUFF_CMD)
						Infantry->cmd->VIS_SMALL_BUFF_CMD = false;
					else
						Infantry->cmd->VIS_SMALL_BUFF_CMD = true;
					break;
					
				case SW_MID_TO_UP:
					if(Infantry->cmd->VIS_LARGE_BUFF_CMD)
						Infantry->cmd->VIS_LARGE_BUFF_CMD = false;
					else
						Infantry->cmd->VIS_LARGE_BUFF_CMD = true;
					break;
					
				default:
					break;
			}
			break;
			
		case RC_SW_DOWN:
			switch(Infantry->rc_module->info->THUMBWHEEL.thumbwheel_filp)
			{
				case SW_MID_TO_DOWN:
					if(Infantry->flag->TEST_VIS_CTRL_FLAG == true)
						Infantry->flag->TEST_VIS_CTRL_FLAG = false;
					else
						Infantry->flag->TEST_VIS_CTRL_FLAG = true;
					break;
					
				case SW_MID_TO_UP:
					if(Infantry->cmd->TEST_GIM_AIM_ON_CMD == true)
						Infantry->cmd->TEST_GIM_AIM_ON_CMD = false;
					else
						Infantry->cmd->TEST_GIM_AIM_ON_CMD = true;
					break;
					
				default:
					break;
			}
			break;
	}
}

void Key_Update(Infantry_t*	Infantry)
{
	Key_Q_Status_Check(Infantry);
  Key_E_Status_Check(Infantry);
	Key_V_Status_Check(Infantry);
  Key_F_Status_Check(Infantry);
	Key_G_Status_Check(Infantry);
	Key_Z_Status_Check(Infantry);
	Key_X_Status_Check(Infantry);
	Key_C_Status_Check(Infantry);
	Key_R_Status_Check(Infantry);
	Key_B_Status_Check(Infantry);
	Key_MOUSE_L_Status_Check(Infantry);
	Key_MOUSE_R_Status_Check(Infantry);

	Key_CTRL_Status_Check(Infantry);
	Key_SHIFT_Status_Check(Infantry);
	Key_Infantry_Reset_Check(Infantry);
}

void Key_Q_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->Q.flip == PRESS_TO_RELEASE)
			if(Infantry->mode == IMU_MODE)
				Infantry->cmd->GIM_TRUN_LEFT_45_CMD = true;
}

void Key_E_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->E.flip == PRESS_TO_RELEASE)
			if(Infantry->mode == IMU_MODE)
				Infantry->cmd->GIM_TURN_RIGHT_45_CMD = true;
}

void Key_R_Status_Check(Infantry_t* Infantry)
{
	switch(Infantry->rc_module->info->R.state)
	{
		case PRESS:
			Infantry->cmd->LNCH_CLEAN_HEAT_CMD = true;
			break;
		
		case RELEASE:
			Infantry->cmd->LNCH_CLEAN_HEAT_CMD = false;
			break;
			
		default:
			break;
	}
}

void Key_F_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->F.flip == PRESS_TO_RELEASE)
	{
		if(!Infantry->cmd->CHAS_SPIN_TOP_CMD)
			chas_spin_cnt ++ ;
			
		Infantry->cmd->CHAS_SPIN_TOP_CMD = true;
	}
}

void Key_G_Status_Check(Infantry_t* Infantry)
{
	rc_t*			RC			=	Infantry->rc_module;
	Vision_t*	Vision	= Infantry->vision_module;
	
	if(RC->info->MOUSE_R.state == PRESS)
	{
		if(RC->info->G.state == PRESS)
			Vision->info->tx_info->is_blood_first = 1;
		else
			Vision->info->tx_info->is_blood_first = 0;
	}
	else
	{
		if(Infantry->rc_module->info->G.flip == RELEASE_TO_PRESS)
		{
			Infantry->cmd->VIS_OUTPOST_CMD			= true;
			Infantry->launch_module->fric_mode	= FRIC_SPIN;
		}
	}
}

void Key_B_Status_Check(Infantry_t* Infantry)
{
	Launch_t*		Launch	= Infantry->launch_module;
	
	if(Infantry->rc_module->info->B.state == PRESS)
	{
		Launch->fric_mode									= FRIC_STOP;
		Launch->cmd->LAUNCH_SINGLE 				= false;
		Launch->cmd->LAUNCH_CONTINUOUS 		= false;
	}
}

void Key_V_Status_Check(Infantry_t* Infantry)
{	
	if(Infantry->rc_module->info->V.flip == PRESS_TO_RELEASE)
		Infantry->cmd->GIM_TURN_180_CMD = true;
}

void Key_CTRL_Status_Check(Infantry_t* Infantry)
{
	Gimbal_t*		Gimbal	=	Infantry->gimbal_module;

	if(Infantry->rc_module->info->CTRL.state == PRESS)
	{
		if (Infantry->rc_module->info->CTRL.LONG_PRESS_FLAG != true)
		{
			Infantry->cmd->CHAS_SPIN_TOP_CMD 	= false;
			Infantry->cmd->VIS_OUTPOST_CMD		=	false;
			Infantry->cmd->VIS_LARGE_BUFF_CMD = false;
			Infantry->cmd->VIS_SMALL_BUFF_CMD = false;
			
			Infantry->flag->GIM_BUFF_MODE_FLAG= false;
			
			Gimbal->mode		=	GIM_IMU;
			Infantry->mode	= IMU_MODE;
		}
		else
		{
			Gimbal->mode		=	GIM_MEC;
			Infantry->mode	= MEC_MODE;
			
			Infantry->cmd->CHAS_SPIN_TOP_CMD 		= false;
		}
	}
}

bool Cap_Switch;
void Key_SHIFT_Status_Check(Infantry_t* Infantry)
{
	rc_t*				RC			= Infantry->rc_module;
	
	if(RC->info->SHIFT.state == PRESS)
		Infantry->cmd->CAP_OPEN_CMD		=	true;
	else
		Infantry->cmd->CAP_OPEN_CMD		=	false;
}

void Key_Z_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->Z.flip == RELEASE_TO_PRESS)
	{
		Infantry->cmd->VIS_LARGE_BUFF_CMD		= true;
		Infantry->launch_module->fric_mode	= FRIC_SPIN;
	}
	
	if(Infantry->rc_module->info->Z.LONG_PRESS_FLAG == true)
		Infantry->cmd->VIS_FAST_BUFF_CMD = true;
	else
		Infantry->cmd->VIS_FAST_BUFF_CMD = false;
}

void Key_X_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->X.flip == RELEASE_TO_PRESS)
	{
		Infantry->cmd->VIS_SMALL_BUFF_CMD		= true;
		Infantry->launch_module->fric_mode	= FRIC_SPIN;
	}
}

void Key_C_Status_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->C.flip == PRESS_TO_RELEASE)
		Infantry->cmd->VIS_CHANGE_TARGET_CMD = true;
}

void Key_MOUSE_L_Status_Check(Infantry_t* Infantry)
{
	Launch_t*	Launch = Infantry->launch_module;

	if(		Infantry->flag->LAUNCH_HIGH_HEAT_FLAG == false)
	{
		if(Infantry->rc_module->info->MOUSE_L.LONG_PRESS_FLAG == true)
		{
			if(Infantry->cmd->GIM_AIM_ON_CMD == true)					//	按住右键
				Infantry->flag->VIS_CTRL_LAUNCH_FLAG	= true;
			else
				Launch->cmd->LAUNCH_CONTINUOUS				= true;
		}
		else if(Infantry->rc_module->info->MOUSE_L.flip == RELEASE_TO_PRESS)
		{
			if(Launch->fric_mode == FRIC_STOP)
				Launch->fric_mode = FRIC_SPIN;
				
			else if(Launch->flag->FRIC_SPEED_READY_FLAG == true)
			{
				Launch->cmd->LAUNCH_SINGLE			= true;
				Launch->cmd->LAUNCH_CONTINUOUS	= false;
				
//				Infantry->flag->VIS_CTRL_LAUNCH_FLAG = false;
			}
		}
		else
		{
			Launch->cmd->LAUNCH_CONTINUOUS	= false;
			Infantry->flag->VIS_CTRL_LAUNCH_FLAG	= false;
		}
	}
}

void Key_MOUSE_R_Status_Check(Infantry_t* Infantry)
{
	Gimbal_t*	Gimbal = Infantry->gimbal_module;

	if(Infantry->mode == IMU_MODE	&& Gimbal->flag->GIMBAL_INIT_OK_FLAG == true)
	{
		switch(Infantry->rc_module->info->MOUSE_R.state)
		{
			case PRESS:
				Infantry->cmd->GIM_AIM_ON_CMD = true;
				break;
			
			case RELEASE:
				if(Infantry->cmd->VIS_LARGE_BUFF_CMD == true || \
					 Infantry->cmd->VIS_SMALL_BUFF_CMD == true || \
					 Infantry->cmd->VIS_OUTPOST_CMD		 == true)
				{
					Infantry->flag->VIS_CTRL_LAUNCH_FLAG	= true;
					Infantry->cmd->GIM_AIM_ON_CMD					= true;
				}
				else
				{
					Infantry->flag->VIS_CTRL_LAUNCH_FLAG 	= false;
					Infantry->cmd->GIM_AIM_ON_CMD 				= false;
				}
				break;
			
			default:
				break;
		}
		
//		switch(Infantry->rc_module->info->MOUSE_R.flip)
//		{
//			case RELEASE_TO_PRESS:
//				Infantry->flag->VIS_CTRL_LAUNCH_FLAG = true;
//				break;
//				
//			case PRESS_TO_RELEASE:
//				Infantry->flag->VIS_CTRL_LAUNCH_FLAG	= false;
//				Infantry->flag->TEST_VIS_CTRL_FLAG		= false;
//				break;
//				
//			default:
//				break;
//		}		
	}
}

void Key_Infantry_Reset_Check(Infantry_t* Infantry)
{
	if(Infantry->rc_module->info->Z.LONG_PRESS_FLAG	==	true	&&	\
		 Infantry->rc_module->info->X.LONG_PRESS_FLAG ==	true	&&	\
		 Infantry->rc_module->info->C.LONG_PRESS_FLAG	==	true)
	{
		Infantry->mode = SLEEP_MODE;
		
		HAL_Delay(10);
		CAN1_Send_1FF_Motor(0, 0, 0, 0);
		CAN1_Send_200_Motor(0, 0, 0, 0);
		CAN2_Send_1FF_Motor(0, 0, 0, 0);
		CAN2_Send_200_Motor(0, 0, 0, 0);
		HAL_Delay(10);
		
		__set_FAULTMASK(1);
		NVIC_SystemReset();
	}
}
