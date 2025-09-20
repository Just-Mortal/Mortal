/* Includes ------------------------------------------------------------------*/
#include "system_task.h"

#include "imu.h"

/* Private macro -------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
/* Private variables ---------------------------------------------------------*/
imu_status_t imu_status = IMU_ERROR;

/* Private functions ---------------------------------------------------------*/
void StartSystemTask(void const * argument)
{
//	imu_status = imu_init(&hi2c1);
	
	for(;;)
	{
			
	
//		AX_PS2_ScanKey(&remote_ctrl);
		
//		imu_update(0.001f);
		
		osDelay(1);
	}
}
