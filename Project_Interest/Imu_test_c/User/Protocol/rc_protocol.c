/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"

/* Exported functions --------------------------------------------------------*/
/**
 *    @brief    �ڴ���3�н���ң������Э��
 */
void USART3_rxDataHandler(uint8_t *rxBuf)
{
    rc_sensor.update(&rc_sensor, rxBuf);
    rc_sensor.check(&rc_sensor);
}
