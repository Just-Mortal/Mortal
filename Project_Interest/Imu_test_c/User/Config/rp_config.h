#ifndef __RP_CONFIG_H
#define __RP_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "drivers_config.h"
#include "devices_config.h"
#include "user_config.h"

/* Exported functions --------------------------------------------------------*/
//��ຯ��
void WFI_SET(void);						//ִ��WFIָ��
void INTX_DISABLE(void);			//�ر������ж�
void INTX_ENABLE(void);				//���������ж�
void MSR_MSP(uint32_t addr);	//���ö�ջ��ַ 

#endif
