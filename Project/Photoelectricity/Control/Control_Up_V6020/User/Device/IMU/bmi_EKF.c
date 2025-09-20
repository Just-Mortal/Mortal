/**
 ******************************************************************************
 * @file    QuaternionEKF.c
 * @author  Wang Hongxi
 * @version V1.2.0
 * @date    2022/3/8
 * @brief   attitude update with gyro bias estimate and chi-square test
 ******************************************************************************
 * @attention
 * 1st order LPF transfer function:
 *     1
 *  ———————
 *  as + 1
 *
 ******************************************************************************
 */
#include "bmi_EKF.h"
#include "rp_config.h"

/* 重力加速度 */
#define GRAVITY_EARTH  (9.80665f)
/* 矩阵实例定义 */
arm_matrix_instance_f32 EKFTrans;
arm_matrix_instance_f32 EKFSrc;
arm_matrix_instance_f32 EKFDst;

/**
 * @brief   坐标变换采用Z-Y-X欧拉角描述，即从陀螺仪坐标系向云台坐标系变换中，
 *          坐标系按照绕陀螺仪Z轴、Y轴、X轴的顺序旋转
 *          每一次旋转的参考坐标系为当前陀螺仪坐标系  
 *  @param
 *  @arz
 *      陀螺仪x轴与roll轴之间的夹角，单位为度
 *  @ary
 *      陀螺仪x轴与yaw轴之间的夹角，单位为度
 *  @arx
 *      陀螺仪y轴与yaw轴之间的夹角，单位为度
 */
gimbal_transform_t EKFgim_trans = {
    .arz = 90.0f,
    .ary = 0.0f,
    .arx = 0.0f,
    .trans = {0.0f},
};

QEKF_INS_t QEKF_INS;

const float IMU_QuaternionEKF_F[36] = {1, 0, 0, 0, 0, 0,
                                       0, 1, 0, 0, 0, 0,
                                       0, 0, 1, 0, 0, 0,
                                       0, 0, 0, 1, 0, 0,
                                       0, 0, 0, 0, 1, 0,
                                       0, 0, 0, 0, 0, 1};
float IMU_QuaternionEKF_P[36] = {100000, 0.1, 0.1, 0.1, 0.1, 0.1,
                                 0.1, 100000, 0.1, 0.1, 0.1, 0.1,
                                 0.1, 0.1, 100000, 0.1, 0.1, 0.1,
                                 0.1, 0.1, 0.1, 100000, 0.1, 0.1,
                                 0.1, 0.1, 0.1, 0.1, 100, 0.1,
                                 0.1, 0.1, 0.1, 0.1, 0.1, 100};
float IMU_QuaternionEKF_K[18];
float IMU_QuaternionEKF_H[18];

static void IMU_QuaternionEKF_Observe(KalmanFilter_t *kf);
static void IMU_QuaternionEKF_F_Linearization_P_Fading(KalmanFilter_t *kf);
static void IMU_QuaternionEKF_SetH(KalmanFilter_t *kf);
static void IMU_QuaternionEKF_xhatUpdate(KalmanFilter_t *kf);

/**
 * @brief 基于扩展卡尔曼滤波的姿态解算初始化
 * @param[in] process_noise1 设置四元数的过程噪声协方差矩阵，越小解算数据越平滑，越大系统对快速变化的反应越快   10
 * @param[in] process_noise2 设置陀螺仪零偏估计过程噪声协方差矩阵     0.001
 * @param[in] measure_noise  设置加速度计测量噪声协方差矩阵，越小对加速度越信任，系统对快速变化的反应越快       1000000
 * @param[in] lambda         设置渐消因子防止陀螺仪零偏估计协方差过度收敛         0.9996
 */
