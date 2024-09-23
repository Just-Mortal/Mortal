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
float imu_test;
// 物体坐标系下的加速度 (ax, ay, az)，单位为 m/s^2
float a_b[3] = {0.0f, 0.0f, 0.0f};
// 世界坐标系下的加速度和速度
float a_w[3] = {0.0f, 0.0f, 0.0f};
float v_w[3] = {0.0f, 0.0f, 0.0f};  // 初始速度为 0
// 旋转矩阵
float test_R[3][3];

/* Private functions ---------------------------------------------------------*/
void StartSystemTask(void const * argument)
{
	IMU_INIT_LOAD_FLAG = false;
	
	float q0, q1, q2, q3;
	
	
	
	for(;;)
	{
	
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
			
			q0 = imu.data.q.q0;
			q1 = imu.data.q.q1;
			q2 = imu.data.q.q2;
			q3 = imu.data.q.q3;
			
//			imu_vx = 2.f*(q1*q3 - q0*q2);
//			imu_vy = 2.f*(q2*q3 + q0*q1);
//			imu_vz = pow(q0,2)+pow(q3,2)-pow(q1,2)-pow(q2,2);
//				
//			imu_test = pow(imu_vx, 2) + pow(imu_vy, 2) + pow(imu_vz, 2);

			a_b[0]	=	imu.data.acc_gyr.acc_x;
			a_b[1]	=	imu.data.acc_gyr.acc_y;
			a_b[2]	=	imu.data.acc_gyr.acc_z;

			quaternion_to_rotation_matrix(q0, q1, q2, q3, test_R);
			transform_acceleration(a_b, test_R, a_w);
			integrate_acceleration(a_w, v_w);
		
		osDelay(1);
	}
}
