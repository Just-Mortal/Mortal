#include "mpu6050_ekf.h"
#include "arm_math.h"
#include <stdbool.h>
#include <string.h>  // ���memsetͷ�ļ�

#define GRAVITY_EARTH (9.80665f)

/* ȫ��ʵ�� */
MPUEKF_INS_t MPUEKF_INS;

/* ��̬�������� */
static void MPU_EKF_Observe(KalmanFilter_t *kf);
static void MPU_EKF_F_Linearization_P_Fading(KalmanFilter_t *kf);
static void MPU_EKF_SetH(KalmanFilter_t *kf);
static void Quat_To_Euler(float q[4], float* roll, float* pitch, float* yaw);

void MPU_EKF_Init(float* init_quat, float proc_noise1, float proc_noise2, 
                 float meas_noise, float lambda)
{
    MPUEKF_INS.Initialized = 1;
    MPUEKF_INS.Q1 = proc_noise1;
    MPUEKF_INS.Q2 = proc_noise2;
    MPUEKF_INS.R = meas_noise;
    MPUEKF_INS.ChiSquareTestThreshold = 1e-8;
    MPUEKF_INS.ConvergeFlag = 0;
    MPUEKF_INS.ErrorCount = 0;
    MPUEKF_INS.UpdateCount = 0;
    
    if (lambda > 1) lambda = 1;
    MPUEKF_INS.lambda = lambda;

    // ��ʼ���������˲���
    Kalman_Filter_Init(&MPUEKF_INS.EKF, 6, 0, 3);
    
    // ��ʼ����Ԫ��
    for(int i = 0; i < 4; i++) {
        MPUEKF_INS.EKF.xhat_data[i] = init_quat[i];
    }
    MPUEKF_INS.EKF.xhat_data[4] = 0; // ������ƫ��X
    MPUEKF_INS.EKF.xhat_data[5] = 0; // ������ƫ��Y

    // �����û��Զ��庯��
    MPUEKF_INS.EKF.User_Func0_f = MPU_EKF_Observe;
    MPUEKF_INS.EKF.User_Func1_f = MPU_EKF_F_Linearization_P_Fading;
    MPUEKF_INS.EKF.User_Func2_f = MPU_EKF_SetH;
    
    // ��Ҫ�޸���ȷ�����������ȫ
    MPUEKF_INS.EKF.UseAutoAdjustment = 0; // �����Զ���������
    memset(MPUEKF_INS.EKF.StateMinVariance, 0.1f, sizeof(float)*6); // ������С����
    
    // ��ʼ��Э�������
    for(int i = 0; i < 36; i++) {
        MPUEKF_INS.EKF.P_data[i] = (i % 7 == 0) ? 100.0f : 0.1f; // �Խ��߳�ʼ��
    }
}

