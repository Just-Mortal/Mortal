#include "imu.h"

#if !defined  (IMU_POTOCAL_TYPE)
  #define IMU_POTOCAL_TYPE    IMU_SPI
#endif 

#if !defined  (IMU_POSE_ANGLE)
  #define IMU_POSE_ANGLE  0.f
#endif 

#if !defined  (IMU_POSE_AX)
  #define IMU_POSE_AX    	1.f
#endif 

#if !defined  (IMU_POSE_AY)
  #define IMU_POSE_AY    	0.f
#endif 

#if !defined  (IMU_POSE_AZ)
  #define IMU_POSE_AZ    	0.f
#endif 
	
#if !defined  (IMU_PID_KP)
  #define IMU_PID_KP    	10.f
#endif 

static void imu_init(struct imu_struct *self);
static void imu_heartbeat(struct imu_struct *imu);
static void imu_updata(struct imu_struct *self);
static void imu_spi_get_data(struct imu_struct *self);

// --- 计算函数 --- begin //
static void  AccGyr_2_EulerAngle(struct imu_struct *self);
static void  RotationTransform_IMU2Pose(struct imu_struct *self);
static void  QuaternionGyr_2_WorldGyro(struct imu_struct *self);
static void  Difference_2_WorldGyro(struct imu_struct *self);
static float IMU_Lpf(float data,float data_pre,float a);
static void  IMU_LpfCenter(struct imu_struct *self);

//static float lsb_to_mps2(int16_t val, float g_range, uint8_t bit_width);
//static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width);
// --- 计算函数 --- end   //

extern SPI_HandleTypeDef hspi2;

imu_t imu = {

	.info.tpye  					= IMU_POTOCAL_TYPE,
	.info.state 					= IMU_OFFLINE,
	.info.init_flag       = 0,
	.info.offline_max_cnt = 50,	
	
	// 解算使能
	.info.flag.rpy 	      = IMU_ENABLE,     //使能rpy解算
	.info.flag.wGryo      = IMU_ENABLE,    	//使能世界角速度解算
	.info.flag.wGryo_type = IMU_DISENABLE,	//世界角速度解算方式：四元数ENABLE  角度差分DISEABLE
	.info.flag.gyrOffset  = IMU_ENABLE,  		//使能校正
	
	// imu装配位置，绕（ax，ay，az）轴旋转angle（角度制）
	.info.pose.angle = IMU_POSE_ANGLE,
	.info.pose.ax    = IMU_POSE_AX,
	.info.pose.ay    = IMU_POSE_AY,
	.info.pose.az    = IMU_POSE_AZ,
	
	// 四元数收敛的pid系数  T：数据更新周期单位s
	.algo.KP = IMU_PID_KP,
	.algo.KI = 0.f,
	.algo.T  = 0.001f,
	
	// 校正最大次数
	.algo.correct_cnt_max = 1000,
	
	// 低通滤波系数，0-1 越大滤波越强
	.algo.ACC_LPF_a  = 0.9,
	.algo.GYR_LPF_a  = 0.f,
	.algo.RPY_LPF_a  = 0.f,
	.algo.wGYR_LPF_a = 0.8f,

	.init       = &imu_init,
	.updata     = &imu_updata,
  .heartbeat	= &imu_heartbeat,

};

#define IMU_GRAVITY_EARTH  (9.80665f)

/*!
 * @brief This function converts lsb to meter per second squared for 16 bit accelerometer at
 * range 2G, 4G, 8G or 16G.
 */
//static float lsb_to_mps2(int16_t val, float g_range, uint8_t bit_width)
//{
//    float half_scale = ((float)(1 << bit_width) / 2.0f);

//    return (IMU_GRAVITY_EARTH * val * g_range) / half_scale;
//}

/*!
 * @brief This function converts lsb to degree per second for 16 bit gyro at
 * range 125, 250, 500, 1000 or 2000dps.
 */
//static float lsb_to_dps(int16_t val, float dps, uint8_t bit_width)
//{
//    float half_scale = ((float)(1 << bit_width) / 2.0f);

