/* Includes ------------------------------------------------------------------*/
#include "gpio_drv.h"

/* Exported functions --------------------------------------------------------*/
void Led_Open(uint8_t led_id)
{
	switch(led_id % 3)
	{
		case 1:				//	R
			HAL_GPIO_WritePin(GPIOC, LED_R_Pin, GPIO_PIN_RESET);
			break;
		
		case 2:				//	B
			HAL_GPIO_WritePin(GPIOC, LED_B_Pin, GPIO_PIN_RESET);
			break;
			
		case 0:				//	G
			HAL_GPIO_WritePin(GPIOC, LED_G_Pin, GPIO_PIN_RESET);
			break;
			
		default:
			break;
	}
}

void Led_Close(uint8_t led_id)
{
	switch(led_id % 3)
	{
		case 1:				//	R
			HAL_GPIO_WritePin(GPIOC, LED_R_Pin, GPIO_PIN_SET);
			break;
		
		case 2:				//	B
			HAL_GPIO_WritePin(GPIOC, LED_B_Pin, GPIO_PIN_SET);
			break;
			
		case 0:				//	G
			HAL_GPIO_WritePin(GPIOC, LED_G_Pin, GPIO_PIN_SET);
			break;
			
		default:
			break;
	}
}

void Led_Toggle(uint8_t led_id)
{
	switch(led_id % 3)
	{
		case 1:				//	R
			HAL_GPIO_TogglePin(GPIOC, LED_R_Pin);
			break;
		
		case 2:				//	B
			HAL_GPIO_TogglePin(GPIOC, LED_B_Pin);
			break;
			
		case 0:				//	G
			HAL_GPIO_TogglePin(GPIOC, LED_G_Pin);
			break;
			
		default:
			break;
	}
}

GPIO_PinState Key_GPIO_Update(uint8_t key_num)
{
	GPIO_PinState key_state;
	
	switch(key_num % 4)
	{
		case 1:
			key_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12);
			break;
			
		case 2:
			key_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_10);
			break;
		
		case 3:
			key_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
			break;
			
		case 0:
			key_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_11);
			break;
			
		default:
			break;
	}
	
	return key_state;
}

