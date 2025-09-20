#include "driver.h"

#include "imu.h"

void Driver_Init(void)
{
	imu.init(&imu);

	CAN1_Init();
	CAN2_Init();
}