//    return (dps / ((half_scale))) * (val);
//}

float inVSqrt(float x)
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

float imu_half_cycle(float angle,float max)
{
	if(i_abs(angle) > (max/2))
	{	
		if(angle >= 0)
			angle += -max;		
		else
			angle +=  max;
	}
	return angle;
}

/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/

// imu 初始化测试		test begin
int8_t rslt_test ;
// imu 初始化测试		test end
void imu_init(struct imu_struct *self)
{
	int8_t rslt;

	float q0_init,q1_init,q2_init,q3_init;
	float d2r,norm;
	
	self->info.state = IMU_OFFLINE;

	rslt = BMI088_init();

	while(rslt)
	{
		self->info.init_cycle++;
		self->info.state = IMU_INIT_ERR;
		rslt = BMI088_init();
		
		// test code begin
		rslt_test = rslt ;
		// test code end
		
		// 初始化失败程序复位
		if(self->info.init_cycle > 100)
		{
				__set_FAULTMASK(1);
				NVIC_SystemReset();

		}
	}
	
	if(!rslt)
	{
//		imu_init_times = 0;
//		
//		Flash_WriteWordData(IMU_DATA_ADDR, &imu_init_times, 1);
	
		self->info.state 			= IMU_ONLINE;
		self->info.init_flag 	= 1;	
		
		if(self->info.flag.gyrOffset == IMU_ENABLE)
		{
			self->info.correct = IMU_NO;
		}
		else
		{
			self->info.correct = IMU_OK;
		}
		
	  self->solve_rpy = AccGyr_2_EulerAngle;
		self->solve_lpf = IMU_LpfCenter;
		self->solve_s2p = RotationTransform_IMU2Pose;
		
		if(self->info.flag.wGryo_type == IMU_ENABLE)
			self->solve_gyr = QuaternionGyr_2_WorldGyro;
		
		if(self->info.flag.wGryo_type == IMU_DISENABLE)
			self->solve_gyr = Difference_2_WorldGyro;				
		
		//获取初始四元数				//th = angle/180*pi/2;
		d2r = (float)self->info.pose.angle/180.f*3.141592654f/2.f;
		
		q0_init =                    arm_cos_f32(d2r);
		q1_init = self->info.pose.ax*arm_sin_f32(d2r);
		q2_init = self->info.pose.ay*arm_sin_f32(d2r);
		q3_init = self->info.pose.az*arm_sin_f32(d2r);
	
		norm = inVSqrt(q0_init*q0_init + q1_init*q1_init + q2_init*q2_init + q3_init*q3_init);
		q0_init = q0_init *norm;
		q1_init = q1_init *norm;
		q2_init = q2_init *norm;
		q3_init = q3_init *norm;		
		
		self->data.qInit.q0 = self->data.q.q0 = q0_init;
		self->data.qInit.q1 = self->data.q.q1 = q1_init;
		self->data.qInit.q2 = self->data.q.q2 = q2_init;
		self->data.qInit.q3 = self->data.q.q3 = q3_init;
	}
	
	self->info.init_time = HAL_GetTick();
}



