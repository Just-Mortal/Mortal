/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M_LB1_Pin GPIO_PIN_5
#define M_LB1_GPIO_Port GPIOE
#define M_LB2_Pin GPIO_PIN_6
#define M_LB2_GPIO_Port GPIOE
#define ENCODER_LB1_Pin GPIO_PIN_0
#define ENCODER_LB1_GPIO_Port GPIOA
#define ENCODER_LB2_Pin GPIO_PIN_1
#define ENCODER_LB2_GPIO_Port GPIOA
#define PS2_CLK_Pin GPIO_PIN_7
#define PS2_CLK_GPIO_Port GPIOE
#define PS2_CS_Pin GPIO_PIN_8
#define PS2_CS_GPIO_Port GPIOE
#define M_LF1_Pin GPIO_PIN_9
#define M_LF1_GPIO_Port GPIOE
#define PS2_CMD_Pin GPIO_PIN_10
#define PS2_CMD_GPIO_Port GPIOE
#define M_LF2_Pin GPIO_PIN_11
#define M_LF2_GPIO_Port GPIOE
#define PS2_DAT_Pin GPIO_PIN_12
#define PS2_DAT_GPIO_Port GPIOE
#define M_RF1_Pin GPIO_PIN_13
#define M_RF1_GPIO_Port GPIOE
#define M_RF2_Pin GPIO_PIN_14
#define M_RF2_GPIO_Port GPIOE
#define M_RB1_Pin GPIO_PIN_14
#define M_RB1_GPIO_Port GPIOB
#define M_RB2_Pin GPIO_PIN_15
#define M_RB2_GPIO_Port GPIOB
#define ENCODER_RB1_Pin GPIO_PIN_12
#define ENCODER_RB1_GPIO_Port GPIOD
#define ENCODER_RB2_Pin GPIO_PIN_13
#define ENCODER_RB2_GPIO_Port GPIOD
#define ENCODER_LF1_Pin GPIO_PIN_15
#define ENCODER_LF1_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_2
#define LED_GPIO_Port GPIOD
#define ENCODER_LF2_Pin GPIO_PIN_3
#define ENCODER_LF2_GPIO_Port GPIOB
#define ENCODER_RF1_Pin GPIO_PIN_4
#define ENCODER_RF1_GPIO_Port GPIOB
#define ENCODER_RF2_Pin GPIO_PIN_5
#define ENCODER_RF2_GPIO_Port GPIOB
#define Buzzer_Pin GPIO_PIN_1
#define Buzzer_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
