#include "driver.h"

#include "imu.h"

void Driver_Init(void)
{
	imu.init(&imu);
	HAL_Delay(10);
	
	CAN1_Init();
	CAN2_Init();
	
	USART1_Init();
	USART2_Init();
	USART3_Init();
	
	Led_Close(1);
	Led_Close(2);
	Led_Close(3);
}
