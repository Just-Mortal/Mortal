#ifndef __IMU_H
#define __IMU_H

#include "BMI088driver.h"
#include "BMI088reg.h"
#include "BMI088Middleware.h"

#include "string.h"
#include "rp_math.h"
#include "arm_math.h"
#include "ave_filter.h"


#include "rp_config.h"

#define i_abs(x) 					((x)>0? (x):(-(x)))
#define i_constrain(x, min, max)	((x>max)?max:(x<min?min:x))
#define i_anti_constrain(x, min, max)	(((x<max) && (x>min))?(0):x)

typedef enum {
	
	IMU_ONLINE,
	IMU_OFFLINE,

	IMU_INIT_ERR,	
	IMU_DATA_ERR,
	
	IMU_OK,
	IMU_ING,
	IMU_NO,
	
} IMU_State_e;

typedef enum{

	IMU_SPI,
	IMU_IIC,
	
}IMU_DriveType_e;

typedef enum {
	
	IMU_ENABLE,
	IMU_DISENABLE,
	
} IMU_EnableFlag_e;

typedef struct{
	
	IMU_EnableFlag_e  rpy;
	IMU_EnableFlag_e  wGryo;
	IMU_EnableFlag_e  wGryo_type;
	IMU_EnableFlag_e  gyrOffset;
	
}IMU_SolveEnable_t;


typedef struct{

	short acc_x;
	short acc_y;
	short acc_z;

	short gyr_x;
	short gyr_y;
	short gyr_z;
	
}imu_raw_t;

typedef struct{

	float acc_x;
	float acc_y;
	float acc_z;

	float gyr_x;
	float gyr_y;
	float gyr_z;
	
}acc_gyr_t;



typedef struct{

	float x;
	float y;
	float z;
	
}world_gyr_t;

typedef struct{

	float yaw;
	float pitch;
	float roll;
	
//	float yaw_no_lpf; 
	
}rpy_t;

typedef struct{

	float q0;
	float q1;
	float q2;
	float q3;
	
}quaternion_t;

typedef struct{

	float x;
	float y;
	float z;
	
}xyz_t;

typedef struct{

	float angle;
	float ax;
	float ay;
	float az;
	
}IMU_Pose_t;


typedef struct imu_data_struct {

	imu_raw_t     rawData;

	acc_gyr_t     acc_gyr;
	acc_gyr_t     acc_gyr_pre;	
	
	acc_gyr_t     gyr_offset;
	
	rpy_t         rpy;
	rpy_t         rpy_pre;
	
	world_gyr_t   worldGyr;
	world_gyr_t   worldGyr_pre;
	
	quaternion_t  q;
	quaternion_t  qInit;
	
	float transf[9];
	
} IMU_Data_t;

typedef struct imu_algo_struct {

	float KP;
	float KI;
	float T;
	
	float ACC_LPF_a;
	float GYR_LPF_a;
	float RPY_LPF_a;
	float wGYR_LPF_a;
	
	uint16_t correct_cnt_max;
	
} IMU_ALGO_t;


typedef struct imu_info_struct {
	
	uint8_t		   init_flag;
	
	uint32_t     init_time;
	uint16_t     init_cycle;
	
	IMU_Pose_t   pose;
	
	IMU_State_e	 state;

	IMU_State_e  correct;
	
	IMU_DriveType_e     tpye;
	
	IMU_SolveEnable_t   flag;
	
	uint8_t   offline_cnt;
	uint8_t   offline_max_cnt;

} IMU_Info_t;


typedef struct imu_struct {
	
	IMU_Data_t	data;
	IMU_ALGO_t  algo;
	IMU_Info_t  info;
	
	void				(*init)(struct imu_struct *self);
	void				(*updata)(struct imu_struct *self);
	void				(*heartbeat)(struct imu_struct *self);	
	
	void        (*solve_rpy)(struct imu_struct *self);
	void        (*solve_gyr)(struct imu_struct *self);
	void				(*solve_lpf)(struct imu_struct *self);
	void				(*solve_s2p)(struct imu_struct *self);

} imu_t;

extern imu_t imu;

void RP_Quaternion_2_EulerAngle(float q0,float q1,float q2,float q3,float *r,float *p,float *y);
void RP_EulerAngle_2_Quaternion(float *q0,float *q1,float *q2,float *q3,double r,double p,double y);
void RP_QuaternionMartix_LeftMult(float q0,float q1,float q2,float q3,float x,float y,float z,float *ax,float *ay,float *az);
void RP_QuaternionMartix_RightMult(float q0,float q1,float q2,float q3,float x,float y,float z,float *ax,float *ay,float *az);

#endif
