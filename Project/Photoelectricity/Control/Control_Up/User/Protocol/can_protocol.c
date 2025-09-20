#include "can_protocol.h"

uint32_t TxMailbox;

uint8_t CAN1_6020[8];
uint8_t CAN1_4310[8];

CAN_TxHeaderTypeDef TxMessage;

static float uint_to_float(int x_int, float x_min, float x_max, int bits)
{
	/* converts unsigned int to float, given range and number of bits */
	float span = x_max - x_min;
	float offset = x_min;
	return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

static int float_to_uint(float x_float, float x_min, float x_max, int bits)
{
    /* Converts a float to an unsigned int, given range and number of bits */
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x_float-offset)*((float)((1<<bits)-1))/span);
}

void dm_motor_data(motor_t *motor, uint8_t *rx_data)
{
    motor->id.rx_id				= (rx_data[0])&0x0F;
    motor->rx_info.state	= (rx_data[0])>>4;
    uint16_t dm_angle	=(rx_data[1]<<8)|rx_data[2];
    uint16_t dm_speed	=(rx_data[3]<<4)|(rx_data[4]>>4);
    uint16_t dm_torque	=((rx_data[4]&0xF)<<8)|rx_data[5];
		motor->rx_info.angle = uint_to_float(dm_angle,	-3.141593, +3.141593, 16); // (-12.5,12.5)
    motor->rx_info.speed = uint_to_float(dm_speed,		-30.f, +30.f, 12);
    motor->rx_info.torque = uint_to_float(dm_torque,	-10.f, +10.f, 12);
    motor->rx_info.temperature 	= (float)(rx_data[6]);
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
		case GIM_4310:
			dm_motor_data(&motor[GIMBAL_PCH], rxBuf);
			motor[GIMBAL_PCH].state.offline_cnt = 0;
			break;
		
		case GIM_6020:
			motor[GIMBAL_YAW].rx(&motor[GIMBAL_YAW], rxBuf, 8);
			motor[GIMBAL_YAW].state.offline_cnt = 0;
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

uint8_t mit_ctrl(uint16_t motor_id, float tor)
{
	uint8_t res;	
	uint8_t data[8];
//	uint16_t pos_tmp, vel_tmp, kp_tmp, kd_tmp, tor_tmp;
	uint16_t tor_tmp;
	
	TxMessage.StdId	=	motor_id;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;

	tor_tmp = float_to_uint(tor, -10.f, +10.f, 12);
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0x00;
	data[4] = 0x00;
	data[5] = 0x00;
	data[6] = tor_tmp>>8;
	data[7] = tor_tmp;
	
	res = HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &TxMailbox);
	
	return res;
}

uint8_t enable_motor_mode(uint16_t motor_id)
{
	uint8_t res;	
	uint8_t data[8];
	
	TxMessage.StdId	=	motor_id;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFC;
	
	res = HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &TxMailbox);
	
	return res;
}

uint8_t clean_motor_err(uint16_t motor_id)
{
	uint8_t res;	
	uint8_t data[8];
	
	TxMessage.StdId	=	motor_id;
	TxMessage.IDE		=	CAN_ID_STD;
	TxMessage.RTR		=	CAN_RTR_DATA;
	TxMessage.DLC		=	8;
	
	data[0] = 0xFF;
	data[1] = 0xFF;
	data[2] = 0xFF;
	data[3] = 0xFF;
	data[4] = 0xFF;
	data[5] = 0xFF;
	data[6] = 0xFF;
	data[7] = 0xFB;
	
	res = HAL_CAN_AddTxMessage(&hcan1, &TxMessage, data, &TxMailbox);
	
	return res;
}
/* ---------- CAN Transmit Protocol Fuction end   ---------- */
