/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"

/* Exported functions --------------------------------------------------------*/
/**
 *    @brief    在串口3中解析遥控数据协议
 */
void USART3_rxDataHandler(uint8_t *rxBuf)
{
    rc_sensor.update(&rc_sensor, rxBuf);
    rc_sensor.check(&rc_sensor);
}
