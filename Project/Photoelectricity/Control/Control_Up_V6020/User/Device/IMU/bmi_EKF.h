/**
 ******************************************************************************
 * @file    QuaternionEKF.h
 * @author  Wang Hongxi
 * @version V1.2.0
 * @date    2022/3/8
 * @brief   attitude update with gyro bias estimate and chi-square test
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#ifndef _BMI_EKF_H
#define _BMI_EKF_H
#include "kalman_filter.h"

/* boolean type definitions */
#ifndef TRUE
#define TRUE 1 /**< boolean true  */
#endif

#ifndef FALSE
#define FALSE 0 /**< boolean fails */
#endif

/* 陀螺仪坐标变换为云台坐标结构体 */
typedef struct
{
    float arz;
    float ary;
    float arx;
    float trans[9];
} gimbal_transform_t;

typedef struct
{
    uint8_t Initialized;
    KalmanFilter_t IMU_QuaternionEKF;
    uint8_t ConvergeFlag;//滤波器是否收敛标志变量
    uint8_t StableFlag;//运动状态是否稳定标志变量
    uint64_t ErrorCount;
    uint64_t UpdateCount;

    float q[4];        // 四元数估计值
    float GyroBias[3]; // 陀螺仪零偏估计值

    float Gyro[3];
    float Accel[3];

    float OrientationCosine[3];//预测值和各个轴的方向余弦

    float accLPFcoef;
    float gyro_norm;
    float accl_norm;
    float AdaptiveGainScale;

    float Roll;
    float Pitch;
    float Yaw;

    float YawTotalAngle;

    float Q1; // 四元数更新过程噪声
    float Q2; // 陀螺仪零偏过程噪声
    float R;  // 加速度计量测噪声

    float dt; // 姿态更新周期
    mat ChiSquare;
    float ChiSquare_Data[1];      // 卡方检验检测函数
    float ChiSquareTestThreshold; // 卡方检验阈值
    float lambda;                 // 渐消因子

    int16_t YawRoundCount;

    float YawAngleLast;
} QEKF_INS_t;

extern gimbal_transform_t EKFgim_trans;
extern QEKF_INS_t QEKF_INS;
extern float chiSquare;
extern float ChiSquareTestThreshold;
void IMU_QuaternionEKF_Init(float* init_quaternion,float process_noise1, float process_noise2, float measure_noise, float lambda);
void IMU_QuaternionEKF_Update(float gx, float gy, float gz, float ax, float ay, float az, float dt);
void transform_init(gimbal_transform_t *gim_trans);
void Vector_Transform(float gx, float gy, float gz,\
	                  float ax, float ay, float az,\
	                  float *ggx, float *ggy, float *ggz,\
					  float *aax, float *aay, float *aaz);
void BMI_Get_Acceleration(float pitch, float roll, float yaw,\
						  float ax, float ay, float az,\
						  float *accx, float *accy, float *accz);

#endif
