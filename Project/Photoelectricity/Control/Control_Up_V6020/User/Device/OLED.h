#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx_hal.h"
#include <stdint.h>

// 命令/数据标识
#define OLED_CMD   0
#define OLED_DATA  1

// 引脚定义
#define OLED_CS_GPIO_Port   GPIOB
#define OLED_CS_Pin         GPIO_PIN_12
#define OLED_DC_GPIO_Port   GPIOF
#define OLED_DC_Pin         GPIO_PIN_1
#define OLED_RST_GPIO_Port  GPIOF
#define OLED_RST_Pin        GPIO_PIN_0

// 外部字体声明（只保留1608字体）
extern const unsigned char oled_asc2_1608[95][16];
extern const unsigned char oled_asc2_1206[95][12];

// 函数声明
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Refresh_Gram(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t color);
void OLED_ShowChar(uint8_t x, uint8_t y, char chr, uint8_t mode);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t mode);
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t digits, uint8_t mode);
void OLED_ShowFloat(uint8_t x, uint8_t y, float fnum, uint8_t int_digits, uint8_t frac_digits, uint8_t mode);

#endif