void imu_updata(struct imu_struct *self)
{
	static uint16_t correct_cnt = 0;	
	
	if(self->info.init_flag == 0)
		return;
	
	/*获取raw data*/
	self->data.acc_gyr_pre = self->data.acc_gyr;
	
	imu_spi_get_data(self);

	/*零飘处理*/
	if(self->info.correct == IMU_OK)
	{
		self->data.acc_gyr.gyr_x -= self->data.gyr_offset.gyr_x;
		self->data.acc_gyr.gyr_y -= self->data.gyr_offset.gyr_y;
		self->data.acc_gyr.gyr_z -= self->data.gyr_offset.gyr_z;
	}
	else if(self->info.correct == IMU_ING)
	{
		if(correct_cnt < self->algo.correct_cnt_max)
		{
			self->data.gyr_offset.gyr_x += self->data.acc_gyr.gyr_x/(float)self->algo.correct_cnt_max;
			self->data.gyr_offset.gyr_y += self->data.acc_gyr.gyr_y/(float)self->algo.correct_cnt_max;
			self->data.gyr_offset.gyr_z += self->data.acc_gyr.gyr_z/(float)self->algo.correct_cnt_max;
		
			correct_cnt++;
		}
		else
		{
			if((i_abs(self->data.gyr_offset.gyr_x) > 0.01f)
			|| (i_abs(self->data.gyr_offset.gyr_y) > 0.01f)
			|| (i_abs(self->data.gyr_offset.gyr_z) > 0.01f))
			{
				self->data.gyr_offset.gyr_x = 0;
			  self->data.gyr_offset.gyr_y = 0;
			  self->data.gyr_offset.gyr_z = 0;
			}
		  self->info.correct = IMU_OK;
		}
	}
	else if(self->info.correct == IMU_NO)
	{
		correct_cnt = 0; 
		
		self->data.gyr_offset.gyr_x = 0;
		self->data.gyr_offset.gyr_y = 0;
		self->data.gyr_offset.gyr_z = 0;

		self->info.correct = IMU_ING;
	}
	/*零飘处理end*/
	
	/*转换装配坐标系*/
	self->solve_s2p(self);
	
	/*获取rpy*/
	if(self->info.flag.rpy   == IMU_ENABLE)
	{
		self->data.rpy_pre = self->data.rpy;	
		self->solve_rpy(self);
	}
	else
	{
		memset(&self->data.rpy, 0, sizeof(self->data.rpy));
	}
	
	/*获取世界角速度*/
	if(self->info.flag.wGryo   == IMU_ENABLE)
	{
		self->data.worldGyr_pre = self->data.worldGyr;
		self->solve_gyr(self);
	}
	else
	{
		memset(&self->data.worldGyr, 0, sizeof(self->data.worldGyr));
	}
	
	/*低通滤波*/
	self->solve_lpf(self);
	
	self->info.offline_cnt = 0;
}

void imu_heartbeat(struct imu_struct *imu)
{
	IMU_Info_t *info = &imu->info;
	
	info->offline_cnt++;
	
	if(info->offline_cnt > info->offline_max_cnt) 
	{
		info->offline_cnt = info->offline_max_cnt;
		info->state = IMU_OFFLINE;
	}
	else 
	{
		if(info->state == IMU_OFFLINE)
			info->state = IMU_ONLINE;
	}
}

float temp;
void imu_spi_get_data(struct imu_struct *self)
{
	float gyro[3], accel[3];
	
	BMI088_read(gyro,accel,&temp);
	
//	self->data.rawData.acc_x	 = (int16_t)data[0] | ( (int16_t)data[1] << 8);
//	self->data.rawData.acc_y	 = (int16_t)data[2] | ( (int16_t)data[3] << 8);
//	self->data.rawData.acc_z	 = (int16_t)data[4] | ( (int16_t)data[5] << 8);
//	
//	self->data.rawData.gyr_x	= (int16_t)data[6]  | ( (int16_t)data[7] << 8);
//	self->data.rawData.gyr_y  = (int16_t)data[8]  | ( (int16_t)data[9] << 8);
//	self->data.rawData.gyr_z  = (int16_t)data[10] | ( (int16_t)data[11]<< 8);	

	self->data.acc_gyr.acc_x = accel[0];   //分钟转秒
	self->data.acc_gyr.acc_y = accel[1];
	self->data.acc_gyr.acc_z = accel[2];	
	
	self->data.acc_gyr.gyr_x = gyro[0];  //转弧度
	self->data.acc_gyr.gyr_y = gyro[1];
	self->data.acc_gyr.gyr_z = gyro[2];
		
	self->info.offline_cnt = 0 ;
}


