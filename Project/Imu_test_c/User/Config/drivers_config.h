#ifndef __DRIVER_CONFIG_H
#define __DRIVER_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

/* Exported types ------------------------------------------------------------*/
/* ---------------------------------������ -----------------------------------*/
/**
 *	@brief	�����������(Error No.)
 *	@class	driver
 */
typedef enum drv_errno {
    DRV_OK = HAL_OK,
    DRV_ERR = HAL_ERROR,
    DRV_BUSY = HAL_BUSY,
    DRV_TIME_OUT = HAL_TIMEOUT,
    DRV_UNKNOWN_ID,
    DRV_OUT_OF_RANGE
}drv_errno_t;
 
/**
 *	@brief	��������
 *	@class	driver
 */
typedef enum drv_type {
	DRV_CAN,
	DRV_UART,
} drv_type_t;

/**
 *	@brief	can���� id
 *	@class	driver
 */
typedef enum {
    DRV_CAN1,
    DRV_CAN2
} can_id_t;

/**
 *	@brief	uart���� id
 *	@class	driver
 */
typedef enum {
    DRV_UART1,
    DRV_UART2,
    DRV_UART3,
    DRV_UART4,
    DRV_UART5,
		DRV_UART6,
} uart_id_t;

/**
*	@brief	can����ģʽ
 *	@class	driver
 */
typedef enum {
    CAN_SINGLE_TX 		= 0,
    CAN_CONTINOUS_TX 	= 1
} CAN_TxModeTypeDef;

/**
 *	@brief	can����
 *	@class	driver
 */
typedef struct drv_can {
		drv_type_t 	type;
    can_id_t    id;
    drv_errno_t errno;
    uint32_t    err_cnt;
		uint32_t		rx_id;  		// �������ı�ʶ��
		uint32_t		tx_id;  		// �ϴ����ı�ʶ��
		uint8_t			data_idx;		// �����±�
    uint16_t    tx_period; 	// ��ʱ���ͼ��(ms)
		void				(*add_msg)(struct drv_can *self, uint8_t *data, uint8_t data_cnt);
    void        (*add_byte)(struct drv_can *self, uint8_t byte);
    void        (*add_halfword)(struct drv_can *self, uint16_t hword);
    void        (*add_word)(struct drv_can *self, uint32_t word);
    FunctionalState (*get_tx_state)(struct drv_can *self);
    void        (*set_tx_state)(struct drv_can *self, FunctionalState state);
    void        (*set_tx_mode)(struct drv_can *self, CAN_TxModeTypeDef mode);
    void        (*manual_tx)(struct drv_can *self, uint8_t *data);
} drv_can_t;

/**
 *	@brief	uart����
 *	@class	driver
 */
typedef struct drv_uart {
		drv_type_t	type;
    uart_id_t   id;
    drv_errno_t errno;
    uint32_t    err_cnt;
    uint8_t     *tx_buf;
    uint8_t     tx_buf_len;
    uint8_t     tx_data_cnt;
    void        (*add_msg)(struct drv_uart *self, uint8_t *data, uint8_t data_cnt);
    void        (*start_tx)(struct drv_uart *self);
    void        (*start_tx_dma)(struct drv_uart *self);
} drv_uart_t;

#endif
