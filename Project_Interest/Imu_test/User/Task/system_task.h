#ifndef __SYSTEM_TASK_H
#define __SYSTEM_TASK_H

/* Includes ------------------------------------------------------------------*/
#include "imu.h"

#include "stdbool.h"
#include "cmsis_os.h"
#include "haltick_drv.h"


#define GRAVITY_Z -9.80665f
#define DT 				0.001f


// 计算旋转矩阵
void quaternion_to_rotation_matrix(float q0, float q1, float q2, float q3, float R[3][3])
{
    R[0][0] = 1.0f - 2.0f * (q2 * q2 + q3 * q3);
    R[0][1] = 2.0f * (q1 * q2 - q3 * q0);
    R[0][2] = 2.0f * (q1 * q3 + q2 * q0);
    
    R[1][0] = 2.0f * (q1 * q2 + q3 * q0);
    R[1][1] = 1.0f - 2.0f * (q1 * q1 + q3 * q3);
    R[1][2] = 2.0f * (q2 * q3 - q1 * q0);
    
    R[2][0] = 2.0f * (q1 * q3 - q2 * q0);
    R[2][1] = 2.0f * (q2 * q3 + q1 * q0);
    R[2][2] = 1.0f - 2.0f * (q1 * q1 + q2 * q2);
}

// 将加速度从物体坐标系转换到世界坐标系
void transform_acceleration(float a_b[3], float R[3][3], float a_w[3])
{
    for (int i = 0; i < 3; ++i)
		{
			a_w[i] = 0.0f;
      for (int j = 0; j < 3; ++j)
				a_w[i] += R[i][j] * a_b[j];
    }

    a_w[2] += GRAVITY_Z;
}

// 对加速度积分得到速度
void integrate_acceleration(float a_w[3], float v_w[3])
{
    for (int i = 0; i < 3; ++i)
       v_w[i] += a_w[i] * DT;
}

#endif
