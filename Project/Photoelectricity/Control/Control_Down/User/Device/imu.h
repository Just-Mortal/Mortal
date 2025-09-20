#ifndef __IMU_H
#define __IMU_H

#include "stm32f4xx_hal.h"
#include "mpu6050_ekf.h"

/* 定义MPU6050相关参数 */
#define MPU6050_ADDR        0xD0
#define MPU6050_TIMEOUT     100

/* IMU数据结构 */
typedef struct
{
    float roll;         // 横滚角(度)
    float pitch;        // 俯仰角(度)
    float yaw;          // 偏航角(度)
    float yaw_total;    // 总偏航角(度，连续多圈)
    
    float rate_roll;    // 横滚角速度(度/秒)
    float rate_pitch;   // 俯仰角速度(度/秒)
    float rate_yaw;     // 偏航角速度(度/秒)
    
    float acc_x;        // X轴加速度(m/s2)
    float acc_y;        // Y轴加速度(m/s2)
    float acc_z;        // Z轴加速度(m/s2)
    
    float temperature;  // 温度(摄氏度)
} imu_data_t;

/* IMU工作状态 */
typedef enum
{
    IMU_OK,
    IMU_NOT_DETECTED,
    IMU_CALIBRATING,
    IMU_ERROR
} imu_status_t;

/* 函数声明 */
imu_status_t imu_init(I2C_HandleTypeDef *hi2c);
void imu_update(float dt);

extern imu_data_t imu_data;

#endif
