/* Includes ------------------------------------------------------------------*/
#include "monitor_task.h"

// led Ê§Áª¼ì²âÏÔÊ¾
// R	->	1
// B	->	2
// G	->	3	

extern IWDG_HandleTypeDef hiwdg;

/* Exported functions --------------------------------------------------------*/
void StartMonitorTask(void const * argument)
{
	uint32_t i;
	
	for(;;)
	{
		#if WATCH_DOG_SWITCH == 1
		HAL_IWDG_Refresh(&hiwdg);
		#endif
	
		i++;
		
		infantry.heartbeat(&infantry);
		
		// Ò£¿ØÆ÷Ê§Áª¼ì²â
		switch(infantry.rc_module->dev->work_state)
		{
			case DEV_ONLINE:
				Led_Open(1);
				break;
				
			case DEV_OFFLINE:
				Led_Close(1);
				break;
			
			default:
				break;
		}
		
		// Vision Ê§Áª¼ì²â
		switch(infantry.vision_module->work_state)
		{
			case DEV_ONLINE:
				Led_Open(2);
				break;
				
			case DEV_OFFLINE:
				Led_Close(2);
				break;
			
			default:
				break;
		}
		
		// Motor Ê§Áª¼ì²â
		if(motor[CHASSIS_LB].state.work_state		== M_OFFLINE	||
			 motor[CHASSIS_RB].state.work_state		== M_OFFLINE	||
			 motor[CHASSIS_LF].state.work_state		== M_OFFLINE	||
			 motor[CHASSIS_RF].state.work_state		== M_OFFLINE	||
			 motor[GIMBAL_YAW].state.work_state 	== M_OFFLINE	||
			 motor[GIMBAL_PITCH].state.work_state == M_OFFLINE	||
			 motor[LAUNCH_FRIC_L].state.work_state== M_OFFLINE	||
			 motor[LAUNCH_FRIC_R].state.work_state== M_OFFLINE	||
			 motor[LAUNCH_DIAL].state.work_state 	== M_OFFLINE	)
			Led_Close(3);
		else
			Led_Open(3);
			
		osDelay(1);
	}
}
