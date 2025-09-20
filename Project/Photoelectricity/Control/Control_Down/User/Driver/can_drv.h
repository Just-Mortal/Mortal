#ifndef __CAN_DRV_H
#define __CAN_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "string.h"

/* Exported macro ------------------------------------------------------------*/
#define MAX_WAIT_TX_PORT_CNT 4  // 等待发送的最大端口数目

/* Exported types ------------------------------------------------------------*/
/**
*	@brief	can发送模式
 *	@class	driver
 */
typedef enum {
    CAN_SINGLE_TX 		= 0,
    CAN_CONTINOUS_TX 	= 1
} CAN_TxModeTypeDef;

typedef enum {
    CAN_MAILBOX_OK,
    CAN_MAILBOX_ERR,
    CAN_MAILBOX_BUSY
} CAN_MailboxErrnoTypeDef;

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t             data[8];
} CAN_RxFrameTypeDef;

typedef struct {
    CAN_TxHeaderTypeDef header;
    uint8_t             data[8];
} CAN_TxFrameTypeDef;

typedef struct CAN_RxPortType {
    uint32_t 	rx_id;
    uint8_t  	rx_buff[8];
    struct 		CAN_RxPortType *next;
} CAN_RxPortTypeDef;

typedef struct CAN_TxPortType {
    uint32_t                tx_id;
    uint8_t                 tx_buff[8];
    uint16_t                tx_period;
    uint32_t                last_tx_time;
    uint32_t                delta_tx_time;
    FunctionalState         state;
    CAN_TxModeTypeDef       mode;
    void                    *mailbox;
    struct CAN_TxPortType   *next;
} CAN_TxPortTypeDef;

typedef struct {
    CAN_MailboxErrnoTypeDef errno;
    uint32_t            err_cnt;
    CAN_TxPortTypeDef   *tx_port;
    uint8_t             tx_port_cnt;
    CAN_TxPortTypeDef   *wait_tx_port_fifo[MAX_WAIT_TX_PORT_CNT];
    uint8_t             wait_tx_port_cnt;
    FunctionalState     auto_tx;
} CAN_MailboxTypeDef;

extern CAN_MailboxTypeDef hcan1Mailbox;

/* Exported functions --------------------------------------------------------*/
void CAN1_Init(void);
HAL_StatusTypeDef CAN_Send_Data(CAN_HandleTypeDef *hcan, uint32_t stdId, uint8_t *dat);

#endif
