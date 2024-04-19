#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint16_t Byte);
void Serial_SendArray(uint8_t *Array ,uint16_t Length);
void Serial_SendString(char *String);
uint16_t Serial_Pow(uint32_t x,uint32_t y);
void Serial_SendNumber(uint32_t Number ,uint8_t Length);

uint8_t Serial_GetRXFlag(void);
uint16_t Serial_GetRXDate(void);

#endif
