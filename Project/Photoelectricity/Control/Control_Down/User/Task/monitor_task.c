/* Includes ------------------------------------------------------------------*/
#include "monitor_task.h"

extern IWDG_HandleTypeDef hiwdg;

/* Exported functions --------------------------------------------------------*/
void StartMonitorTask(void const * argument)
{
	
	for(;;)
	{
		ctrl.Heartbeat(&ctrl);
		
		HAL_IWDG_Refresh(&hiwdg);
		
		osDelay(1);
	}
}
