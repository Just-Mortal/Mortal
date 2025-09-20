/* Includes ------------------------------------------------------------------*/
#include "control_task.h"

/* Exported variables --------------------------------------------------------*/
//uint8_t dead_time;

/* Exported functions --------------------------------------------------------*/
void StartControlTask(void const * argument)
{
	chassis.Init(&chassis);
	locator.Init(&locator);
	
	for(;;)
	{
		//	复位操作
		if(ctrl.up_info.bit_control.chassis_begin == 0 && ctrl.status == CTRL_ONLINE)
		{
			locator.info->measure.x_pos = 0.f;
			locator.info->measure.y_pos = 0.f;
			locator.info->yaw_bias = 0.f;
		}

		if(ctrl.up_info.bit_control.chassis_switch == 1 && ctrl.status == CTRL_ONLINE)
		{
			locator.info->target.x_pos = ctrl.x_pos;
			locator.info->target.y_pos = ctrl.y_pos;
			locator.info->target.a_yaw = 0.f;
			
			locator.Update(&locator);
			locator.CalculatePID(&locator);
			
			// Update Speed
			chassis.info.front_speed = locator.info->front;
			chassis.info.right_speed = locator.info->right;
			chassis.info.cycle_speed = locator.info->cycle;
			
			chassis.Update(&chassis);
			chassis.Work(&chassis);


		}
		else
		{
			chassis.info.front_speed = 0.0f;
			chassis.info.right_speed = 0.0f;
			chassis.info.cycle_speed = 0.0f;

			chassis.Sleep(&chassis);
		}
		
		// ------------ 响鸣亮灯 begin ------------ //
		if(ctrl.up_info.bit_control.winner_flag == 1)
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);
		}
		// ------------ 响鸣亮灯 end   ------------ //
		
		osDelay(1);
	}
}