void IMU_QuaternionEKF_Init(float* init_quaternion,float process_noise1, float process_noise2, float measure_noise, float lambda)
{
	  
    QEKF_INS.Initialized = 1;
    QEKF_INS.Q1 = process_noise1;
    QEKF_INS.Q2 = process_noise2;
    QEKF_INS.R = measure_noise;
    QEKF_INS.ChiSquareTestThreshold = 1e-8;
    QEKF_INS.ConvergeFlag = 0;
    QEKF_INS.ErrorCount = 0;
    QEKF_INS.UpdateCount = 0;
    if (lambda > 1)
    {
        lambda = 1;
    }
    QEKF_INS.lambda = lambda;

    // 初始化矩阵维度信息
    Kalman_Filter_Init(&QEKF_INS.IMU_QuaternionEKF, 6, 0, 3);
    Matrix_Init(&QEKF_INS.ChiSquare, 1, 1, (float *)QEKF_INS.ChiSquare_Data);

    // 姿态初始化
    for(int i = 0; i < 4; i++)
    {
        QEKF_INS.IMU_QuaternionEKF.xhat_data[i] = init_quaternion[i];
    }

    // 自定义函数初始化,用于扩展或增加kf的基础功能
    QEKF_INS.IMU_QuaternionEKF.User_Func0_f = IMU_QuaternionEKF_Observe;
    QEKF_INS.IMU_QuaternionEKF.User_Func1_f = IMU_QuaternionEKF_F_Linearization_P_Fading;
    QEKF_INS.IMU_QuaternionEKF.User_Func2_f = IMU_QuaternionEKF_SetH;
    QEKF_INS.IMU_QuaternionEKF.User_Func3_f = IMU_QuaternionEKF_xhatUpdate;

    // 设定标志位,用自定函数替换kf标准步骤中的SetK(计算增益)以及xhatupdate(后验估计/融合)
    QEKF_INS.IMU_QuaternionEKF.SkipEq3 = TRUE;
    QEKF_INS.IMU_QuaternionEKF.SkipEq4 = TRUE;

    memcpy(QEKF_INS.IMU_QuaternionEKF.F_data, IMU_QuaternionEKF_F, sizeof(IMU_QuaternionEKF_F));
    memcpy(QEKF_INS.IMU_QuaternionEKF.P_data, IMU_QuaternionEKF_P, sizeof(IMU_QuaternionEKF_P));
}

/**
 * @brief 基于扩展卡尔曼滤波对四元数进行更新
 * @param[in]       陀螺仪数据 gx gy gz in rad/s
 * @param[in]       加速度计数据 ax ay az in m/s
 * @param[in]       数据更新周期 in s
 */
