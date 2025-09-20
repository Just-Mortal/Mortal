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
	for(;;)
	{
		motor[GIMBAL_YAW].heartbeat(&motor[GIMBAL_YAW]);
		motor[GIMBAL_PCH].heartbeat(&motor[GIMBAL_PCH]);

		vision.heartbeat(&vision);
		
		ctrl.Heartbeat(&ctrl);

		osDelay(1);
	}
}
