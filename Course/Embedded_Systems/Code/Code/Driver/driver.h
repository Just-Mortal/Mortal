#ifndef __DRIVER_H
#define __DRIVER_H

#include	"config.h"
#include	"STC8G_H_GPIO.h"
#include	"STC8G_H_I2C.h"
#include	"STC8G_H_Soft_I2C.h"
#include	"STC8G_H_Timer.h"
#include	"STC8G_H_UART.h"
#include	"STC8G_H_NVIC.h"
#include	"STC8G_H_Switch.h"
#include	"STC8G_H_ADC.h"

void	GPIO_config(void);
void	Timer_config(void);
void	I2C_config(void);
void	UART_config(void);
void	ADC_config(void);

#endif