void IMU_QuaternionEKF_Update(float gx, float gy, float gz, float ax, float ay, float az, float dt)
{
    // 0.5(Ohm-Ohm^bias)*deltaT,用于更新工作点处的状态转移F矩阵
    static float halfgxdt, halfgydt, halfgzdt;
    static float accelInvNorm;

    /*   F, number with * represent vals to be set
     0      1*     2*     3*     4     5
     6*     7      8*     9*    10    11
    12*    13*    14     15*    16    17
    18*    19*    20*    21     22    23
    24     25     26     27     28    29
    30     31     32     33     34    35
    */
    QEKF_INS.dt = dt;

    QEKF_INS.Gyro[0] = gx - QEKF_INS.GyroBias[0];
    QEKF_INS.Gyro[1] = gy - QEKF_INS.GyroBias[1];
    QEKF_INS.Gyro[2] = gz - QEKF_INS.GyroBias[2];

    // set F
    halfgxdt = 0.5f * QEKF_INS.Gyro[0] * dt;
    halfgydt = 0.5f * QEKF_INS.Gyro[1] * dt;
    halfgzdt = 0.5f * QEKF_INS.Gyro[2] * dt;

    // 此部分设定状态转移矩阵F的左上角部分 4x4子矩阵,即0.5(Ohm-Ohm^bias)*deltaT,右下角有一个2x2单位阵已经初始化好了
    // 注意在predict步F的右上角是4x2的零矩阵,因此每次predict的时候都会调用memcpy用单位阵覆盖前一轮线性化后的矩阵
    memcpy(QEKF_INS.IMU_QuaternionEKF.F_data, IMU_QuaternionEKF_F, sizeof(IMU_QuaternionEKF_F));

    QEKF_INS.IMU_QuaternionEKF.F_data[1] = -halfgxdt;
    QEKF_INS.IMU_QuaternionEKF.F_data[2] = -halfgydt;
    QEKF_INS.IMU_QuaternionEKF.F_data[3] = -halfgzdt;

    QEKF_INS.IMU_QuaternionEKF.F_data[6] = halfgxdt;
    QEKF_INS.IMU_QuaternionEKF.F_data[8] = halfgzdt;
    QEKF_INS.IMU_QuaternionEKF.F_data[9] = -halfgydt;

    QEKF_INS.IMU_QuaternionEKF.F_data[12] = halfgydt;
    QEKF_INS.IMU_QuaternionEKF.F_data[13] = -halfgzdt;
    QEKF_INS.IMU_QuaternionEKF.F_data[15] = halfgxdt;

    QEKF_INS.IMU_QuaternionEKF.F_data[18] = halfgzdt;
    QEKF_INS.IMU_QuaternionEKF.F_data[19] = halfgydt;
    QEKF_INS.IMU_QuaternionEKF.F_data[20] = -halfgxdt;

		QEKF_INS.Accel[0] = ax;
		QEKF_INS.Accel[1] = ay;
		QEKF_INS.Accel[2] = az;
    // set z,单位化重力加速度向量
    arm_sqrt_f32(QEKF_INS.Accel[0] * QEKF_INS.Accel[0] + QEKF_INS.Accel[1] * QEKF_INS.Accel[1] + QEKF_INS.Accel[2] * QEKF_INS.Accel[2], &QEKF_INS.accl_norm);
		accelInvNorm = 1.0f / QEKF_INS.accl_norm;
    for (uint8_t i = 0; i < 3; ++i)
    {
        QEKF_INS.IMU_QuaternionEKF.MeasuredVector[i] = QEKF_INS.Accel[i] * accelInvNorm; // 用加速度向量更新量测值
    }

    // 计算陀螺仪数据和加速度数据的归一化值，用于判断当前陀螺仪的运动状态
    arm_sqrt_f32(QEKF_INS.Gyro[0] * QEKF_INS.Gyro[0] + QEKF_INS.Gyro[1] * QEKF_INS.Gyro[1] + QEKF_INS.Gyro[2] * QEKF_INS.Gyro[2], &QEKF_INS.gyro_norm);


    // 如果角速度小于阈值且加速度处于设定范围内,认为运动稳定,加速度可以用于修正角速度
    // 稍后在最后的姿态更新部分会利用StableFlag来确定
    if (QEKF_INS.gyro_norm < 0.5f && QEKF_INS.accl_norm > 9.8f - 0.5f && QEKF_INS.accl_norm < 9.8f + 0.5f)
    {
        QEKF_INS.StableFlag = 1;
    }
    else
    {
        QEKF_INS.StableFlag = 0;
    }

    // set Q R,过程噪声和观测噪声矩阵
    QEKF_INS.IMU_QuaternionEKF.Q_data[0] = QEKF_INS.Q1 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.Q_data[7] = QEKF_INS.Q1 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.Q_data[14] = QEKF_INS.Q1 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.Q_data[21] = QEKF_INS.Q1 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.Q_data[28] = QEKF_INS.Q2 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.Q_data[35] = QEKF_INS.Q2 * QEKF_INS.dt;
    QEKF_INS.IMU_QuaternionEKF.R_data[0] = QEKF_INS.R;
    QEKF_INS.IMU_QuaternionEKF.R_data[4] = QEKF_INS.R;
    QEKF_INS.IMU_QuaternionEKF.R_data[8] = QEKF_INS.R;

    // 调用kalman_filter.c封装好的函数,注意几个User_Funcx_f的调用
    Kalman_Filter_Update(&QEKF_INS.IMU_QuaternionEKF);

    // 获取融合后的数据,包括四元数和xy零飘值
    QEKF_INS.q[0] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[0];
    QEKF_INS.q[1] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[1];
    QEKF_INS.q[2] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[2];
    QEKF_INS.q[3] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[3];
    QEKF_INS.GyroBias[0] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[4];
    QEKF_INS.GyroBias[1] = QEKF_INS.IMU_QuaternionEKF.FilteredValue[5];
    QEKF_INS.GyroBias[2] = 0; // 大部分时候z轴通天,无法观测yaw的漂移

    // 利用四元数反解欧拉角
    arm_atan2_f32(2.0f * (QEKF_INS.q[0] * QEKF_INS.q[3] + QEKF_INS.q[1] * QEKF_INS.q[2]), 2.0f * (QEKF_INS.q[0] * QEKF_INS.q[0] + QEKF_INS.q[1] * QEKF_INS.q[1]) - 1.0f, &QEKF_INS.Yaw);
    arm_atan2_f32(2.0f * (QEKF_INS.q[0] * QEKF_INS.q[1] + QEKF_INS.q[2] * QEKF_INS.q[3]), 2.0f * (QEKF_INS.q[0] * QEKF_INS.q[0] + QEKF_INS.q[3] * QEKF_INS.q[3]) - 1.0f, &QEKF_INS.Roll);
		float sintemp, costemp;
		sintemp	= -2.0f * (QEKF_INS.q[1] * QEKF_INS.q[3] - QEKF_INS.q[0] * QEKF_INS.q[2]);
    arm_sqrt_f32(1 - sintemp*sintemp, &costemp);
    arm_atan2_f32(sintemp, costemp, &QEKF_INS.Pitch);
		
		QEKF_INS.Yaw *= 57.295779513f;
		QEKF_INS.Roll *= 57.295779513f;
		QEKF_INS.Pitch *= 57.295779513f;
    // get Yaw total, yaw数据可能会超过360,处理一下方便其他功能使用(如小陀螺)
    if (QEKF_INS.Yaw - QEKF_INS.YawAngleLast > 180.0f)
    {
        QEKF_INS.YawRoundCount--;
    }
    else if (QEKF_INS.Yaw - QEKF_INS.YawAngleLast < -180.0f)
    {
        QEKF_INS.YawRoundCount++;
    }
		
		
    QEKF_INS.YawTotalAngle = 360.0f * QEKF_INS.YawRoundCount + QEKF_INS.Yaw;
    QEKF_INS.YawAngleLast = QEKF_INS.Yaw;
    QEKF_INS.UpdateCount++; // 初始化低通滤波用,计数测试用
}

