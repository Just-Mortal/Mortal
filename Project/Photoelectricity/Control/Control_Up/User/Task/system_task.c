/* Includes ------------------------------------------------------------------*/
#include "system_task.h"

#include "vision.h"

// imu update
// heartbeat

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void StartSystemTask(void const * argument)
{
	for(;;)
	{
		
		imu.update(&imu);


		
		osDelay(1);
	}
}
