/* Includes ------------------------------------------------------------------*/
#include "monitor_task.h"

/* Exported functions --------------------------------------------------------*/
void StartMonitorTask(void const * argument)
{
	for(;;)
	{
		for(motor_list_e i = JOINT_REAL; i < MOTOR_LIST; i++)
			motor[i].heartbeat(&motor[i]);
			
		rc.dev->heartbeat(rc.dev);
		
		
		osDelay(1);
	}
}