/**
 * @brief 用于更新线性化后的状态转移矩阵F右上角的一个4x2分块矩阵,稍后用于协方差矩阵P的更新;
 *        并对零漂的方差进行限制,防止过度收敛并限幅防止发散
 *
 * @param kf
 */
static void IMU_QuaternionEKF_F_Linearization_P_Fading(KalmanFilter_t *kf)
{
    static float q0, q1, q2, q3;
    // quaternion normalize将四元数规范化为单位四元数
		q0 = kf->xhatminus_data[0];
		arm_quaternion_normalize_f32(kf->xhatminus_data, kf->xhatminus_data, 1);
		
    q0 = kf->xhatminus_data[0];
    q1 = kf->xhatminus_data[1];
    q2 = kf->xhatminus_data[2];
    q3 = kf->xhatminus_data[3];
    /*  F, number with * represent vals to be set
     0     1     2     3     4*     5*
     6     7     8     9    10*    11*
    12    13    14    15    16*    17*
    18    19    20    21    22*    23*
    24    25    26    27    28     29
    30    31    32    33    34     35
    */
    // set F
    kf->F_data[4] = q1 * QEKF_INS.dt / 2;
    kf->F_data[5] = q2 * QEKF_INS.dt / 2;

    kf->F_data[10] = -q0 * QEKF_INS.dt / 2;
    kf->F_data[11] = q3 * QEKF_INS.dt / 2;

    kf->F_data[16] = -q3 * QEKF_INS.dt / 2;
    kf->F_data[17] = -q0 * QEKF_INS.dt / 2;

    kf->F_data[22] = q2 * QEKF_INS.dt / 2;
    kf->F_data[23] = -q1 * QEKF_INS.dt / 2;

    // fading filter,防止零飘参数过度收敛
    kf->P_data[28] /= QEKF_INS.lambda;
    kf->P_data[35] /= QEKF_INS.lambda;

    // 限幅,防止发散
    if (kf->P_data[28] > 10000)
    {
        kf->P_data[28] = 10000;
    }
    if (kf->P_data[35] > 10000)
    {
        kf->P_data[35] = 10000;
    }
}

