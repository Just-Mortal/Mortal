#include "battery.h"

extern ADC_HandleTypeDef hadc3;

void Battery_Init(void)
{
	// main.c �� main() ������
//	HAL_ADCEx_Calibration_Start(&hadc3, 0); // У׼ADC3[7](@ref)
}

float Read_Battery_Voltage(void)
{
  HAL_ADC_Start(&hadc3);              // ����ת��
  if (HAL_ADC_PollForConversion(&hadc3, 10) == HAL_OK)
	{
    uint16_t adc_val = HAL_ADC_GetValue(&hadc3);
    
    // ����ʵ�ʵ�ѹ�������ѹ����R1=100k, R2=10k��
    float v_adc = adc_val * 3.3f / 4095;      // ADC�����ѹ
    float v_battery = v_adc * (200 + 22) / 22; // ��ѹϵ�� = (R1+R2)/R2
    
    return v_battery;
  }
  return 0;
}

