/* Includes ------------------------------------------------------------------*/
#include "control_task.h"
#include "imu.h"

#include "can_protocol.h"

/* Exported variables --------------------------------------------------------*/

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
		
		int16_t set_yaw = gimbal.info->yaw_motor_set;
		int16_t	set_pch = gimbal.info->pch_motor_set;
		
		CAN1_Send_6020(set_yaw, set_pch, 0, 0);
		// --------------- gimbal --------------- //


		// ------------- 底盘控制 ---------------- //
		ctrl.Work(&ctrl);
		// ------------- 底盘控制 ---------------- //
		
		if(ctrl.up_info.bit_control.winner_flag == 1)
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, GPIO_PIN_RESET);
		
		osDelay(1);
	}
}
