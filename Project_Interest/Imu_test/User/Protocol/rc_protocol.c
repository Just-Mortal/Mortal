/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"

/* Exported functions --------------------------------------------------------*/
/**
 *    @brief    �ڴ���2�н���ң������Э��
 */
void USART2_rxDataHandler(uint8_t *rxBuf)
{
    rc_sensor.update(&rc_sensor, rxBuf);
    rc_sensor.check(&rc_sensor);
}
