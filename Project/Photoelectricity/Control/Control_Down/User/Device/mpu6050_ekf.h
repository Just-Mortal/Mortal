#ifndef __MPU6050_EKF_H
#define __MPU6050_EKF_H

#include "kalman_filter.h"

/* 滤波器状态结构 */
typedef struct {
    uint8_t Initialized;
    KalmanFilter_t EKF;
    uint8_t ConvergeFlag;      // 收敛标志
    uint8_t StableFlag;         // 稳态标志
    uint64_t ErrorCount;
    uint64_t UpdateCount;
    
    float q[4];                 // 四元数
    float GyroBias[3];          // 陀螺仪偏置
    
    float Gyro[3];              // 原始陀螺仪数据
    float Accel[3];             // 原始加速度计数据
    
    float OrientationCosine[3]; // 方向余弦
    
    float Roll, Pitch, Yaw;     // 欧拉角(弧度)
    float YawTotalAngle;        // 总偏航角
    
    float Q1;                   // 过程噪声1
    float Q2;                   // 过程噪声2
    float R;                    // 测量噪声
    
    float dt;                   // 时间步长
    float lambda;               // 遗忘因子
    float ChiSquareTestThreshold; // 卡方检验阈值
    float ChiSquare_Data[1];    // 卡方检验数据
    float AdaptiveGainScale;    // 自适应增益比例
    float accLPFcoef;           // 加速度低通系数
    float gyro_norm;            // 陀螺仪模值
    float accl_norm;            // 加速度计模值
    
    int16_t YawRoundCount;      // 偏航圈数计数
    float YawAngleLast;         // 上一次偏航角
} MPUEKF_INS_t;

extern MPUEKF_INS_t MPUEKF_INS;

/* 函数声明 */
void MPU_EKF_Init(float* init_quat, float proc_noise1, float proc_noise2, 
                 float meas_noise, float lambda);
void MPU_EKF_Update(float gx, float gy, float gz, 
                   float ax, float ay, float az, float dt);

#endif
