#ifndef __DEVICES_CONFIG_H
#define __DEVICES_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "drivers_config.h"

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

/**
 *	@brief	�豸id�б�
 *	@class	device
 */
typedef enum {
	DEV_ID_IMU = 0,
  DEV_ID_IMU_EX,
	DEV_ID_RC,
	DEV_ID_VISION,
	DEV_ID_CNT,
} dev_id_t;

#endif
