#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "can_drv.h"

#include "motor.h"

uint8_t CAN1_Send_200_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN1_Send_1FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN1_Send_2FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3);
uint8_t CAN1_Send_1FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN1_Send_2FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3);
uint8_t CAN2_Send_200_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN2_Send_1FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN2_Send_2FF_Motor(int16_t motor1,int16_t motor2,int16_t motor3);
uint8_t CAN2_Send_1FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN2_Send_2FE_Motor(int16_t motor1,int16_t motor2,int16_t motor3);

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#endif
