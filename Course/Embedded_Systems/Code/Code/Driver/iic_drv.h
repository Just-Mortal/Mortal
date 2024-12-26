#ifndef __IIC_DRV_H
#define __IIC_DRV_H
 
#include "driver.h"

#define OLED_I2C_Start()	I2C_Start()
#define OLED_I2C_Stop()		I2C_Stop()

void OLED_I2C_SendByte(uint8 Byte);

#endif