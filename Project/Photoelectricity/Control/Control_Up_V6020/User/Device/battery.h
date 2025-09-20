#ifndef __BATTERY_H
#define __BATTERY_H

#include "stm32f4xx_hal.h"

void Battery_Init(void);
float Read_Battery_Voltage(void);

#endif
