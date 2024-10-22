/* Includes ------------------------------------------------------------------*/
#include "system_task.h"

// imu update
// heartbeat

/* Private macro -------------------------------------------------------------*/
#define IMU_LOAD_TIME		1000

/* Private variables ---------------------------------------------------------*/
int32_t imu_i = 0;
bool 		IMU_INIT_LOAD_FLAG = false;


float imu_vx, imu_vy, imu_vz;
float imu2_vx, imu2_vy, imu2_vz;
float imu_dx, imu_dy, imu_dz;
float imu_dx2, imu_dy2, imu_dz2;
float imu_ax, imu_ay, imu_az;
float imu_ax_pre, imu_ay_pre, imu_az_pre;
float imu_test;
// 物体坐标系下的加速度 (ax, ay, az)，单位为 m/s^2
float a_b[3] = {0.0f, 0.0f, 0.0f};
// 世界坐标系下的加速度和速度
float a_w[3] = {0.0f, 0.0f, 0.0f};
float v_w[3] = {0.0f, 0.0f, 0.0f};  // 初始速度为 0
// 旋转矩阵
float test_R[3][3];
float ax_w, ay_w, az_w, vx_w, vy_w, vz_w;


/* Private functions ---------------------------------------------------------*/
void StartSystemTask(void const * argument)
{
	IMU_INIT_LOAD_FLAG = false;
	
	for(;;)
	{
	
//		float q0, q1, q2, q3;
		float ax, ay, az;
		float roll, pitch;
	
		if(!IMU_INIT_LOAD_FLAG)
		{
			imu_i ++;
			
			if(imu_i > IMU_LOAD_TIME)
			{
				IMU_INIT_LOAD_FLAG = true;
				imu.algo.KP = 0.125f;
			}
		}
		
		imu.updata(&imu);
//		
//		q0 = imu.data.q.q0;
//		q1 = imu.data.q.q1;
//		q2 = imu.data.q.q2;
//		q3 = imu.data.q.q3;

		ax	=	imu.data.acc_gyr.acc_x;
		ay	=	imu.data.acc_gyr.acc_y;
		az	=	imu.data.acc_gyr.acc_z;
		
		roll	= imu.data.rpy.roll;
		pitch	= imu.data.rpy.pitch;
		pitch *= (double)0.017453;
		roll  *= (double)0.017453;
		
		imu_ax = ax + arm_sin_f32(pitch) * GRAVITY_Z;
		imu_ay = ay - arm_sin_f32(roll) * arm_cos_f32(pitch) * GRAVITY_Z;
		imu_az = az - arm_cos_f32(roll) * arm_cos_f32(pitch) * GRAVITY_Z;
		
		// low pass
		imu_ax = 0.1f*imu_ax_pre + 0.9f*imu_ax;
		imu_ay = 0.1f*imu_ay_pre + 0.9f*imu_ay;
		imu_az = 0.1f*imu_az_pre + 0.9f*imu_az;

		if(abs(imu_ax) > 0.2f || imu_ax * imu2_vx < 0.f)
			imu2_vx += imu_ax * 0.001f;
		if(abs(imu_ay) > 0.2f || imu_ay * imu2_vy < 0.f)
			imu2_vy += imu_ay * 0.001f;
		if(abs(imu_az) > 0.2f  || imu_az * imu2_vz < 0.f)
			imu2_vz += imu_az * 0.001f;
		
		if(abs(imu2_vx) > 0.4f || (imu2_vx * imu_dx < 0.f))
			imu_dx	+=	imu2_vx * 0.001f;
		if(abs(imu2_vy) > 0.4f || (imu2_vy * imu_dy < 0.f))
			imu_dy	+=	imu2_vy * 0.001f;
		if(abs(imu2_vz) > 0.6f || (imu2_vz * imu_dz < 0.f))
			imu_dz	+=	imu2_vz * 0.001f;
		
		imu_dx2 = 1000.f * imu_dx;
		imu_dy2 = 1000.f * imu_dy;
		imu_dz2 = 1000.f * imu_dz;
		
		imu_ax_pre = imu_ax;
		imu_ay_pre = imu_ay;
		imu_az_pre = imu_az;
//		a_b[0] = ax;
//		a_b[1] = ay;
//		a_b[2] = az;
//		quaternion_to_rotation_matrix(q0, q1, q2, q3, test_R);
//		transform_acceleration(a_b, test_R, a_w);
//		integrate_acceleration(a_w, v_w);

//		ax_w = a_w[0];
//		ay_w = a_w[1];
//		az_w = a_w[2];
//		vx_w = v_w[0];
//		vy_w = v_w[1];
//		vz_w = v_w[2];


		osDelay(1);
	}
}
