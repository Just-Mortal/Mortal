#include "iic_drv.h"

sbit    SDA = P0^1;
sbit    SCL = P0^0;

#define OLED_W_SCL(x)			SCL = x;I2C_Delay();
#define OLED_W_SDA(x)			SDA = x;I2C_Delay();

void OLED_I2C_SendByte(uint8 Byte)
{
	uint8 i;
	for (i = 0; i < 8; i++)
	{
		OLED_W_SDA(Byte & (0x80 >> i));
		OLED_W_SCL(1);
		OLED_W_SCL(0);
	}
	OLED_W_SCL(1);
	OLED_W_SCL(0);
}

