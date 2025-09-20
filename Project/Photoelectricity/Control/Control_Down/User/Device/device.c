#include "device.h"

void Device_Init(void)
{
	// Initialize all motors
	for (int i = 0; i < MOTOR_NUM; i++)
			motor[i].Init(&motor[i]);

	// PS2ÊÖ±ú³õÊ¼»¯
//	AX_PS2_Init(GPIOE, GPIO_PIN_7,   // CLK
//							GPIOE, GPIO_PIN_8,   // CS
//							GPIOE, GPIO_PIN_10,  // CMD
//							GPIOE, GPIO_PIN_12); // DAT

  ctrl.Init(&ctrl);
}
