/* Includes ------------------------------------------------------------------*/
#include "can_drv.h"

extern CAN_HandleTypeDef hcan1;

__WEAK void CAN1_rxDataHandler(uint32_t canId, uint8_t *rxBuf);

/* Private function prototypes -----------------------------------------------*/
static void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_RxFrameTypeDef hcan1RxFrame;
CAN_TxFrameTypeDef hcan1TxFrame;

/* Exported variables --------------------------------------------------------*/
CAN_MailboxTypeDef hcan1Mailbox = {
    .errno 							= CAN_MAILBOX_OK,
    .err_cnt 						= 0,
    .tx_port 						= NULL,
    .tx_port_cnt 				= 0,
    .wait_tx_port_fifo 	= {NULL,NULL,NULL,NULL},
    .wait_tx_port_cnt 	= 0,
    .auto_tx 						= ENABLE
};

/* Exported functions --------------------------------------------------------*/
/**
 *  @brief  CAN1 ��ʼ��
 */
void CAN1_Init(void)
{
    CAN_FilterTypeDef sFilterConfig;
    
    CAN_Filter_ParamsInit(&sFilterConfig);				    									// ����CAN��ʶ���˲���
    HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig);
    
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);	// ʹ�ܽ����ж�
    
    HAL_CAN_Start(&hcan1);				    																	// ����CAN1
}

/**
 *  @brief  CAN ���ͺ���
 */
HAL_StatusTypeDef CAN_Send_Data(CAN_HandleTypeDef *hcan, uint32_t stdId, uint8_t *dat)
{
	CAN_TxHeaderTypeDef pHeader;
	uint32_t txMailBox;
	
	if((hcan->Instance != CAN1)&&(hcan->Instance != CAN2))
	{
		return HAL_ERROR;
	}
	
	pHeader.StdId = stdId;
	pHeader.IDE = CAN_ID_STD;
	pHeader.RTR = CAN_RTR_DATA;
	pHeader.DLC = 8;
	
	if(HAL_CAN_AddTxMessage(hcan, &pHeader, dat, &txMailBox) != HAL_OK)
	{
		return HAL_ERROR;
	}
	
	return HAL_OK;
}

/* Callback functions --------------------------------------------------------*/
/**
 *  @brief  ��д CAN RxFifo �жϽ��պ���
 *  @note   ��stm32f4xx_hal_can.c��������
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance == CAN1)
	{
		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &hcan1RxFrame.header, hcan1RxFrame.data);
		CAN1_rxDataHandler(hcan1RxFrame.header.StdId, hcan1RxFrame.data);
	}
}

/* Private functions ---------------------------------------------------------*/
/**
 *  @brief  CAN ��ʶ����������λ��Ĭ������
 */
static void CAN_Filter_ParamsInit(CAN_FilterTypeDef *sFilterConfig)
{
    sFilterConfig->FilterIdHigh = 0;                        
    sFilterConfig->FilterIdLow = 0;                            
    sFilterConfig->FilterMaskIdHigh = 0;                    // ������
    sFilterConfig->FilterMaskIdLow = 0;                     // ������
    sFilterConfig->FilterFIFOAssignment = CAN_FILTER_FIFO0; // ������������FIFO0
    sFilterConfig->FilterBank = 0;                          // ���ù�����0
    sFilterConfig->FilterMode = CAN_FILTERMODE_IDMASK;      // ��ʶ������ģʽ
    sFilterConfig->FilterScale = CAN_FILTERSCALE_32BIT;     // 32λ��
    sFilterConfig->FilterActivation = ENABLE;               // �����˲���
    sFilterConfig->SlaveStartFilterBank = 0;
}

/* rxData Handler [Weak] functions -------------------------------------------*/
/**
 *  @brief  [__WEAK] ��Ҫ��Protocol Layer��ʵ�־���� CAN ����Э��
 */
__WEAK void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
}
