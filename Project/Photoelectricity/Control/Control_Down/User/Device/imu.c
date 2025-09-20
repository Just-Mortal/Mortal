#include "imu.h"
#include "math.h"

/* ȫ�ֱ��� */
imu_data_t imu_data;

static I2C_HandleTypeDef *imu_i2c;
static uint32_t calibration_count = 0;
static float gyro_bias[3] = {0};

static void imu_calibrate(void);

/* MPU6050�Ĵ������� */
#define SMPLRT_DIV     0x19
#define CONFIG         0x1A
#define GYRO_CONFIG    0x1B
#define ACCEL_CONFIG   0x1C
#define ACCEL_XOUT_H   0x3B
#define TEMP_OUT_H     0x41
#define GYRO_XOUT_H    0x43
#define PWR_MGMT_1     0x6B
#define WHO_AM_I       0x75

/* ��ʼ��MPU6050 */
static uint8_t mpu6050_init(void)
{
    uint8_t check, data;
    
    // ����豸ID
    HAL_I2C_Mem_Read(imu_i2c, MPU6050_ADDR, WHO_AM_I, 1, &check, 1, MPU6050_TIMEOUT);
    if (check != 0x68) return 0;
    
    // �����豸
    data = 0x00;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, PWR_MGMT_1, 1, &data, 1, MPU6050_TIMEOUT);
		
    // ���ò�����
    data = 0x07;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, SMPLRT_DIV, 1, &data, 1, MPU6050_TIMEOUT);
    
    // ����DLPF
    data = 0x06;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    // �������������� ��1000��/s
    data = 0x10;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, GYRO_CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    // ���ü��ٶȼ����� ��4g
    data = 0x08;
    HAL_I2C_Mem_Write(imu_i2c, MPU6050_ADDR, ACCEL_CONFIG, 1, &data, 1, MPU6050_TIMEOUT);
    
    return 1;
}

/* ��ȡԭʼ���� */
static void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp)
{
    uint8_t data[14];
    
    // ��ȡ���д���������
    HAL_I2C_Mem_Read(imu_i2c, MPU6050_ADDR, ACCEL_XOUT_H, 1, data, 14, MPU6050_TIMEOUT);
    
    // ���ٶȼ�����
    accel[0] = (int16_t)((data[0] << 8) | data[1]);
    accel[1] = (int16_t)((data[2] << 8) | data[3]);
    accel[2] = (int16_t)((data[4] << 8) | data[5]);
    
    // �¶�����
    *temp = (int16_t)((data[6] << 8) | data[7]);
    
    // ����������
    gyro[0] = (int16_t)((data[8] << 8) | data[9]);
    gyro[1] = (int16_t)((data[10] << 8) | data[11]);
    gyro[2] = (int16_t)((data[12] << 8) | data[12]);
}

/* IMU��ʼ�� */
imu_status_t imu_init(I2C_HandleTypeDef *hi2c)
{
    imu_i2c = hi2c;
    
    // ��ʼ��MPU6050
    if (!mpu6050_init()) {
        return IMU_NOT_DETECTED;
    }
    
    // ��ʼ��EKF
    float init_quat[4] = {1.0f, 0.0f, 0.0f, 0.0f};
    MPU_EKF_Init(init_quat, 0.1f, 0.0001f, 10000.0f, 0.999f);
    
    // ��ʼУ׼
    imu_calibrate();
    
    return IMU_OK;
}

/* IMUУ׼ */
void imu_calibrate(void)
{
    calibration_count = 0;
    memset(gyro_bias, 0, sizeof(gyro_bias));
}

/* IMU���ݸ��� */
void imu_update(float dt)
{
    int16_t raw_accel[3], raw_gyro[3], raw_temp;
    float accel[3], gyro[3];
    
    // ��ȡԭʼ����
    mpu6050_read_raw(raw_accel, raw_gyro, &raw_temp);
    
    // ת�����ٶ����� (m/s2)
    const float accel_scale = 4.0f / 32768.0f * 9.80665f;
    accel[0] = raw_accel[0] * accel_scale;
    accel[1] = raw_accel[1] * accel_scale;
    accel[2] = raw_accel[2] * accel_scale;
    
    // ת������������ (rad/s)
    const float gyro_scale = 1000.0f / 32768.0f * (PI / 180.0f);
    gyro[0] = raw_gyro[0] * gyro_scale;
    gyro[1] = raw_gyro[1] * gyro_scale;
    gyro[2] = raw_gyro[2] * gyro_scale;
    
    // ת���¶�����
    imu_data.temperature = (raw_temp / 340.0f) + 36.53f;
    
    // У׼�׶�
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
    
    // Ӧ����ƫУ׼
    gyro[0] -= gyro_bias[0];
    gyro[1] -= gyro_bias[1];
    gyro[2] -= gyro_bias[2];
    
    // ����EKF
    MPU_EKF_Update(gyro[0], gyro[1], gyro[2], 
                   accel[0], accel[1], accel[2], 
                   dt);
    
    // ��ȡ���
    imu_data.roll = MPUEKF_INS.Roll;
    imu_data.pitch = MPUEKF_INS.Pitch;
    imu_data.yaw = MPUEKF_INS.Yaw;
    imu_data.yaw_total = MPUEKF_INS.YawTotalAngle;
    
    // ��ȡ���ٶ� (��/��)
    imu_data.rate_roll = (gyro[0] - MPUEKF_INS.GyroBias[0]) * 57.295779513f;
    imu_data.rate_pitch = (gyro[1] - MPUEKF_INS.GyroBias[1]) * 57.295779513f;
    imu_data.rate_yaw = (gyro[2] - MPUEKF_INS.GyroBias[2]) * 57.295779513f;
    
    // ������ٶ�����
    imu_data.acc_x = accel[0];
    imu_data.acc_y = accel[1];
    imu_data.acc_z = accel[2];
}