void MPU_EKF_Update(float gx, float gy, float gz, 
                   float ax, float ay, float az, float dt)
{
    float halfgxdt, halfgydt, halfgzdt;
    float accelInvNorm;

    MPUEKF_INS.dt = dt;
    
    // Ӧ��ƫ�ò���
    MPUEKF_INS.Gyro[0] = gx - MPUEKF_INS.GyroBias[0];
    MPUEKF_INS.Gyro[1] = gy - MPUEKF_INS.GyroBias[1];
    MPUEKF_INS.Gyro[2] = gz - MPUEKF_INS.GyroBias[2];
    
    // ����״̬ת�ƾ���
    halfgxdt = 0.5f * MPUEKF_INS.Gyro[0] * dt;
    halfgydt = 0.5f * MPUEKF_INS.Gyro[1] * dt;
    halfgzdt = 0.5f * MPUEKF_INS.Gyro[2] * dt;

    // ����״̬ת�ƾ��� (6x6)
    float F[36] = {
        1, -halfgxdt, -halfgydt, -halfgzdt, 0, 0,
        halfgxdt, 1, halfgzdt, -halfgydt, 0, 0,
        halfgydt, -halfgzdt, 1, halfgxdt, 0, 0,
        halfgzdt, halfgydt, -halfgxdt, 1, 0, 0,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1
    };
    memcpy(MPUEKF_INS.EKF.F_data, F, sizeof(F));

    // ������ٶ�����
    MPUEKF_INS.Accel[0] = ax;
    MPUEKF_INS.Accel[1] = ay;
    MPUEKF_INS.Accel[2] = az;
    
    // ������ٶȷ�������һ��
    arm_sqrt_f32(ax*ax + ay*ay + az*az, &MPUEKF_INS.accl_norm);
    if (MPUEKF_INS.accl_norm > 0.01f) {
        accelInvNorm = 1.0f / MPUEKF_INS.accl_norm;
        for (uint8_t i = 0; i < 3; ++i) {
            MPUEKF_INS.EKF.MeasuredVector[i] = MPUEKF_INS.Accel[i] * accelInvNorm;
        }
    }

    // ���ù����������� (6x6)
    memset(MPUEKF_INS.EKF.Q_data, 0, sizeof(float)*36);
    for(uint8_t i = 0; i < 4; i++) {
        MPUEKF_INS.EKF.Q_data[i*6+i] = MPUEKF_INS.Q1 * dt;
    }
    MPUEKF_INS.EKF.Q_data[4 * 6+4] = MPUEKF_INS.Q2 * dt;
    MPUEKF_INS.EKF.Q_data[5 * 6+5] = MPUEKF_INS.Q2 * dt;
    
    // ���ò����������� (3x3)
    memset(MPUEKF_INS.EKF.R_data, 0, sizeof(float)*9);
    for(uint8_t i = 0; i < 3; i++) {
        MPUEKF_INS.EKF.R_data[i*3+i] = MPUEKF_INS.R;
    }

    // ִ�п������˲�����
    Kalman_Filter_Update(&MPUEKF_INS.EKF);

    // ��ȡ���
    for(int i=0; i<4; i++) {
        MPUEKF_INS.q[i] = MPUEKF_INS.EKF.FilteredValue[i];
    }
    MPUEKF_INS.GyroBias[0] = MPUEKF_INS.EKF.FilteredValue[4];
    MPUEKF_INS.GyroBias[1] = MPUEKF_INS.EKF.FilteredValue[5];
    MPUEKF_INS.GyroBias[2] = 0;  // Zƫ�ò�����

    // ת��Ϊŷ����
    Quat_To_Euler(MPUEKF_INS.q, &MPUEKF_INS.Roll, &MPUEKF_INS.Pitch, &MPUEKF_INS.Yaw);
    
    // ����ת�Ƕ�
    const float rad2deg = 57.295779513f;
    MPUEKF_INS.Roll *= rad2deg;
    MPUEKF_INS.Pitch *= rad2deg;
    MPUEKF_INS.Yaw *= rad2deg;
    
    // ����ƫ�����ۻ�
    if (MPUEKF_INS.Yaw - MPUEKF_INS.YawAngleLast > 180.0f) {
        MPUEKF_INS.YawRoundCount--;
    } else if (MPUEKF_INS.Yaw - MPUEKF_INS.YawAngleLast < -180.0f) {
        MPUEKF_INS.YawRoundCount++;
    }
    
    MPUEKF_INS.YawTotalAngle = 360.0f * MPUEKF_INS.YawRoundCount + MPUEKF_INS.Yaw;
    MPUEKF_INS.YawAngleLast = MPUEKF_INS.Yaw;
    MPUEKF_INS.UpdateCount++;
}

/* �۲⺯�� (��ʵ��) */
static void MPU_EKF_Observe(KalmanFilter_t *kf)
{
    // ���ڴ�����Զ���۲��߼�
}

/* ״̬ת�ƾ������Ի���P����˥�� */
static void MPU_EKF_F_Linearization_P_Fading(KalmanFilter_t *kf)
{
    // ��һ����Ԫ��
    arm_quaternion_normalize_f32(kf->xhatminus_data, kf->xhatminus_data, 1);
    
    // P����˥��
    for(uint8_t i = 4; i < 6; i++) {
        kf->Pminus_data[i*6+i] /= MPUEKF_INS.lambda;
        if (kf->Pminus_data[i*6+i] > 10000) 
            kf->Pminus_data[i*6+i] = 10000;
    }
}

/* ���ù۲����H */
static void MPU_EKF_SetH(KalmanFilter_t *kf)
{
    float doubleq0 = 2 * kf->xhatminus_data[0];
    float doubleq1 = 2 * kf->xhatminus_data[1];
    float doubleq2 = 2 * kf->xhatminus_data[2];
    float doubleq3 = 2 * kf->xhatminus_data[3];
    
    // H���� (3x6)
    float H[18] = {
        -doubleq2, doubleq3, -doubleq0, doubleq1, 0, 0,
        doubleq1, doubleq0, doubleq3, doubleq2, 0, 0,
        doubleq0, -doubleq1, -doubleq2, doubleq3, 0, 0
    };
    
    memcpy(kf->H_data, H, sizeof(H));
}

/* ��Ԫ��תŷ���� */
static void Quat_To_Euler(float q[4], float* roll, float* pitch, float* yaw)
{
    // ��Ԫ��תŷ���� (Z-Y-X˳��)
    *roll = atan2f(2*(q[0]*q[1] + q[2]*q[3]), 1 - 2*(q[1]*q[1] + q[2]*q[2]));
    *pitch = asinf(2*(q[0]*q[2] - q[3]*q[1]));
    *yaw = atan2f(2*(q[0]*q[3] + q[1]*q[2]), 1 - 2*(q[2]*q[2] + q[3]*q[3]));
}
