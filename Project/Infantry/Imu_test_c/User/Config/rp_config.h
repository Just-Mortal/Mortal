#ifndef __RP_CONFIG_H
#define __RP_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "drivers_config.h"
#include "devices_config.h"
#include "user_config.h"

/* Exported functions --------------------------------------------------------*/
//汇编函数
void WFI_SET(void);						//执行WFI指令
void INTX_DISABLE(void);			//关闭所有中断
void INTX_ENABLE(void);				//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址 

#endif
