/* Includes ------------------------------------------------------------------*/
#include "imu.h"
#include "bmi_EKF.h"

#include "gpio_drv.h"
#include "haltick_drv.h"

#include "math_support.h"


/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void imu_init(imu_t *self);
void imu_heart_beat(work_state_t *heart);
void imu_update(imu_t *self);
//void imu_set_temperature(imu_t *self, float temp);
static void InitQuaternion(float *init_q4);

/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

imu_info_t imu_info = 
{
	.offset_info.gx_offset = 0.f,
	.offset_info.gy_offset = 0.f,
	.offset_info.gz_offset = 0.f,
	.init_flag = 0,
};

/* Exported variables --------------------------------------------------------*/
imu_t imu = {

	.info = &imu_info,
	
	.work_state.dev_state = DEV_OFFLINE,
	
	.work_state.cali_end = 0,	
	.work_state.offline_max_cnt = 50,	
	.work_state.err_cnt = 0,
	
	.init = &imu_init,
	.update = &imu_update,
    .heartbeat = &imu_heart_beat,
//    .set_temperature = &imu_set_temperature,
};

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
float imu_read[3];
uint8_t init_cnt = 200;
/**
 * @brief  imu初始化
 */
void imu_init(struct imu_struct *self)
{
	uint32_t tickstart = HAL_GetTick();

	self->work_state.dev_state = DEV_OFFLINE;
	self->work_state.init_code = BMI088_init();

	while(self->work_state.init_code)
	{
		if (++self->work_state.err_cnt == init_cnt)
		{
			__set_FAULTMASK(1); 
			NVIC_SystemReset();
			break;
		}
        self->work_state.err_code = IMU_INIT_ERR;
        self->work_state.init_code = BMI088_init();
	}	

	
	if (self->work_state.init_code == 0)
	{
		self->work_state.dev_state = DEV_ONLINE;
		
//		self->work_state.err_cnt = 0;
		self->info->init_flag = 1;
		
		/* EKF初始化 */
		transform_init(&EKFgim_trans);
		// float init_quaternion[4] = {0.999019921, -0.0315267481, -0.0310692526};
		float init_quaternion[4] = {0};
		InitQuaternion(init_quaternion);
		IMU_QuaternionEKF_Init(init_quaternion, 10, 0.001, 1000000, 1);

		self->work_state.err_code = IMU_DATA_CALI;//开启陀螺仪校正使用IMU_DATA_CALI，关闭使用IMU_NONE_ERR
		imu.info->offset_info.gx_offset = 0.f;
		imu.info->offset_info.gy_offset = 0.f;
		imu.info->offset_info.gz_offset = 0.f;
	}
	else
	{
		self->work_state.dev_state = DEV_OFFLINE;
		self->work_state.err_code = IMU_INIT_ERR;
		self->work_state.offline_cnt = self->work_state.offline_max_cnt;
		self->info->init_flag = 0;
	}
	
}

/**
 * @brief  imu失联检测
 */
void imu_heart_beat(work_state_t *heart)
{
	heart->offline_cnt++;
	if(heart->offline_cnt > heart->offline_max_cnt) 
	{
		heart->offline_cnt = heart->offline_max_cnt;
		heart->dev_state = DEV_OFFLINE;
	}
	else 
	{
		if(heart->dev_state == DEV_OFFLINE)
        {
            heart->dev_state = DEV_ONLINE;
        }
	}
}

/**
 * @brief  imu设置温度
 */
//void imu_set_temperature(imu_t *self, float temp)
//{
//    self->temp_pid->err = temp - imu_info.base_info.temperature;
//    single_pid_ctrl(self->temp_pid);
//	/* 温度异常值保护 */
//    if(imu_info.base_info.temperature > 50 || imu_info.base_info.temperature < 0
//       || self->temp_pid->out < 0)
//    {
//        self->temp_pid->out = 0;
//    }