/*--------------------------------------------算法部分-----------------------------------------------*/
/*

旋转矩阵转换imu坐标 to 装配坐标

*/
void RotationTransform_IMU2Pose(struct imu_struct *self)
{
	
	RP_QuaternionMartix_RightMult(self->data.qInit.q0,
															  self->data.qInit.q1,
															  self->data.qInit.q2,
															  self->data.qInit.q3,
	
	                              self->data.acc_gyr.acc_x,
	                              self->data.acc_gyr.acc_y,
	                              self->data.acc_gyr.acc_z,
	
														 	 &self->data.acc_gyr.acc_x,
														 	 &self->data.acc_gyr.acc_y,
														 	 &self->data.acc_gyr.acc_z);	

	RP_QuaternionMartix_RightMult(self->data.qInit.q0,
															  self->data.qInit.q1,
															  self->data.qInit.q2,
															  self->data.qInit.q3,
	
	                              self->data.acc_gyr.gyr_x,
	                              self->data.acc_gyr.gyr_y,
	                              self->data.acc_gyr.gyr_z,
																
														 	 &self->data.acc_gyr.gyr_x,
														 	 &self->data.acc_gyr.gyr_y,
														 	 &self->data.acc_gyr.gyr_z);
}

/*

	四元数解算世界坐标系的角速度
左乘
*/
void QuaternionGyr_2_WorldGyro(struct imu_struct *self)
{

	RP_QuaternionMartix_LeftMult(self->data.q.q0,
															 self->data.q.q1,
															 self->data.q.q2,
															 self->data.q.q3,
	
	                             self->data.acc_gyr.gyr_x,
	                             self->data.acc_gyr.gyr_y,
	                             self->data.acc_gyr.gyr_z,															 
	                             &self->data.worldGyr.x,
															 &self->data.worldGyr.y,
															 &self->data.worldGyr.z);

}


/*
	四元数+PID 解算RPY
*/
/**
    @param
    @Kp
        越大表示越信任加速度，但快速晃动时，yaw轴角度可能会变化或者快速漂移。Kp越大，初始化的时候数据稳定越快。
    @Ki
        越小积分误差越小
    @halfT
        解算周期的一半，比如1ms解算1次则halfT为0.0005f

*/
float exInt,eyInt,ezInt;

void AccGyr_2_EulerAngle(struct imu_struct *self)
{ 
  float q0, q1, q2, q3;	
	float q0temp,q1temp,q2temp,q3temp; 
	
	float norm,sintemp,costemp;
	
  float vx, vy, vz;
  float gx,gy,gz,ax,ay,az;	
  float ex, ey, ez;	
	
	float Kp = self->algo.KP;
	float Ki = self->algo.KI;
	float halfT = self->algo.T/2.f;

	q0 = self->data.q.q0;
	q1 = self->data.q.q1;
	q2 = self->data.q.q2;
	q3 = self->data.q.q3;
	
	ax = self->data.acc_gyr.acc_x;
	ay = self->data.acc_gyr.acc_y;
	az = self->data.acc_gyr.acc_z;
	
	gx = self->data.acc_gyr.gyr_x;
	gy = self->data.acc_gyr.gyr_y;
	gz = self->data.acc_gyr.gyr_z;

	if(ax * ay *az == 0)
	{
		return;
	}		

	norm = inVSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 *norm;
	q1 = q1 *norm;
	q2 = q2 *norm;
	q3 = q3 *norm;	
	
	norm = inVSqrt(ax*ax + ay*ay + az*az);
	ax = ax *norm;
	ay = ay *norm;
	az = az *norm;
	
	vx = 2.f*(q1*q3 - q0*q2); 
	vy = 2.f*(q0*q1 + q2*q3);
	vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	
	ex = (ay*vz - az*vy) ;   
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;
	
	exInt = exInt + ex * Ki;   
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;
	
	gx = gx + Kp*ex + exInt;
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt; 
	
	q0temp=q0;  
	q1temp=q1;  
	q2temp=q2;  
	q3temp=q3; 
	
	q0 = q0temp + (-q1temp*gx - q2temp*gy -q3temp*gz)*halfT;
	q1 = q1temp + ( q0temp*gx + q2temp*gz -q3temp*gy)*halfT;
	q2 = q2temp + ( q0temp*gy - q1temp*gz +q3temp*gx)*halfT;
	q3 = q3temp + ( q0temp*gz + q1temp*gy -q2temp*gx)*halfT;
	
	norm = inVSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 *norm;
	q1 = q1 *norm;
	q2 = q2 *norm;
	q3 = q3 *norm;
	
	self->data.q.q0 = q0;
	self->data.q.q1 = q1;
	self->data.q.q2 = q2;
	self->data.q.q3 = q3;
	
  /*-合成角-*/
	sintemp = 2.f * q1 * q3 -2.f * q0* q2;
	arm_sqrt_f32(1.f - sintemp * sintemp, &costemp);
	
	arm_atan2_f32(2.f * q2 * q3 + 2.f * q0 * q1, q0*q0 - q1 * q1 - q2 * q2 + q3 * q3, &self->data.rpy.roll);
  arm_atan2_f32(sintemp, costemp, &self->data.rpy.pitch);
	arm_atan2_f32(2.f *(q1 * q2 + q0* q3),     q0*q0 + q1 * q1 - q2 * q2 - q3 * q3, &self->data.rpy.yaw);

	self->data.rpy.roll  *=  57.295773f;
	self->data.rpy.pitch *= -57.295773f;
	self->data.rpy.yaw   *=  57.295773f;
}

