/* Includes ------------------------------------------------------------------*/
#include "haltick_drv.h"

/* Exported variables --------------------------------------------------------*/
volatile uint32_t HalTick = 0;		// Halʱ���׼(us)

/* Exported functions --------------------------------------------------------*/
/**
 *  @brief 
 *      ��дstm32f4xx_hal.c�е�HAL_IncTick����
 *  @note  
 *      ʹ��cubemx����FREERTOS��Ὠ�齫SYS��ʱ���л��ɳ�SysTick֮��Ķ�ʱ��
 *	    �Ӷ�ϵͳ���������ʱ����������RTOS��SysTick ������HAL��HalTick
 *	    SysTick ʹ��cortex-m4�ں˵�SysTick (SysTick->VAL�����������������֮��Ÿ���)
 *	    HalTick �ڱ���������ʹ��TIM2 (TIM2->CNT���ṩ΢���ʱ)
 *	    # delay_us �� delay_ms ���������������(������)
 */
void HAL_IncTick(void)
{
    // ����Hal��ȫ��ʱ�����(ms)
    uwTick += uwTickFreq;
    // ���µ�ǰ��Halʱ���׼(us)
    HalTick = uwTick*1000 + TIM2->CNT;
}

uint32_t micros(void)
{
    // ���µ�ǰ��Halʱ���׼(us)
    HalTick = uwTick*1000 + TIM2->CNT;
		return HalTick;
}

void delay_us(uint32_t us)
{
	uint32_t now = micros();
	
	while((micros() - now) < us);
}

void delay_ms(uint32_t ms)
{
	while(ms--)
		delay_us(1000);
}