//    IMU_Set_PWM(self->temp_pid->out);
//}

ave_filter_t imu_pitch_dif_speed_ave_filter;
ave_filter_t imu_roll_dif_speed_ave_filter;
ave_filter_t imu_yaw_dif_speed_ave_filter;
/* 临时变量 */
static float pitch, roll, yaw;
static float gyrox, gyroy, gyroz;
static float accx, accy, accz;
static float gyrox_, gyroy_, gyroz_;
static float accx_, accy_, accz_;
static float gyro[3], accel[3], temp;
static int16_t imu_cnt = 0;
// 采样时间
static float imu_dt;
static uint32_t imu_tick_now, imu_tick_last;
void imu_update(imu_t *imu_sen)
{

    imu_info_t *imu_info = imu_sen->info;
	
	/* 获取陀螺仪数据 */
	BMI088_read(gyro, accel, &temp);
	
	imu_info->raw_info.acc_x = accel[0];
	imu_info->raw_info.acc_y = accel[1];
	imu_info->raw_info.acc_z = accel[2];
	imu_info->raw_info.gyro_x = gyro[0];
	imu_info->raw_info.gyro_y = gyro[1];
	imu_info->raw_info.gyro_z = gyro[2];
	
	/* 坐标系变换 */
	Vector_Transform(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2],\
	                 &gyrox, &gyroy, &gyroz, &accx, &accy, &accz);
	
	/* 陀螺仪校正 */
	if (imu_sen->work_state.err_code == IMU_DATA_CALI)
	{
		if (imu_cnt < 2000)
		{
			imu_info->offset_info.gx_offset -= gyrox * 0.0005f;
			imu_info->offset_info.gy_offset -= gyroy * 0.0005f;
			imu_info->offset_info.gz_offset -= gyroz * 0.0005f;
			imu_cnt++;
		}
		else
		{
			imu_cnt = 0;
			imu_sen->work_state.err_code = IMU_NONE_ERR;
			
			if (abs(imu_info->offset_info.gx_offset) > 0.005f)
				imu_info->offset_info.gx_offset = 0;
			if (abs(imu_info->offset_info.gy_offset) > 0.005f)
				imu_info->offset_info.gy_offset = 0;
			if (abs(imu_info->offset_info.gz_offset) > 0.005f)
				imu_info->offset_info.gz_offset = 0;
			imu_sen->work_state.cali_end = 1;
		}
	}
	else
	{
		//	2025/06/14 Humble 存疑
		gyrox += imu_info->offset_info.gx_offset;
		gyroy += imu_info->offset_info.gy_offset;//只对gyroz作修正，x，y的角速度已经在解算过程中完成了校正
		gyroz += imu_info->offset_info.gz_offset;//只对gyroz作修正，x，y的角速度已经在解算过程中完成了校正
	}
	
	
	/* 原始数据低通滤波 */
	gyrox_ = Lowpass(gyrox_, gyrox, 1);
	gyroy_ = Lowpass(gyroy_, gyroy, 1);
	gyroz_ = Lowpass(gyroz_, gyroz, 1);
	accx_ = Lowpass(accx_, accx, 0.8);
	accy_ = Lowpass(accy_, accy, 0.2);
	accz_ = Lowpass(accz_, accz, 0.2);
	
	/* 解算陀螺仪数据 */

	// 计算采样时间
		imu_tick_now = micros();
		if(imu_tick_last == 0) // 第一次特殊处理
		{
			imu_tick_last = imu_tick_now - 1000; // 间隔1ms
		}
		imu_dt = (imu_tick_now - imu_tick_last) * 0.000001f; // us to s
		imu_tick_last = imu_tick_now;
		if(imu_dt > 1)//防止系统定时器发癫计算出很大的值
		{
			imu_dt = 0.001f;
		}
    // 核心函数,EKF更新四元数
    IMU_QuaternionEKF_Update(gyrox, gyroy, gyroz, accx, accy, accz, imu_dt);

		imu_info->base_info.yaw = QEKF_INS.Yaw;
    imu_info->base_info.pitch = QEKF_INS.Pitch;
    imu_info->base_info.roll = QEKF_INS.Roll;
    imu_info->base_info.yaw_total_angle = QEKF_INS.YawTotalAngle;

	/* 获取世界坐标系的加速度 */						 
	pitch = imu_info->base_info.pitch, roll = imu_info->base_info.roll, yaw = imu_info->base_info.yaw;
	BMI_Get_Acceleration(pitch, roll, yaw,\
											 accx_, accy_, accz_,\
											 &imu_info->base_info.accx, &imu_info->base_info.accy, &imu_info->base_info.accz);
	
	/* 计算陀螺仪数据 */
	//pitch
	imu_info->base_info.rate_pitch = gyroy_ / (double)0.017453;
	imu_info->base_info.ave_rate_pitch = ave_fil_update(&imu_pitch_dif_speed_ave_filter, imu_info->base_info.rate_pitch, 3);
	
	//roll
	imu_info->base_info.rate_roll = gyrox_ / (double)0.017453;
	imu_info->base_info.ave_rate_roll = ave_fil_update(&imu_roll_dif_speed_ave_filter, imu_info->base_info.rate_roll, 3);
	
	//yaw
	imu_info->base_info.rate_yaw = gyroz_ / (double)0.017453;
	imu_info->base_info.ave_rate_yaw = ave_fil_update(&imu_yaw_dif_speed_ave_filter, imu_info->base_info.rate_yaw, 3);
	
	imu_sen->work_state.offline_cnt = 0;
	
	/* imu读取数据判断  */
	if ((accel[0] == 0) && (accel[1] == 0) && (accel[2] == 0) \
		 && (gyro[0] == 0) && (gyro[1] == 0) && (gyro[2]== 0))
	{
		if(++imu_sen->work_state.err_cnt >= 100)
		{
			imu_sen->work_state.dev_state = DEV_OFFLINE;
			imu_sen->work_state.err_code = IMU_DATA_ERR;
			imu_sen->work_state.offline_cnt = imu_sen->work_state.offline_max_cnt;
			imu_sen->work_state.err_cnt = 100;
		}
	}

    /* imu获取温度 */
    imu_info->base_info.temperature = temp;

}


