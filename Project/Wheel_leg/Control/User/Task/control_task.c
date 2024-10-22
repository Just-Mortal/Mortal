/* Includes ------------------------------------------------------------------*/
#include "control_task.h"
#include "imu.h"

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void StartControlTask(void const * argument)
{
	for(;;)
	{

//		HAL_IWDG_Refresh(&hiwdg);
		
		osDelay(1);
	}
}
