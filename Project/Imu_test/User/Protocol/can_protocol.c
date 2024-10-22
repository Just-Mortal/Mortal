#include "can_protocol.h"

// NOTE
// Half_Rudder send Buffx2E and Buffx2F in slave

uint32_t TxMailbox;

uint8_t CAN1_200_TxData[8];
uint8_t CAN1_1FF_TxData[8];
uint8_t CAN1_2FF_TxData[8];
uint8_t CAN1_1FE_TxData[8];
uint8_t CAN1_2FE_TxData[8];
uint8_t CAN2_200_TxData[8];
uint8_t CAN2_1FF_TxData[8];
uint8_t CAN2_2FF_TxData[8];
uint8_t CAN2_1FE_TxData[8];
uint8_t CAN2_2FE_TxData[8];

CAN_TxHeaderTypeDef TxMessage;

float int16_to_float(int16_t a, int16_t a_max, int16_t a_min, float b_max, float b_min)
{
	float b = (float)(a - a_min) / (float)(a_max - a_min) * (b_max - b_min) + b_min;
	return b;
}

/* ---------- CAN Receive Protocol Fuction begin ---------- */
/**
 *  @brief  CAN1 Receive
 */
void CAN1_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch(rxId)
	{
		/* --- Motor CAN receive begin --- */
		case CHASSIS_LF_ID:
			motor[CHASSIS_LF].rx(&motor[CHASSIS_LF], rxBuf, 8);
			motor[CHASSIS_LF].state.offline_cnt = 0;
			break;
	
		case CHASSIS_RF_ID:
			motor[CHASSIS_RF].rx(&motor[CHASSIS_RF], rxBuf, 8);
			motor[CHASSIS_RF].state.offline_cnt = 0;
			break;
			
		case CHASSIS_LB_ID:
			motor[CHASSIS_LB].rx(&motor[CHASSIS_LB], rxBuf, 8);
			motor[CHASSIS_LB].state.offline_cnt = 0;
			break;
			
		case CHASSIS_RB_ID:
			motor[CHASSIS_RB].rx(&motor[CHASSIS_RB], rxBuf, 8);
			motor[CHASSIS_RB].state.offline_cnt = 0;
			break;
		/* --- Motor CAN receive end   --- */

		/* --- Judge CAN receive begin --- */
//		case GAME_STATUS_ID:
//			judge.update(&judge, rxId, rxBuf);
//			break;
		
		// 3	Hz
		case GAME_ROBOT_HP_ID:
			judge.update(&judge, rxId, rxBuf);
			break;

		// 50	Hz
		case POWER_HEAT_ID:
			judge.update(&judge, rxId, rxBuf);
			break;
			
		// 10	Hz
		case GAME_ROBOT_STATUS_ID:
			judge.update(&judge, rxId, rxBuf);
			break;
			
		// Éä»÷·¢ËÍ
		case SHOOT_DATA_ID:
			judge.update(&judge, rxId, rxBuf);
			break;
		
//		case ROBOT_HURT_ID:					// ¿ÛÑª·¢ËÍ
//			judge.update(&judge, rxId, rxBuf);
//			break;
//		
//		case CHASSIS_POWER_CAN_ID:	// 50Hz
//			judge.update(&judge, rxId, rxBuf);
//			break;
		/* --- Judge CAN receive end   --- */
		
		// slave other info
		case SLAVE_INFO_ID:
			judge.update(&judge, rxId, rxBuf);
			break;

//		case 0x30:
//			cap_u = int16_to_float(((uint16_t)rxBuf[0] << 8| rxBuf[1]), 32000, -32000, 30, 0);
//			
//			if(((uint16_t)rxBuf[4] << 8| rxBuf[5]) == 0)
//				CAP_STATE = 1; 
//			else
//				CAP_STATE = 0;
//			break;
//			
		default:
			break;
	}
}

/**
 *  @brief  CAN2 Receive
 */
void CAN2_rxDataHandler(uint32_t rxId, uint8_t *rxBuf)
{
	switch(rxId)
	{
		/* --- Motor CAN receive begin --- */
		case LAUNCH_FRIC_R_ID:
				motor[LAUNCH_FRIC_R].rx(&motor[LAUNCH_FRIC_R], rxBuf, 8);
				motor[LAUNCH_FRIC_R].state.offline_cnt = 0;
				break;
		
		case LAUNCH_FRIC_L_ID:
				motor[LAUNCH_FRIC_L].rx(&motor[LAUNCH_FRIC_L], rxBuf, 8);
				motor[LAUNCH_FRIC_L].state.offline_cnt = 0;
				break;
		
		case LAUNCH_DIAL_ID:
				motor[LAUNCH_DIAL].rx(&motor[LAUNCH_DIAL], rxBuf, 8);
				motor[LAUNCH_DIAL].state.offline_cnt = 0;
				break;

		case GIMBAL_PITCH_ID:
				motor[GIMBAL_PITCH].rx(&motor[GIMBAL_PITCH], rxBuf, 8);
				motor[GIMBAL_PITCH].state.offline_cnt = 0;
				break;
			
		case GIMBAL_YAW_ID:
				motor[GIMBAL_YAW].rx(&motor[GIMBAL_YAW], rxBuf, 8);
				motor[GIMBAL_YAW].state.offline_cnt = 0;
				break;
		/* --- Motor CAN receive end   --- */
		
		default:
			break;
	}
}
/* ---------- CAN Receive Protocol Fuction end   ---------- */