/*

	差分角度解算世界坐标系的角速度

*/
void Difference_2_WorldGyro(struct imu_struct *self)
{
	float err[3];
	
	err[0] = self->data.rpy.roll  - self->data.rpy_pre.roll;
	err[1] = self->data.rpy.pitch - self->data.rpy_pre.pitch;
	err[2] = self->data.rpy.yaw   - self->data.rpy_pre.yaw;

	for(char i = 0; i < 3; i++)
	{
		err[i] = imu_half_cycle(err[i],360);
	}

	self->data.worldGyr.x = err[0] * 1000.f;///self->algo.T;
	self->data.worldGyr.y = err[1] * 1000.f;///self->algo.T;
	self->data.worldGyr.z = err[2] * 1000.f;///self->algo.T;
}



/*

低通滤波结算

*/
float IMU_Lpf(float data,float data_pre,float a)
{
	if(i_abs(a) > 1)
		a =i_abs(a)/10;
	return data*(1-a)+data_pre*a;
}

void IMU_LpfCenter(struct imu_struct *self)
{
	if(self->algo.ACC_LPF_a != 0)
	{
		self->data.acc_gyr.acc_x = IMU_Lpf(self->data.acc_gyr.acc_x,self->data.acc_gyr_pre.acc_x,self->algo.ACC_LPF_a);
		self->data.acc_gyr.acc_y = IMU_Lpf(self->data.acc_gyr.acc_y,self->data.acc_gyr_pre.acc_y,self->algo.ACC_LPF_a);
	  self->data.acc_gyr.acc_z = IMU_Lpf(self->data.acc_gyr.acc_z,self->data.acc_gyr_pre.acc_z,self->algo.ACC_LPF_a);
	}
	if(self->algo.GYR_LPF_a != 0)
	{
		self->data.acc_gyr.gyr_x = IMU_Lpf(self->data.acc_gyr.gyr_x,self->data.acc_gyr_pre.gyr_x,self->algo.GYR_LPF_a);
		self->data.acc_gyr.gyr_y = IMU_Lpf(self->data.acc_gyr.gyr_y,self->data.acc_gyr_pre.gyr_y,self->algo.GYR_LPF_a);
	  self->data.acc_gyr.gyr_z = IMU_Lpf(self->data.acc_gyr.gyr_z,self->data.acc_gyr_pre.gyr_z,self->algo.GYR_LPF_a);
	}	
	if(self->algo.RPY_LPF_a != 0)
	{
		self->data.rpy.roll  = IMU_Lpf(self->data.rpy.roll, self->data.rpy_pre.roll, self->algo.RPY_LPF_a);
		self->data.rpy.pitch = IMU_Lpf(self->data.rpy.pitch,self->data.rpy_pre.pitch,self->algo.RPY_LPF_a);
		
		// test code
//		self->data.rpy.yaw_no_lpf = self->data.rpy.yaw;
		
	  self->data.rpy.yaw   = IMU_Lpf(self->data.rpy.yaw,  self->data.rpy_pre.yaw,  self->algo.RPY_LPF_a);
	}
	if(self->algo.wGYR_LPF_a != 0)
	{
		self->data.worldGyr.x = IMU_Lpf(self->data.worldGyr.x,self->data.worldGyr_pre.x,self->algo.wGYR_LPF_a);
		self->data.worldGyr.y = IMU_Lpf(self->data.worldGyr.y,self->data.worldGyr_pre.y,self->algo.wGYR_LPF_a);
	  self->data.worldGyr.z = IMU_Lpf(self->data.worldGyr.z,self->data.worldGyr_pre.z,self->algo.wGYR_LPF_a);
	}	
}