/**
 * @brief 在工作点处计算观测函数h(x)的Jacobi矩阵H
 *
 * @param kf
 */
static void IMU_QuaternionEKF_SetH(KalmanFilter_t *kf)
{
    static float doubleq0, doubleq1, doubleq2, doubleq3;
    /* H
     0     1     2     3     4     5
     6     7     8     9    10    11
    12    13    14    15    16    17
    last two cols are zero
    */
    // set H
    doubleq0 = 2 * kf->xhatminus_data[0];
    doubleq1 = 2 * kf->xhatminus_data[1];
    doubleq2 = 2 * kf->xhatminus_data[2];
    doubleq3 = 2 * kf->xhatminus_data[3];

    memset(kf->H_data, 0, sizeof_float * kf->zSize * kf->xhatSize);

    kf->H_data[0] = -doubleq2;
    kf->H_data[1] = doubleq3;
    kf->H_data[2] = -doubleq0;
    kf->H_data[3] = doubleq1;

    kf->H_data[6] = doubleq1;
    kf->H_data[7] = doubleq0;
    kf->H_data[8] = doubleq3;
    kf->H_data[9] = doubleq2;

    kf->H_data[12] = doubleq0;
    kf->H_data[13] = -doubleq1;
    kf->H_data[14] = -doubleq2;
    kf->H_data[15] = doubleq3;
}

/**
 * @brief 利用观测值和先验估计得到最优的后验估计
 *        加入了卡方检验以判断融合加速度的条件是否满足
 *        同时引入发散保护保证恶劣工况下的必要量测更新
 *
 * @param kf
 */
