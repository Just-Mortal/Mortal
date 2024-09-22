/* Includes ------------------------------------------------------------------*/
#include "vision_protocol.h"

void USART1_rxDataHandler(uint8_t *rxBuf)
{
	vision.update(&vision, rxBuf);
	vision.check(&vision);
}
