#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

// 比赛记得更改		！！！
#define WATCH_DOG_SWITCH						1			// 比赛看门狗开关	0	关			1	开
#define MOTOR_HIGH_TEMP_PROTECT			1			// 电机高温保护		0	关			1	开
// 比赛记得更改		！！！

#define	GIMBAL_SWITCH		1
#define LAUNCH_SWITCH		1
#define CHASSIS_SWITCH	0

// 比赛相关
#define HEAT_LIMIT_SWITCH						1			// 热量限制				0	关			1	开
#define ADAPT_SHOOT_SPEED_SWITCH		1			// 弹速自适应			0	关			1	开
#define CHASSIS_POWER_LIMIT_MODE		2			// 底盘功率限制		0	无限制		1	传统限制	2	建模限制
#define IMU_SOLVE_METHOD						0			// 陀螺仪结算方式	0	Mahony	1	EKF
#define CHASSIS_CTRL_MODE						0			// 底盘控制方式		0	位控		1	力控

// 测试用
#define RC_CAP_SWITCH								0			// 遥控超电开关		0	关			1	开
#define DIAL_RESET_SWITCH						0			// 拨盘复位				0	关			1	开
#define LAUNCH_CONTINUOUS_MODE			0			// 连发模式				0 速度环	1 角度环
#define PCH_FEEDFORWARD_SWITCH			0			// pitch 前馈输出	0	关			1	开
#define MEASURE_LAUNCH_DELAY_SWITCH 0			// 测量发弹延时		0	关			1	开


// 文件夹首字母大写，文件全小写
// 变量小写					——> 全局变量
// 首字母大写				——> 局部变量
// 结构体类型定义，首字母大写！！！
// 函数定义与声明，下划线之后首字母大写
// bool 标志位，cmd 与 define 宏定义——> 全大写
// 外部引用的函数，小写。函数定义与声明，首字母大写
 
/* Includes ------------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
  * @brief
  * @param  
  * @retval 
  */
	
/**
 * @file
 * @author
 * @Version
 * @brief
 * @update
 *
 *
 *
 */



#endif
