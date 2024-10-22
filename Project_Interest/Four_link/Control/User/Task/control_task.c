/* Includes ------------------------------------------------------------------*/
#include "control_task.h"


/* Exported functions --------------------------------------------------------*/
void StartControlTask(void const * argument)
{
	for(;;)
	{

		Module_Ctrl();

		osDelay(1);
	}
}
