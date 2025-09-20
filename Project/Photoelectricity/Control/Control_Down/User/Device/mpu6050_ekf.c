#include "mpu6050_ekf.h"
#include "arm_math.h"
#include <stdbool.h>
#include <string.h>  // 添加memset头文件

#define GRAVITY_EARTH (9.80665f)

/* 全局实例 */
MPUEKF_INS_t MPUEKF_INS;

/* 静态函数声明 */
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

    // 初始化卡尔曼滤波器
    Kalman_Filter_Init(&MPUEKF_INS.EKF, 6, 0, 3);
    
    // 初始化四元数
    for(int i = 0; i < 4; i++) {
        MPUEKF_INS.EKF.xhat_data[i] = init_quat[i];
    }
    MPUEKF_INS.EKF.xhat_data[4] = 0; // 陀螺仪偏置X
    MPUEKF_INS.EKF.xhat_data[5] = 0; // 陀螺仪偏置Y

    // 设置用户自定义函数
    MPUEKF_INS.EKF.User_Func0_f = MPU_EKF_Observe;
    MPUEKF_INS.EKF.User_Func1_f = MPU_EKF_F_Linearization_P_Fading;
    MPUEKF_INS.EKF.User_Func2_f = MPU_EKF_SetH;
    
    // 重要修复：确保矩阵操作安全
    MPUEKF_INS.EKF.UseAutoAdjustment = 0; // 禁用自动调整功能
    memset(MPUEKF_INS.EKF.StateMinVariance, 0.1f, sizeof(float)*6); // 设置最小方差
    
    // 初始化协方差矩阵
    for(int i = 0; i < 36; i++) {
        MPUEKF_INS.EKF.P_data[i] = (i % 7 == 0) ? 100.0f : 0.1f; // 对角线初始化
    }
}

void MPU_EKF_Update(float gx, float gy, float gz, 
                   float ax, float ay, float az, float dt)
{
    float halfgxdt, halfgydt, halfgzdt;
    float accelInvNorm;

    MPUEKF_INS.dt = dt;
    
    // 应用偏置补偿
    MPUEKF_INS.Gyro[0] = gx - MPUEKF_INS.GyroBias[0];
    MPUEKF_INS.Gyro[1] = gy - MPUEKF_INS.GyroBias[1];
    MPUEKF_INS.Gyro[2] = gz - MPUEKF_INS.GyroBias[2];
    
    // 构建状态转移矩阵
    halfgxdt = 0.5f * MPUEKF_INS.Gyro[0] * dt;
    halfgydt = 0.5f * MPUEKF_INS.Gyro[1] * dt;
    halfgzdt = 0.5f * MPUEKF_INS.Gyro[2] * dt;

    // 设置状态转移矩阵 (6x6)
    float F[36] = {
        1, -halfgxdt, -halfgydt, -halfgzdt, 0, 0,
        halfgxdt, 1, halfgzdt, -halfgydt, 0, 0,
        halfgydt, -halfgzdt, 1, halfgxdt, 0, 0,
        halfgzdt, halfgydt, -halfgxdt, 1, 0, 0,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1
    };
    memcpy(MPUEKF_INS.EKF.F_data, F, sizeof(F));

    // 处理加速度数据
    MPUEKF_INS.Accel[0] = ax;
    MPUEKF_INS.Accel[1] = ay;
    MPUEKF_INS.Accel[2] = az;
    
    // 计算加速度范数并归一化
    arm_sqrt_f32(ax*ax + ay*ay + az*az, &MPUEKF_INS.accl_norm);
    if (MPUEKF_INS.accl_norm > 0.01f) {
        accelInvNorm = 1.0f / MPUEKF_INS.accl_norm;
        for (uint8_t i = 0; i < 3; ++i) {
            MPUEKF_INS.EKF.MeasuredVector[i] = MPUEKF_INS.Accel[i] * accelInvNorm;
        }
    }

    // 设置过程噪声矩阵 (6x6)
    memset(MPUEKF_INS.EKF.Q_data, 0, sizeof(float)*36);
    for(uint8_t i = 0; i < 4; i++) {
        MPUEKF_INS.EKF.Q_data[i*6+i] = MPUEKF_INS.Q1 * dt;
    }
    MPUEKF_INS.EKF.Q_data[4 * 6+4] = MPUEKF_INS.Q2 * dt;
    MPUEKF_INS.EKF.Q_data[5 * 6+5] = MPUEKF_INS.Q2 * dt;
    
    // 设置测量噪声矩阵 (3x3)
    memset(MPUEKF_INS.EKF.R_data, 0, sizeof(float)*9);
    for(uint8_t i = 0; i < 3; i++) {
        MPUEKF_INS.EKF.R_data[i*3+i] = MPUEKF_INS.R;
    }

    // 执行卡尔曼滤波更新
    Kalman_Filter_Update(&MPUEKF_INS.EKF);

    // 获取结果
    for(int i=0; i<4; i++) {
        MPUEKF_INS.q[i] = MPUEKF_INS.EKF.FilteredValue[i];
    }
    MPUEKF_INS.GyroBias[0] = MPUEKF_INS.EKF.FilteredValue[4];
    MPUEKF_INS.GyroBias[1] = MPUEKF_INS.EKF.FilteredValue[5];
    MPUEKF_INS.GyroBias[2] = 0;  // Z偏置不估计

    // 转换为欧拉角
    Quat_To_Euler(MPUEKF_INS.q, &MPUEKF_INS.Roll, &MPUEKF_INS.Pitch, &MPUEKF_INS.Yaw);
    
    // 弧度转角度
    const float rad2deg = 57.295779513f;
    MPUEKF_INS.Roll *= rad2deg;
    MPUEKF_INS.Pitch *= rad2deg;
    MPUEKF_INS.Yaw *= rad2deg;
    
    // 处理偏航角累积
    if (MPUEKF_INS.Yaw - MPUEKF_INS.YawAngleLast > 180.0f) {
        MPUEKF_INS.YawRoundCount--;
    } else if (MPUEKF_INS.Yaw - MPUEKF_INS.YawAngleLast < -180.0f) {
        MPUEKF_INS.YawRoundCount++;
    }
    
    MPUEKF_INS.YawTotalAngle = 360.0f * MPUEKF_INS.YawRoundCount + MPUEKF_INS.Yaw;
    MPUEKF_INS.YawAngleLast = MPUEKF_INS.Yaw;
    MPUEKF_INS.UpdateCount++;
}

