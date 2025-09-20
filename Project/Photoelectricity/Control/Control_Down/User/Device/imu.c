#include "imu.h"
#include "math.h"

/* 全局变量 */
imu_data_t imu_data;

static I2C_HandleTypeDef *imu_i2c;
static uint32_t calibration_count = 0;
static float gyro_bias[3] = {0};

static void imu_calibrate(void);

/* MPU6050寄存器定义 */
#define SMPLRT_DIV     0x19
#define CONFIG         0x1A
#define GYRO_CONFIG    0x1B
#define ACCEL_CONFIG   0x1C
#define ACCEL_XOUT_H   0x3B
#define TEMP_OUT_H     0x41
#define GYRO_XOUT_H    0x43
#define PWR_MGMT_1     0x6B
#define WHO_AM_I       0x75

/* 初始化MPU6050 */
static uint8_t mpu6050_init(void)
{
    uint8_t check, data;
    
    // 检查设备ID
    HAL_I2C_Mem_Read(imu_i2c, MPU6050_ADDR, WHO_AM_I, 1, &check, 1, MPU6050_TIMEOUT);
    if (check != 0x68) return 0;
    
    // 唤醒设备
    data = 0x00;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, PWR_MGMT_1, 1, &data, 1, MPU6050_TIMEOUT);
		
    // 设置采样率
    data = 0x07;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, SMPLRT_DIV, 1, &data, 1, MPU6050_TIMEOUT);
    
    // 设置DLPF
    data = 0x06;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    // 设置陀螺仪量程 ±1000°/s
    data = 0x10;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, GYRO_CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    // 设置加速度计量程 ±4g
    data = 0x08;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, ACCEL_CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    return 1;
}

/* 读取原始数据 */
static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t data[14];
    
    // 读取所有传感器数据
    HAL_I2C_Mem_Read(imu_i2c, MPU6050_ADDR, ACCEL_XOUT_H, 1, data, 14, MPU6050_TIMEOUT);
    
    // 加速度计数据
    accel[0] = (int16_t)((data[0] << 8) | data[1]);
    accel[1] = (int16_t)((data[2] << 8) | data[3]);
    accel[2] = (int16_t)((data[4] << 8) | data[5]);
    
    // 温度数据
    *temp = (int16_t)((data[6] << 8) | data[7]);
    
    // 陀螺仪数据
    gyro[0] = (int16_t)((data[8] << 8) | data[9]);
    gyro[1] = (int16_t)((data[10] << 8) | data[11]);
    gyro[2] = (int16_t)((data[12] << 8) | data[12]);
}

/* IMU初始化 */
imu_status_t imu_init(I2C_HandleTypeDef *hi2c)
{
    imu_i2c = hi2c;
    
    // 初始化MPU6050
    if (!mpu6050_init()) {
        return IMU_NOT_DETECTED;
    }
    
    // 初始化EKF
    float init_quat[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    MPU_EKF_Init(init_quat, 0.1f, 0.0001f, 10000.0f, 0.999f);
    
    // 初始校准
    imu_calibrate();
    
    return IMU_OK;
}

/* IMU校准 */
void imu_calibrate(void)
{
    calibration_count = 0;
    memset(gyro_bias, 0, sizeof(gyro_bias));
}

/* IMU数据更新 */
void imu_update(float dt)
{
    int16_t raw_accel[3], raw_gyro[3], raw_temp;
    float accel[3], gyro[3];
    
    // 读取原始数据
    mpu6050_read_raw(raw_accel, raw_gyro, &raw_temp);
    
    // 转换加速度数据 (m/s2)
    const float accel_scale = 4.0f / 32768.0f * 9.80665f;
    accel[0] = raw_accel[0] * accel_scale;
    accel[1] = raw_accel[1] * accel_scale;
    accel[2] = raw_accel[2] * accel_scale;
    
    // 转换陀螺仪数据 (rad/s)
    const float gyro_scale = 1000.0f / 32768.0f * (PI / 180.0f);
    gyro[0] = raw_gyro[0] * gyro_scale;
    gyro[1] = raw_gyro[1] * gyro_scale;
    gyro[2] = raw_gyro[2] * gyro_scale;
    
    // 转换温度数据
    imu_data.temperature = (raw_temp / 340.0f) + 36.53f;
    
    // 校准阶段
    if (calibration_count < 1000) {
        gyro_bias[0] += gyro[0];
        gyro_bias[1] += gyro[1];
        gyro_bias[2] += gyro[2];
        calibration_count++;
        
        if (calibration_count == 1000) {
            gyro_bias[0] /= 1000.0f;
            gyro_bias[1] /= 1000.0f;
            gyro_bias[2] /= 1000.0f;
        }
        return;
    }
    
    // 应用零偏校准
    gyro[0] -= gyro_bias[0];
    gyro[1] -= gyro_bias[1];
    gyro[2] -= gyro_bias[2];
    
    // 更新EKF
    MPU_EKF_Update(gyro[0], gyro[1], gyro[2], 
                   accel[0], accel[1], accel[2], 
                   dt);
    
    // 获取结果
    imu_data.roll = MPUEKF_INS.Roll;
    imu_data.pitch = MPUEKF_INS.Pitch;
    imu_data.yaw = MPUEKF_INS.Yaw;
    imu_data.yaw_total = MPUEKF_INS.YawTotalAngle;
    
    // 获取角速度 (度/秒)
    imu_data.rate_roll = (gyro[0] - MPUEKF_INS.GyroBias[0]) * 57.295779513f;
    imu_data.rate_pitch = (gyro[1] - MPUEKF_INS.GyroBias[1]) * 57.295779513f;
    imu_data.rate_yaw = (gyro[2] - MPUEKF_INS.GyroBias[2]) * 57.295779513f;
    
    // 保存加速度数据
    imu_data.acc_x = accel[0];
    imu_data.acc_y = accel[1];
    imu_data.acc_z = accel[2];
}
