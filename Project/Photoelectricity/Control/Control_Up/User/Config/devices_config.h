#ifndef __DEVICES_CONFIG_H
#define __DEVICES_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* Exported types ------------------------------------------------------------*/
/* ----------------------------�豸�� ----------------------------------------*/
/**
 *	@brief	�豸����״̬(ͨ��)
 *	@class	device
 */
typedef enum {
	DEV_ONLINE,
	DEV_OFFLINE,
} dev_work_state_t;

#endif
