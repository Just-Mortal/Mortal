#include "common.h"

extern SPI_HandleTypeDef hspi2;

#define SPI_CS_HIG()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET))
#define SPI_CS_LOW()			(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET))

/******************************************************************************/
/*!                 Macro definitions                                         */
#define BMI2XY_SHUTTLE_ID  UINT16_C(0x1B8)

/*! Macro that defines read write length */
#define READ_WRITE_LEN     UINT8_C(46)

/******************************************************************************/
/*!                Static variable definition                                 */

/*! Variable that holds the I2C device address or SPI chip selection */
static uint8_t dev_addr = 0x69;

/******************************************************************************/
/*!                User interface functions                                   */

/*!
 * Delay function map to RP
 */
uint32_t btick = 0;

uint32_t bmi2_micros(void)
{
	register uint32_t bms, bus;

	bms = HAL_GetTick();

	bus = TIM2->CNT;
	
  btick = bms*1000 + bus;
    
	return btick;
} 


void bmi2_delay_us(uint32_t period, void *intf_ptr)
{
	uint32_t now = bmi2_micros();
	
	while((bmi2_micros() - now) < period);
}

void bmi2_delay(uint32_t ms)
{

	while(ms--)
		bmi2_delay_us(1000,NULL);
	
}

/*!
 * SPI read function map to COINES platform
 */
BMI2_INTF_RETURN_TYPE bmi2_spi_read(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	uint8_t res;
	
	SPI_CS_LOW();
	
	bmi2_delay_us(10, NULL);
	
	HAL_SPI_Transmit(&hspi2, &reg_addr, 1, 1000);
	res = HAL_SPI_Receive(&hspi2, reg_data, len, 1000);
	
	bmi2_delay_us(10, NULL);
	
	SPI_CS_HIG();
	return res;
}

/*!
 * SPI write function map to COINES platform
 */
BMI2_INTF_RETURN_TYPE bmi2_spi_write(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
	uint8_t res;
	
	SPI_CS_LOW();
		
	bmi2_delay_us(10, NULL);
	
	HAL_SPI_Transmit(&hspi2, &reg_addr, 1, 1000);
	res = HAL_SPI_Transmit(&hspi2, (uint8_t *)reg_data, len, 1000);
		
	bmi2_delay_us(10, NULL);
	
	SPI_CS_HIG();
	return res;
}

/*!
 *  @brief Function to select the interface between SPI and I2C.
 *  Also to initialize coines platform
 */
int8_t bmi2_interface_init(struct bmi2_dev *bmi, uint8_t intf)
{
    int8_t rslt = BMI2_OK;

		if(bmi != NULL)
		{
			/* To initialize the user SPI function */
			bmi->intf = BMI2_SPI_INTF;
			bmi->read = bmi2_spi_read;
			bmi->write = bmi2_spi_write;

			/* Assign device address to interface pointer */
			bmi->intf_ptr = &dev_addr;

			/* Configure delay in microseconds */
			bmi->delay_us = bmi2_delay_us;

			/* Configure max read/write length (in bytes) ( Supported length depends on target machine) */
			bmi->read_write_len = READ_WRITE_LEN;

			/* Assign to NULL to load the default config file. */
			bmi->config_file_ptr = NULL;

			bmi2_delay(100);
    }
    else
    {
			rslt = BMI2_E_NULL_PTR;
    }

    return rslt;
}

/*!
 *  @brief Prints the execution status of the APIs.
 */
void bmi2_error_codes_print_result(int8_t rslt)
{
}

/*!
 *  @brief Deinitializes coines platform
 *
 *  @return void.
 */
void bmi2_coines_deinit(void)
{
}
