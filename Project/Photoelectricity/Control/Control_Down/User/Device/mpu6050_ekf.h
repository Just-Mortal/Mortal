#ifndef __MPU6050_EKF_H
#define __MPU6050_EKF_H

#include "kalman_filter.h"

/* �˲���״̬�ṹ */
typedef struct {
    uint8_t Initialized;
    KalmanFilter_t EKF;
    uint8_t ConvergeFlag;      // ������־
    uint8_t StableFlag;         // ��̬��־
    uint64_t ErrorCount;
    uint64_t UpdateCount;
    
    float q[4];                 // ��Ԫ��
    float GyroBias[3];          // ������ƫ��
    
    float Gyro[3];              // ԭʼ����������
    float Accel[3];             // ԭʼ���ٶȼ�����
    
    float OrientationCosine[3]; // ��������
    
    float Roll, Pitch, Yaw;     // ŷ����(����)
    float YawTotalAngle;        // ��ƫ����
    
    float Q1;                   // ��������1
    float Q2;                   // ��������2
    float R;                    // ��������
    
    float dt;                   // ʱ�䲽��
    float lambda;               // ��������
    float ChiSquareTestThreshold; // ����������ֵ
    float ChiSquare_Data[1];    // ������������
    float AdaptiveGainScale;    // ����Ӧ�������
    float accLPFcoef;           // ���ٶȵ�ͨϵ��
    float gyro_norm;            // ������ģֵ
    float accl_norm;            // ���ٶȼ�ģֵ
    
    int16_t YawRoundCount;      // ƫ��Ȧ������
    float YawAngleLast;         // ��һ��ƫ����
} MPUEKF_INS_t;

extern MPUEKF_INS_t MPUEKF_INS;

/* �������� */
void MPU_EKF_Init(float* init_quat, float proc_noise1, float proc_noise2, 
                 float meas_noise, float lambda);
void MPU_EKF_Update(float gx, float gy, float gz, 
                   float ax, float ay, float az, float dt);

#endif
