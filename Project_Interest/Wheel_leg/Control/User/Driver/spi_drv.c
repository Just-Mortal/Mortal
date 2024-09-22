/* Includes ------------------------------------------------------------------*/
#include "spi_drv.h"
#include "main.h"

/* Exported functions --------------------------------------------------------*/
void Imu_SPI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
  HAL_GPIO_WritePin(BMI_CS_GPIO_Port, BMI_CS_Pin, GPIO_PIN_SET);	
	
  /*Configure GPIO pin : BMI_CS_Pin */
  GPIO_InitStruct.Pin = BMI_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BMI_CS_GPIO_Port, &GPIO_InitStruct);	
	
	HAL_Delay(10);
	HAL_GPIO_WritePin(BMI_CS_GPIO_Port, BMI_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}
