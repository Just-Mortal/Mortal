#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx_hal.h"
#include "mpu6050_ekf.h"

/* ����MPU6050��ز��� */
#define MPU6050_ADDR        0xD0
#define MPU6050_TIMEOUT     100

/* IMU���ݽṹ */
typedef struct
{
    float roll;         // �����(��)
    float pitch;        // ������(��)
    float yaw;          // ƫ����(��)
    float yaw_total;    // ��ƫ����(�ȣ�������Ȧ)
    
    float rate_roll;    // ������ٶ�(��/��)
    float rate_pitch;   // �������ٶ�(��/��)
    float rate_yaw;     // ƫ�����ٶ�(��/��)
    
    float acc_x;        // X����ٶ�(m/s2)
    float acc_y;        // Y����ٶ�(m/s2)
    float acc_z;        // Z����ٶ�(m/s2)
    
    float temperature;  // �¶�(���϶�)
} imu_data_t;

/* IMU����״̬ */
typedef enum
{
    IMU_OK,
    IMU_NOT_DETECTED,
    IMU_CALIBRATING,
    IMU_ERROR
} imu_status_t;

/* �������� */
imu_status_t imu_init(I2C_HandleTypeDef *hi2c);
void imu_update(float dt);

extern imu_data_t imu_data;

#endif
