/* Includes ------------------------------------------------------------------*/
#include "control_task.h"
#include "imu.h"

/* Exported variables --------------------------------------------------------*/
extern bool IMU_INIT_LOAD_FLAG;

extern IWDG_HandleTypeDef hiwdg;

/* Exported functions --------------------------------------------------------*/
void StartControlTask(void const * argument)
{
	for(;;)
	{
		if(IMU_INIT_LOAD_FLAG)
		{
			infantry.update(&infantry);
			infantry.work(&infantry);
		}
		
//		HAL_IWDG_Refresh(&hiwdg);
		
		osDelay(1);
	}
}