static void IMU_QuaternionEKF_xhatUpdate(KalmanFilter_t *kf)
{
    static float q0, q1, q2, q3;

    kf->MatStatus = Matrix_Transpose(&kf->H, &kf->HT); // z|x => x|z
    kf->temp_matrix.numRows = kf->H.numRows;
    kf->temp_matrix.numCols = kf->Pminus.numCols;
    kf->MatStatus = Matrix_Multiply(&kf->H, &kf->Pminus, &kf->temp_matrix); // temp_matrix = H·P'(k)
    kf->temp_matrix1.numRows = kf->temp_matrix.numRows;
    kf->temp_matrix1.numCols = kf->HT.numCols;
    kf->MatStatus = Matrix_Multiply(&kf->temp_matrix, &kf->HT, &kf->temp_matrix1); // temp_matrix1 = H·P'(k)·HT
    kf->S.numRows = kf->R.numRows;
    kf->S.numCols = kf->R.numCols;
    kf->MatStatus = Matrix_Add(&kf->temp_matrix1, &kf->R, &kf->S); // S = H P'(k) HT + R
    kf->MatStatus = Matrix_Inverse(&kf->S, &kf->temp_matrix1);     // temp_matrix1 = inv(H·P'(k)·HT + R)

    q0 = kf->xhatminus_data[0];
    q1 = kf->xhatminus_data[1];
    q2 = kf->xhatminus_data[2];
    q3 = kf->xhatminus_data[3];

    kf->temp_vector.numRows = kf->H.numRows;
    kf->temp_vector.numCols = 1;
    // 计算预测得到的重力加速度方向(通过姿态获取的)
    kf->temp_vector_data[0] = 2 * (q1 * q3 - q0 * q2);
    kf->temp_vector_data[1] = 2 * (q0 * q1 + q2 * q3);
    kf->temp_vector_data[2] = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3; // temp_vector = h(xhat'(k))

    // 计算预测值和各个轴的方向余弦
    for (uint8_t i = 0; i < 3; ++i)
    {
        QEKF_INS.OrientationCosine[i] = acosf(fabsf(kf->temp_vector_data[i]));
    }

    // 利用加速度计数据修正
    kf->temp_vector1.numRows = kf->z.numRows;
    kf->temp_vector1.numCols = 1;
    kf->MatStatus = Matrix_Subtract(&kf->z, &kf->temp_vector, &kf->temp_vector1); // temp_vector1 = z(k) - h(xhat'(k))

    // chi-square test,卡方检验
    kf->temp_matrix.numRows = kf->temp_vector1.numRows;
    kf->temp_matrix.numCols = 1;
    kf->MatStatus = Matrix_Multiply(&kf->temp_matrix1, &kf->temp_vector1, &kf->temp_matrix); // temp_matrix = inv(H·P'(k)·HT + R)·(z(k) - h(xhat'(k)))
    kf->temp_vector.numRows = 1;
    kf->temp_vector.numCols = kf->temp_vector1.numRows;
    kf->MatStatus = Matrix_Transpose(&kf->temp_vector1, &kf->temp_vector); // temp_vector = z(k) - h(xhat'(k))'
    kf->MatStatus = Matrix_Multiply(&kf->temp_vector, &kf->temp_matrix, &QEKF_INS.ChiSquare);
    // rk is small,filter converged/converging,rk很小，说明滤波器收敛
    if (QEKF_INS.ChiSquare_Data[0] < 0.5f * QEKF_INS.ChiSquareTestThreshold)
    {
        QEKF_INS.ConvergeFlag = 1;
    }
    // rk is bigger than thre but once converged,当前rk大于阈值，且之前滤波器处于收敛
    if (QEKF_INS.ChiSquare_Data[0] > QEKF_INS.ChiSquareTestThreshold && QEKF_INS.ConvergeFlag)
    {
        if (QEKF_INS.StableFlag)
        {
            QEKF_INS.ErrorCount++; // 载体静止时仍无法通过卡方检验
        }
        else
        {
            QEKF_INS.ErrorCount = 0;
        }

        if (QEKF_INS.ErrorCount > 50)
        {
            // 滤波器发散
            QEKF_INS.ConvergeFlag = 0;
            kf->SkipEq5 = FALSE; // step-5 is cov mat P updating,持续更新P矩阵使滤波器收敛
        }
        else
        {
            //  残差未通过卡方检验 载体存在运动加速度，测量值不可信，仅预测
            //  xhat(k) = xhat'(k)
            //  P(k) = P'(k)
            memcpy(kf->xhat_data, kf->xhatminus_data, sizeof_float * kf->xhatSize);
            memcpy(kf->P_data, kf->Pminus_data, sizeof_float * kf->xhatSize * kf->xhatSize);
            kf->SkipEq5 = TRUE; // part5 is P updating,跳过P矩阵的更新
            return;
        }
    }
    else // if divergent or rk is not that big/acceptable,use adaptive gain,滤波器处于发散或者rk值不大于阈值
    {
        // scale adaptive,rk越小则增益越大,否则更相信预测值
        if (QEKF_INS.ChiSquare_Data[0] > 0.1f * QEKF_INS.ChiSquareTestThreshold && QEKF_INS.ConvergeFlag)
        {
            QEKF_INS.AdaptiveGainScale = (QEKF_INS.ChiSquareTestThreshold - QEKF_INS.ChiSquare_Data[0]) / (0.9f * QEKF_INS.ChiSquareTestThreshold);
        }
        else
        {
            QEKF_INS.AdaptiveGainScale = 1;
        }
        QEKF_INS.ErrorCount = 0;
        kf->SkipEq5 = FALSE;
    }

    // cal kf-gain K,计算卡尔曼增益
    kf->temp_matrix.numRows = kf->Pminus.numRows;
    kf->temp_matrix.numCols = kf->HT.numCols;
    kf->MatStatus = Matrix_Multiply(&kf->Pminus, &kf->HT, &kf->temp_matrix); // temp_matrix = P'(k)·HT
    kf->MatStatus = Matrix_Multiply(&kf->temp_matrix, &kf->temp_matrix1, &kf->K);

    // implement adaptive,通过卡方检验，动态调整卡尔曼增益权重
    for (uint8_t i = 0; i < kf->K.numRows * kf->K.numCols; ++i)
    {
        kf->K_data[i] *= QEKF_INS.AdaptiveGainScale;
    }
    for (uint8_t i = 4; i < 6; ++i)
    {
        for (uint8_t j = 0; j < 3; ++j)
        {
            kf->K_data[i * 3 + j] *= QEKF_INS.OrientationCosine[i - 4] / 1.5707963f; // 1 rad
        }
    }

    kf->temp_vector.numRows = kf->K.numRows;
    kf->temp_vector.numCols = 1;
    kf->MatStatus = Matrix_Multiply(&kf->K, &kf->temp_vector1, &kf->temp_vector); // temp_vector = K(k)·(z(k) - H·xhat'(k))

    // 零漂修正限幅,一般不会有过大的漂移
    if (QEKF_INS.ConvergeFlag)
    {
        for (uint8_t i = 4; i < 6; ++i)
        {
            if (kf->temp_vector.pData[i] > 1e-2f * QEKF_INS.dt)
            {
                kf->temp_vector.pData[i] = 1e-2f * QEKF_INS.dt;
            }
            if (kf->temp_vector.pData[i] < -1e-2f * QEKF_INS.dt)
            {
                kf->temp_vector.pData[i] = -1e-2f * QEKF_INS.dt;
            }
        }
    }

    // 不修正yaw轴数据
//    kf->temp_vector.pData[3] = 0;
    kf->MatStatus = Matrix_Add(&kf->xhatminus, &kf->temp_vector, &kf->xhat);
}

