/* Includes ------------------------------------------------------------------*/
#include "rc_protocol.h"

/* Exported functions --------------------------------------------------------*/
/**
 *    @brief    在串口2中解析遥控数据协议
 */
void USART2_rxDataHandler(uint8_t *rxBuf)
{
    rc_sensor.update(&rc_sensor, rxBuf);
    rc_sensor.check(&rc_sensor);
}