/*-----------------------------------------------------------------------------------*/

/*
旋转 左乘
*/
void RP_QuaternionMartix_LeftMult(float q0,float q1,float q2,float q3,float x,float y,float z,float *ax,float *ay,float *az)
{
	
	float a,b,c,d;
	
	a = q0;
	b = q1;
	c = q2;
	d = q3;
	
	*ax = z*(2.f*a*c + 2.f*b*d) - y*(2.f*a*d - 2.f*b*c) + x*(a*a + b*b - c*c - d*d);
	*ay = x*(2.f*a*d + 2.f*b*c) - z*(2.f*a*b - 2.f*c*d) + y*(a*a - b*b + c*c - d*d);
	*az = y*(2.f*a*b + 2.f*c*d) - x*(2.f*a*c - 2.f*b*d) + z*(a*a - b*b - c*c + d*d);

}

/*
映射 右乘
*/
void RP_QuaternionMartix_RightMult(float q0,float q1,float q2,float q3,\
																	float x,float y,float z,\
																	float *ax,float *ay,float *az)
{
	float a,b,c,d;
	
	a = q0;
	b = q1;
	c = q2;
	d = q3;
	
	*ax = y*(2.f*a*d + 2.f*b*c) - z*(2.f*a*c - 2.f*b*d) + x*(a*a + b*b - c*c - d*d);
	*ay = z*(2.f*a*b + 2.f*c*d) - x*(2.f*a*d - 2.f*b*c) + y*(a*a - b*b + c*c - d*d);
	*az = x*(2.f*a*c + 2.f*b*d) - y*(2.f*a*b - 2.f*c*d) + z*(a*a - b*b - c*c + d*d);
}

void RP_Quaternion_2_EulerAngle(float q0,float q1,float q2,float q3,float *r,float *p,float *y)
{

	float norm,sintemp,costemp;

	norm = inVSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 *norm;
	q1 = q1 *norm;
	q2 = q2 *norm;
	q3 = q3 *norm;
	
	sintemp = 2.f * q1 * q3 -2.f * q0* q2;
	arm_sqrt_f32(1.f - sintemp * sintemp, &costemp);
	
	arm_atan2_f32(2.f * q2 * q3 + 2.f * q0 * q1, q0*q0 - q1 * q1 - q2 * q2 + q3 * q3, r);
  arm_atan2_f32(sintemp, costemp, p);
	arm_atan2_f32(2.f *(q1 * q2 + q0* q3),     q0*q0 + q1 * q1 - q2 * q2 - q3 * q3, y);
	
	*p = -*p; 

}

void RP_EulerAngle_2_Quaternion(float *q0,float *q1,float *q2,float *q3,double r,double p,double y)
{
    double cy = arm_cos_f32(y * 0.5);
    double sy = arm_sin_f32(y * 0.5);
    double cp = arm_cos_f32(p * 0.5);
    double sp = arm_sin_f32(p * 0.5);
    double cr = arm_cos_f32(r * 0.5);
    double sr = arm_sin_f32(r * 0.5);
 
    *q0 = cy * cp * cr + sy * sp * sr;
    *q1 = cy * cp * sr - sy * sp * cr;
    *q2 = sy * cp * sr + cy * sp * cr;
    *q3 = sy * cp * cr - cy * sp * sr;
}