/**
 * @brief EKF观测环节,其实就是把数据复制一下
 *
 * @param kf kf类型定义
 */
static void IMU_QuaternionEKF_Observe(KalmanFilter_t *kf)
{
    memcpy(IMU_QuaternionEKF_P, kf->P_data, sizeof(IMU_QuaternionEKF_P));
    memcpy(IMU_QuaternionEKF_K, kf->K_data, sizeof(IMU_QuaternionEKF_K));
    memcpy(IMU_QuaternionEKF_H, kf->H_data, sizeof(IMU_QuaternionEKF_H));
}

/**
  * @brief  陀螺仪坐标变换初始化，若不需要变换可在imu.c中imu_init将其注释
  * @param  
  * @retval 
  */
void transform_init(gimbal_transform_t *gim_trans)
{
    float arz, ary, arx;

	/* 角度单位转换（to弧度） */
	arz = gim_trans->arz * (double)0.017453;
	ary = gim_trans->ary * (double)0.017453;
	arx = gim_trans->arx * (double)0.017453;

	/* 旋转矩阵赋值（三个旋转矩阵叠加） */
	gim_trans->trans[0] = arm_cos_f32(arz)*arm_cos_f32(ary);
	gim_trans->trans[1] = arm_cos_f32(arz)*arm_sin_f32(ary)*arm_sin_f32(arx) - arm_sin_f32(arz)*arm_cos_f32(arx);
	gim_trans->trans[2] = arm_cos_f32(arz)*arm_sin_f32(ary)*arm_cos_f32(arx) + arm_sin_f32(arz)*arm_sin_f32(arx);
	gim_trans->trans[3] = arm_sin_f32(arz)*arm_cos_f32(ary);
	gim_trans->trans[4] = arm_sin_f32(arz)*arm_sin_f32(ary)*arm_sin_f32(arx) + arm_cos_f32(arz)*arm_cos_f32(arx);
	gim_trans->trans[5] = arm_sin_f32(arz)*arm_sin_f32(ary)*arm_cos_f32(arx) - arm_cos_f32(arz)*arm_sin_f32(arx);
	gim_trans->trans[6] = -arm_sin_f32(ary);
	gim_trans->trans[7] = arm_cos_f32(ary)*arm_sin_f32(arx);
	gim_trans->trans[8] = arm_cos_f32(ary)*arm_cos_f32(arx);
	
    /* 3x3变换矩阵初始化 */
	arm_mat_init_f32(&EKFTrans, 3, 3, (float *)gim_trans->trans); 
}

