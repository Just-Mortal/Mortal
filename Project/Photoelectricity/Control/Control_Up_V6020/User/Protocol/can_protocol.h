#ifndef __CAN_PROTOCOL_H
#define __CAN_PROTOCOL_H

#include "can_drv.h"

#include "motor.h"

uint8_t CAN1_Send_6020(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t CAN1_Send_4310(int16_t motor1,int16_t motor2,int16_t motor3,int16_t motor4);
uint8_t mit_ctrl(uint16_t motor_id, float tor);
uint8_t enable_motor_mode(uint16_t motor_id);
uint8_t clean_motor_err(uint16_t motor_id);

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#endif