/* ---------- CAN Transmit Protocol Fuction begin ---------- */
/**
 *  @brief  CAN Send Fuction
 *  @author JSJ
 */
uint8_t CAN1_Send_200_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{  
	uint8_t cnt, res;
	
	TxMessage.StdId	=	0x200;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;	
		
	CAN1_200_TxData[0] = (uint8_t)(motor1>>8);
	CAN1_200_TxData[1] = (uint8_t) motor1;
	CAN1_200_TxData[2] = (uint8_t)(motor2>>8);
	CAN1_200_TxData[3] = (uint8_t) motor2;
	CAN1_200_TxData[4] = (uint8_t)(motor3>>8);
	CAN1_200_TxData[5] = (uint8_t) motor3;
	CAN1_200_TxData[6] = (uint8_t)(motor4>>8);
	CAN1_200_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_200_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_200_TxData,&TxMailbox);
	
	memset(CAN1_200_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN1_Send_1FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;
	
  TxMessage.StdId	=	0x1FF;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;											
	
	CAN1_1FF_TxData[0] = (uint8_t)(motor1>>8);
	CAN1_1FF_TxData[1] = (uint8_t) motor1;
	CAN1_1FF_TxData[2] = (uint8_t)(motor2>>8);
	CAN1_1FF_TxData[3] = (uint8_t) motor2;
	CAN1_1FF_TxData[4] = (uint8_t)(motor3>>8);
	CAN1_1FF_TxData[5] = (uint8_t) motor3;
	CAN1_1FF_TxData[6] = (uint8_t)(motor4>>8);
	CAN1_1FF_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_1FF_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_1FF_TxData,&TxMailbox);
	
	memset(CAN1_1FF_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN1_Send_2FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3)
{
	uint8_t cnt, res;
	
  TxMessage.StdId	=	0x2FF;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;											
	
	CAN1_2FF_TxData[0] = (uint8_t)(motor1>>8);
	CAN1_2FF_TxData[1] = (uint8_t) motor1;
	CAN1_2FF_TxData[2] = (uint8_t)(motor2>>8);
	CAN1_2FF_TxData[3] = (uint8_t) motor2;
	CAN1_2FF_TxData[4] = (uint8_t)(motor3>>8);
	CAN1_2FF_TxData[5] = (uint8_t) motor3;
	CAN1_2FF_TxData[6] = 0;
	CAN1_2FF_TxData[7] = 0;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_2FF_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_2FF_TxData,&TxMailbox);
	
	memset(CAN1_2FF_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN1_Send_1FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;
	
  TxMessage.StdId	=	0x1FE;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;											
	
	CAN1_1FE_TxData[0] = (uint8_t)(motor1>>8);
	CAN1_1FE_TxData[1] = (uint8_t) motor1;
	CAN1_1FE_TxData[2] = (uint8_t)(motor2>>8);
	CAN1_1FE_TxData[3] = (uint8_t) motor2;
	CAN1_1FE_TxData[4] = (uint8_t)(motor3>>8);
	CAN1_1FE_TxData[5] = (uint8_t) motor3;
	CAN1_1FE_TxData[6] = (uint8_t)(motor4>>8);
	CAN1_1FE_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_1FE_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_1FE_TxData,&TxMailbox);
	
	memset(CAN1_1FE_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN1_Send_2FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3)
{
	uint8_t cnt, res;
	
  TxMessage.StdId	=	0x2FE;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;											
	
	CAN1_2FE_TxData[0] = (uint8_t)(motor1>>8);
	CAN1_2FE_TxData[1] = (uint8_t) motor1;
	CAN1_2FE_TxData[2] = (uint8_t)(motor2>>8);
	CAN1_2FE_TxData[3] = (uint8_t) motor2;
	CAN1_2FE_TxData[4] = (uint8_t)(motor3>>8);
	CAN1_2FE_TxData[5] = (uint8_t) motor3;
	CAN1_2FE_TxData[6] = 0;
	CAN1_2FE_TxData[7] = 0;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN1_2FE_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan1,&TxMessage,CAN1_2FE_TxData,&TxMailbox);
	
	memset(CAN1_2FE_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN2_Send_200_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;		
	
  TxMessage.StdId	=	0x200;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;

	CAN2_200_TxData[0] = (uint8_t)(motor1>>8);
	CAN2_200_TxData[1] = (uint8_t) motor1;
	CAN2_200_TxData[2] = (uint8_t)(motor2>>8);
	CAN2_200_TxData[3] = (uint8_t) motor2;
	CAN2_200_TxData[4] = (uint8_t)(motor3>>8);
	CAN2_200_TxData[5] = (uint8_t) motor3;
	CAN2_200_TxData[6] = (uint8_t)(motor4>>8);
	CAN2_200_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN2_200_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan2,&TxMessage,CAN2_200_TxData,&TxMailbox);
	
	memset(CAN2_200_TxData, 0, sizeof(uint8_t)*8);
	
	return res;
}