/* 观测函数 (空实现) */
static void MPU_EKF_Observe(KalmanFilter_t *kf)
{
    // 可在此添加自定义观测逻辑
}

/* 状态转移矩阵线性化和P矩阵衰减 */
static void MPU_EKF_F_Linearization_P_Fading(KalmanFilter_t *kf)
{
    // 归一化四元数
    arm_quaternion_normalize_f32(kf->xhatminus_data, kf->xhatminus_data, 1);
    
    // P矩阵衰减
    for(uint8_t i = 4; i < 6; i++) {
        kf->Pminus_data[i*6+i] /= MPUEKF_INS.lambda;
        if (kf->Pminus_data[i*6+i] > 10000) 
            kf->Pminus_data[i*6+i] = 10000;
    }
}

/* 设置观测矩阵H */
static void MPU_EKF_SetH(KalmanFilter_t *kf)
{
    float doubleq0 = 2 * kf->xhatminus_data[0];
    float doubleq1 = 2 * kf->xhatminus_data[1];
    float doubleq2 = 2 * kf->xhatminus_data[2];
    float doubleq3 = 2 * kf->xhatminus_data[3];
    
    // H矩阵 (3x6)
    float H[18] = {
        -doubleq2, doubleq3, -doubleq0, doubleq1, 0, 0,
        doubleq1, doubleq0, doubleq3, doubleq2, 0, 0,
        doubleq0, -doubleq1, -doubleq2, doubleq3, 0, 0
    };
    
    memcpy(kf->H_data, H, sizeof(H));
}

/* 四元数转欧拉角 */
static void Quat_To_Euler(float q[4], float* roll, float* pitch, float* yaw)
{
    // 四元数转欧拉角 (Z-Y-X顺序)
    *roll = atan2f(2*(q[0]*q[1] + q[2]*q[3]), 1 - 2*(q[1]*q[1] + q[2]*q[2]));
    *pitch = asinf(2*(q[0]*q[2] - q[3]*q[1]));
    *yaw = atan2f(2*(q[0]*q[3] + q[1]*q[2]), 1 - 2*(q[2]*q[2] + q[3]*q[3]));
}
