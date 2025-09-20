#ifndef __IMU_H
#define __IMU_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "BMI088reg.h"
#include "BMI088driver.h"
#include "BMI088Middleware.h"

#include "ave_filter.h"
#include "math_support.h"

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
typedef enum{
  IMU_NONE_ERR,
  IMU_TYPE_ERR,
  IMU_ID_ERR,
  IMU_INIT_ERR,
  IMU_DATA_ERR,
  IMU_DATA_CALI,
} imu_err_e;

typedef struct drive_str{
	
	int8_t (*send)(struct drive_str *self, uint8_t *Txbuff, uint16_t len);
	int8_t (*read)(struct drive_str *self, uint8_t *Rxbuff, uint16_t len);
	int8_t (*sendread)(struct drive_str *self, uint8_t *Txbuff, uint8_t *Rxbuff, uint16_t len);
}driver_t;

typedef struct work_state_struct {
	dev_work_state_t dev_state;
	imu_err_e	       err_code;
	
	uint8_t		err_cnt;
	int8_t		init_code;
	
	uint8_t		cali_end;
	
	uint8_t   offline_cnt;
	uint8_t   offline_max_cnt;
	
} work_state_t;



typedef struct{
	float acc_x;
	float acc_y;
	float acc_z;
	
	float gyro_x;
	float gyro_y;
	float gyro_z;	

}	raw_info_t;

typedef struct{
	float accx;
	float accy;
	float accz;
	
	float yaw;
	float pitch;
	float roll;
	float yaw_total_angle;
	
	float rate_yaw;
	float rate_pitch;
	float rate_roll;	
	
	float ave_rate_yaw;
	float ave_rate_pitch;	
	float ave_rate_roll;

    float temperature;

}	base_info_t;

typedef struct{
	float gx_offset;
	float gy_offset;
	float gz_offset;	
}	offset_info_t;

typedef struct imu_info_struct {

	raw_info_t  	raw_info;
	base_info_t 	base_info;
	offset_info_t	offset_info;
	
	uint8_t		    init_flag;

} imu_info_t;

typedef struct imu_struct {
	
	imu_info_t		*info;
	driver_t	  	driver;

	void			(*init)(struct imu_struct *self);
	void			(*update)(struct imu_struct *self);
	void			(*heartbeat)(struct work_state_struct *self);

	work_state_t  	work_state;
} imu_t;

extern imu_t imu;

/* Exported functions --------------------------------------------------------*/

#endif