/**
  * @brief  将陀螺仪坐标变换为云台坐标，若不需要变换可在imu_protocol.c中imu_update将其注释
  * @brief  坐标变换采用Z-Y-X欧拉角描述，即从陀螺仪坐标系向云台坐标系变换中，坐标系按照绕陀螺仪Z轴、Y轴、X轴的顺序旋转
  *					每一次旋转的参考坐标系为当前陀螺仪坐标系
  * @param[in]  (int16_t) gx,  gy,  gz,  ax,  ay,  az
  * @param[out] (float *) gx, gy, gz, aax, ay, az
  */
void Vector_Transform(float gx, float gy, float gz,\
	                  float ax, float ay, float az,\
	                  float *ggx, float *ggy, float *ggz,\
					  float *aax, float *aay, float *aaz)
{
    /* 陀螺仪输入输出数组定义 */
    float gyro_in[3], gyro_out[3];
    /* 加速度输入输出数组定义 */
    float acc_in[3], acc_out[3];

	/* 陀螺仪赋值 */
	gyro_in[0] = (float)gx, gyro_in[1] = (float)gy, gyro_in[2] = (float)gz;
	/* 加速度计赋值 */
	acc_in[0] = (float)ax, acc_in[1] = (float)ay, acc_in[2] = (float)az;
	
	/* 陀螺仪坐标变换 */
	arm_mat_init_f32(&EKFSrc, 1, 3, gyro_in);
	arm_mat_init_f32(&EKFDst, 1, 3, gyro_out);
	arm_mat_mult_f32(&EKFSrc, &EKFTrans, &EKFDst);
	*ggx = gyro_out[0], *ggy = gyro_out[1], *ggz = gyro_out[2];
	
	/* 加速度计坐标变换 */
	arm_mat_init_f32(&EKFSrc, 1, 3, acc_in);
	arm_mat_init_f32(&EKFDst, 1, 3, acc_out);
	arm_mat_mult_f32(&EKFSrc, &EKFTrans, &EKFDst);
	*aax = acc_out[0], *aay = acc_out[1], *aaz = acc_out[2];
}

/**
 * @brief  获取世界坐标系的加速度
 */
void BMI_Get_Acceleration(float pitch, float roll, float yaw,\
						  float ax, float ay, float az,\
						  float *accx, float *accy, float *accz)
{
	float imu_accx, imu_accy, imu_accz;
	
    /* 角度制to弧度制 */
	pitch *= (double)0.017453;
	yaw   *= (double)0.017453;
	roll  *= (double)0.017453;

	imu_accx = ax + arm_sin_f32(pitch) * GRAVITY_EARTH;
	imu_accy = ay - arm_sin_f32(roll) * arm_cos_f32(pitch) * GRAVITY_EARTH;
	imu_accz = az - arm_cos_f32(roll) * arm_cos_f32(pitch) * GRAVITY_EARTH;
	
	*accx = imu_accx * arm_cos_f32(pitch) + imu_accz * arm_sin_f32(pitch);
	*accy = imu_accy * arm_cos_f32(roll) - imu_accz * arm_sin_f32(roll);
	*accz = imu_accz * arm_cos_f32(pitch) * arm_cos_f32(roll) - imu_accx * arm_sin_f32(pitch) * arm_cos_f32(roll) \
			+ imu_accy * arm_sin_f32(roll) * arm_cos_f32(pitch);
	
}
