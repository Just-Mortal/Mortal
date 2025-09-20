#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdbool.h"

// �����ǵø���		������
#define WATCH_DOG_SWITCH						1			// �������Ź�����	0	��			1	��
#define MOTOR_HIGH_TEMP_PROTECT			1			// ������±���		0	��			1	��
// �����ǵø���		������

#define	GIMBAL_SWITCH		1
#define LAUNCH_SWITCH		1
#define CHASSIS_SWITCH	0

// �������
#define HEAT_LIMIT_SWITCH						1			// ��������				0	��			1	��
#define ADAPT_SHOOT_SPEED_SWITCH		1			// ��������Ӧ			0	��			1	��
#define CHASSIS_POWER_LIMIT_MODE		2			// ���̹�������		0	������		1	��ͳ����	2	��ģ����
#define IMU_SOLVE_METHOD						0			// �����ǽ��㷽ʽ	0	Mahony	1	EKF
#define CHASSIS_CTRL_MODE						0			// ���̿��Ʒ�ʽ		0	λ��		1	����

// ������
#define RC_CAP_SWITCH								0			// ң�س��翪��		0	��			1	��
#define DIAL_RESET_SWITCH						0			// ���̸�λ				0	��			1	��
#define LAUNCH_CONTINUOUS_MODE			0			// ����ģʽ				0 �ٶȻ�	1 �ǶȻ�
#define PCH_FEEDFORWARD_SWITCH			0			// pitch ǰ�����	0	��			1	��
#define MEASURE_LAUNCH_DELAY_SWITCH 0			// ����������ʱ		0	��			1	��


// �ļ�������ĸ��д���ļ�ȫСд
// ����Сд					����> ȫ�ֱ���
// ����ĸ��д				����> �ֲ�����
// �ṹ�����Ͷ��壬����ĸ��д������
// �����������������»���֮������ĸ��д
// bool ��־λ��cmd �� define �궨�塪��> ȫ��д
// �ⲿ���õĺ�����Сд����������������������ĸ��д
 
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
