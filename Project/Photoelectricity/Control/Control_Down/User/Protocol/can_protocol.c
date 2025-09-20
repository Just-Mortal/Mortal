#include "can_protocol.h"

//uint32_t TxMailbox;
//CAN_TxHeaderTypeDef TxMessage;

/* ---------- CAN Receive Protocol Fuction begin ---------- */
/**
 *  @brief  CAN1 Receive
 */
void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch(rxId)
	{
		/* --- Motor CAN receive begin --- */
		case 0x111:
			ctrl.Update(&ctrl, rxBuf);
		break;
		
		/* --- Motor CAN receive end   --- */
		
		default:
			break;
	}
}
/* ---------- CAN Receive Protocol Fuction end   ---------- */

/* ---------- CAN Transmit Protocol Fuction begin ---------- */
/* ---------- CAN Transmit Protocol Fuction end   ---------- */