// 使用加速度计的数据初始化Roll和Pitch,而Yaw置0,这样可以避免在初始时候的姿态估计误差
static void InitQuaternion(float *init_q4)
{
    float acc_sum[3] = {0};
	float gyro_init[3], acc_init[3];

    // 读取100次加速度计数据,取平均值作为初始值
    for (uint8_t i = 0; i < 100; ++i)
    {
				BMI088_read(gyro_init, acc_init, &temp);
        acc_sum[0] += acc_init[0];
        acc_sum[1] += acc_init[1];
        acc_sum[2] += acc_init[2];
        delay_ms(1);
    }
    for (uint8_t i = 0; i < 3; ++i)
        acc_init[i] = acc_sum[i]/100;
		float pitch, roll, temp;
		arm_sqrt_f32(acc_init[1] * acc_init[1] + acc_init[2] * acc_init[2], &temp);
		arm_atan2_f32(-acc_init[0], temp, &pitch);
		arm_atan2_f32(acc_init[1], acc_init[2], &roll);
		
		float half_pitch = pitch / 2.0f;
    float half_roll = roll / 2.0f;

    init_q4[0] = arm_cos_f32(half_pitch) * arm_cos_f32(half_roll);
    init_q4[1] = arm_sin_f32(half_pitch) * arm_cos_f32(half_roll);
    init_q4[2] = arm_cos_f32(half_pitch) * arm_sin_f32(half_roll);
    init_q4[3] = arm_sin_f32(half_pitch) * arm_sin_f32(half_roll);
}

