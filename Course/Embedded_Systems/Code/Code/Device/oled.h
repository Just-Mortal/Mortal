/**
 * @file			oled.h
 * @author		Sujian Jia
 * @Version		V1.0
 * @date			2024.12.26	04:25
 *
 */
 
#ifndef __OLED_H
#define __OLED_H
 
#include "driver.h"
#include "iic_drv.h"

#include "oled_font.h"

void OLED_WriteData(uint8 Data);
void OLED_SetCursor(uint8 Y, uint8 X);
void OLED_Clear(void);
void OLED_Init(void);

void OLED_DrawDot(uint8 x, uint8 y);
void OLED_DrawLine(uint8 x1, uint8 y1, uint8 x2,uint8 y2);

#endif
 