uint8_t CAN2_Send_1FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;	
	
  TxMessage.StdId	=	0x1FF;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;							
  
	CAN2_1FF_TxData[0] = (uint8_t)(motor1>>8);
	CAN2_1FF_TxData[1] = (uint8_t) motor1;
	CAN2_1FF_TxData[2] = (uint8_t)(motor2>>8);
	CAN2_1FF_TxData[3] = (uint8_t) motor2;
	CAN2_1FF_TxData[4] = (uint8_t)(motor3>>8);
	CAN2_1FF_TxData[5] = (uint8_t) motor3;
	CAN2_1FF_TxData[6] = (uint8_t)(motor4>>8);
	CAN2_1FF_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN2_1FF_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan2,&TxMessage,CAN2_1FF_TxData,&TxMailbox);

	memset(CAN2_1FF_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN2_Send_2FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3)
{
	uint8_t cnt, res;	
	
  TxMessage.StdId	=	0x2FF;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;							
  
	CAN2_2FF_TxData[0] = (uint8_t)(motor1>>8);
	CAN2_2FF_TxData[1] = (uint8_t) motor1;
	CAN2_2FF_TxData[2] = (uint8_t)(motor2>>8);
	CAN2_2FF_TxData[3] = (uint8_t) motor2;
	CAN2_2FF_TxData[4] = (uint8_t)(motor3>>8);
	CAN2_2FF_TxData[5] = (uint8_t) motor3;
	CAN2_2FF_TxData[6] = 0;
	CAN2_2FF_TxData[7] = 0;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN2_2FF_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan2,&TxMessage,CAN2_2FF_TxData,&TxMailbox);

	memset(CAN2_2FF_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN2_Send_1FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4)
{
	uint8_t cnt, res;	
	
  TxMessage.StdId	=	0x1FE;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;							
  
	CAN2_1FE_TxData[0] = (uint8_t)(motor1>>8);
	CAN2_1FE_TxData[1] = (uint8_t) motor1;
	CAN2_1FE_TxData[2] = (uint8_t)(motor2>>8);
	CAN2_1FE_TxData[3] = (uint8_t) motor2;
	CAN2_1FE_TxData[4] = (uint8_t)(motor3>>8);
	CAN2_1FE_TxData[5] = (uint8_t) motor3;
	CAN2_1FE_TxData[6] = (uint8_t)(motor4>>8);
	CAN2_1FE_TxData[7] = (uint8_t) motor4;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN2_1FE_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan2,&TxMessage,CAN2_1FE_TxData,&TxMailbox);

	memset(CAN2_1FE_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}

uint8_t CAN2_Send_2FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3)
{
	uint8_t cnt, res;	
	
  TxMessage.StdId	=	0x2FE;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;							
  
	CAN2_2FE_TxData[0] = (uint8_t)(motor1>>8);
	CAN2_2FE_TxData[1] = (uint8_t) motor1;
	CAN2_2FE_TxData[2] = (uint8_t)(motor2>>8);
	CAN2_2FE_TxData[3] = (uint8_t) motor2;
	CAN2_2FE_TxData[4] = (uint8_t)(motor3>>8);
	CAN2_2FE_TxData[5] = (uint8_t) motor3;
	CAN2_2FE_TxData[6] = 0;
	CAN2_2FE_TxData[7] = 0;
	
	// don't send if every = 0
	for(cnt = 0;cnt < 8;cnt++)
	{
		if(CAN2_2FE_TxData[cnt] == 0)
			cnt++;
		if(cnt == 8)
			return 0;
	}
	
  res = HAL_CAN_AddTxMessage(&hcan2,&TxMessage,CAN2_2FE_TxData,&TxMailbox);

	memset(CAN2_2FE_TxData, 0, sizeof(uint8_t)*8);
		
	return res;
}
/* ---------- CAN Transmit Protocol Fuction end   ---------- */
