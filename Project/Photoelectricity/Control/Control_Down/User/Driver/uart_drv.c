/* Includes ------------------------------------------------------------------*/
#include "uart_drv.h"

extern UART_HandleTypeDef huart2;

/* Private macro -------------------------------------------------------------*/
#define USART2_RX_BUF_LEN     150		//	N10P	108 bytes

/* Private variables ---------------------------------------------------------*/
uint8_t usart2_dma_rxbuf[USART2_RX_BUF_LEN];

/* private functions --------------------------------------------------------*/
static void uart_rx_idle_callback(UART_HandleTypeDef* huart);
static HAL_StatusTypeDef DMA_Start(DMA_HandleTypeDef *hdma, \
																	uint32_t SrcAddress, \
																	uint32_t DstAddress, \
																	uint32_t DataLength);
/* rxData Handler [Weak] functions -------------------------------------------*/
/**
 *	@brief	[__WEAK] 需要在Potocol Layer中实现具体的 USARTX 处理协议
 */
__WEAK void USART2_rxDataHandler(uint8_t *rxBuf)
{
}

/* Exported functions --------------------------------------------------------*/
/**
 *  @brief  callback this function when uart interrupt 
 *  @param  uart IRQHandler id
 *  @usage  call in uart handler function USARTx_IRQHandler()
 */
void DRV_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    if( __HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
        __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        uart_rx_idle_callback(huart);
    }
}


/**
 *	@brief	USART2 Initialization
 */
void USART2_Init(void)
{
	__HAL_UART_CLEAR_IDLEFLAG(&huart2);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	
	// Enable the DMA transfer for the receiver request
	SET_BIT(huart2.Instance->CR3, USART_CR3_DMAR);	
	
	DMA_Start(huart2.hdmarx, \
			  (uint32_t)&huart2.Instance->DR, \
			  (uint32_t)usart2_dma_rxbuf, \
			  USART2_RX_BUF_LEN);
}

/* Private functions ---------------------------------------------------------*/

/**
 *  @brief  clear idle it flag after uart receive a frame data
 *  @param  uart IRQHandler id
 *  @usage  call in DRV_UART_IRQHandler() function
 */
static void uart_rx_idle_callback(UART_HandleTypeDef* huart)
{
    /* clear idle it flag avoid idle interrupt all the time */
    __HAL_UART_CLEAR_IDLEFLAG(huart);    
    /* handle received data in idle interrupt */
    
		if (huart == &huart2)
		{
			/* clear DMA transfer complete flag */
			__HAL_DMA_DISABLE(huart->hdmarx);
			/* handle dbus data dbus_buf from DMA */
			USART2_rxDataHandler(usart2_dma_rxbuf);
			memset(usart2_dma_rxbuf, 0, USART2_RX_BUF_LEN);
			/* restart dma transmission */	  
			__HAL_DMA_ENABLE(huart->hdmarx);		
		}
}

static HAL_StatusTypeDef DMA_Start(DMA_HandleTypeDef *hdma, \
                            uint32_t SrcAddress, \
                            uint32_t DstAddress, \
                            uint32_t DataLength)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* Process locked */
	__HAL_LOCK(hdma);
	if(HAL_DMA_STATE_READY == hdma->State)
	{
		/* Change DMA peripheral state */
		hdma->State = HAL_DMA_STATE_BUSY;

		/* Initialize the error code */
		hdma->ErrorCode = HAL_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		/* Clear DBM bit */
		hdma->Instance->CR &= (uint32_t)(~DMA_SxCR_DBM);

		/* Configure DMA Stream data length */
		hdma->Instance->NDTR = DataLength;

		/* Memory to Peripheral */
		if((hdma->Init.Direction) == DMA_MEMORY_TO_PERIPH)
		{
			/* Configure DMA Stream destination address */
			hdma->Instance->PAR = DstAddress;

			/* Configure DMA Stream source address */
			hdma->Instance->M0AR = SrcAddress;
		}
		/* Peripheral to Memory */
		else
		{
			/* Configure DMA Stream source address */
			hdma->Instance->PAR = SrcAddress;

			/* Configure DMA Stream destination address */
			hdma->Instance->M0AR = DstAddress;
		}

		/* Enable the Peripheral */
		__HAL_DMA_ENABLE(hdma);
	}
	else
	{
		/* Process unlocked */
		__HAL_UNLOCK(hdma);

		/* Return error status */
		status = HAL_BUSY;
	} 
	return status; 	
}
