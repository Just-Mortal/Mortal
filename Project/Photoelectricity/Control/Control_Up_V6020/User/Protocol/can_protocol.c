#include "can_protocol.h"

uint32_t TxMailbox;

uint8_t CAN1_6020[8];
uint8_t CAN1_4310[8];

CAN_TxHeaderTypeDef TxMessage;

/* ---------- CAN Receive Protocol Fuction begin ---------- */
/**
 *  @brief  CAN1 Receive
 */
void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch(rxId)
	{
		/* --- Motor CAN receive begin --- */
		case GIM_YAW:
			motor[GIMBAL_YAW].rx(&motor[GIMBAL_YAW], rxBuf, 8);
			motor[GIMBAL_YAW].state.offline_cnt = 0;
			break;
			
		case GIM_PCH:
			motor[GIMBAL_PCH].rx(&motor[GIMBAL_PCH], rxBuf, 8);
			motor[GIMBAL_PCH].state.offline_cnt = 0;
			break;
		
		/* --- Motor CAN receive end   --- */
		
		default:
			break;
	}
}

/**
 *  @brief  CAN2 Receive
 */
void CAN2_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
}
/* ---------- CAN Receive Protocol Fuction end   ---------- */

/* ---------- CAN Transmit Protocol Fuction begin ---------- */
/**
 *  @brief  CAN Send Fuction
 *  @author JSJ
 */
uint8_t CAN1_Send_6020(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;
	
  TxMessage.StdId	=	0x1FF;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;											
	
	CAN1_6020[0] = (uint8_t)(motor1>>8);
	CAN1_6020[1] = (uint8_t) motor1;
	CAN1_6020[2] = (uint8_t)(motor2>>8);
	CAN1_6020[3] = (uint8_t) motor2;
	CAN1_6020[4] = (uint8_t)(motor3>>8);
	CAN1_6020[5] = (uint8_t) motor3;
	CAN1_6020[6] = (uint8_t)(motor4>>8);
	CAN1_6020[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_6020[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_6020,&TxMailbox);
	
	memset(CAN1_6020, 0, sizeof(uint8_t)*8);
		
	return res;
}

/* ---------- CAN Transmit Protocol Fuction end   ---------- */
