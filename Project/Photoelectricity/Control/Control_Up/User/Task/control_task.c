/* Includes ------------------------------------------------------------------*/
#include "control_task.h"
#include "imu.h"

#include "can_protocol.h"

/* Exported variables --------------------------------------------------------*/
extern IWDG_HandleTypeDef hiwdg;

/* Exported functions --------------------------------------------------------*/
void StartControlTask(void const * argument)
{
	static uint16_t vision_fps;

	for(;;)
	{
		// 状态更新
		robot.state_update(&robot);
		
		// --------------- vision --------------- //
		vision_fps  ++;
		if(vision_fps > 200)
		{
			vision_fps = 0;
			vision_send_data();
		}
		// --------------- vision --------------- //
		
		// --------------- gimbal --------------- //
		gimbal.work(&gimbal);
		
		int16_t set_6020 = gimbal.info->yaw_motor_set;
		float		set_4310 = gimbal.info->pch_motor_set;
		
		CAN1_Send_6020(set_6020, 0, 0, 0);
		
		if(motor[GIMBAL_PCH].state.work_state == M_OFFLINE)
			enable_motor_mode(0x011);
		else if(motor[GIMBAL_PCH].rx_info.state != 1)
			clean_motor_err(0x011);
		else if(motor[GIMBAL_PCH].state.work_state == M_ONLINE)
			mit_ctrl(0x011, set_4310);
		// --------------- gimbal --------------- //


		// ------------- 底盘控制 ---------------- //
		ctrl.Work(&ctrl);
		// ------------- 底盘控制 ---------------- //
		
//		HAL_IWDG_Refresh(&hiwdg);
		
		osDelay(1);
	}
}
