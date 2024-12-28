#include "oled.h"


bit B_1ms;

#define QUEUE_LENGTH 10

uint8 temp[4];
uint16 msecond;
uint8 voltage_queue[QUEUE_LENGTH] = {0};
uint8 voltage_filtered; 
uint8 measure_time = 0;
uint8 freq_ctrl = 0; 

void AddToQueue(uint8 value);
uint8 GetFilteredVoltage();
void OLED_DrawLine(uint8 x1, uint8 y1, uint8 x2, uint8 y2);

void main(void)
{
    uint8 i, voltage;
    uint8 prev_x = 0, prev_y = 0;

    EAXSFR();  
    GPIO_config();
    Timer_config();
    I2C_config();
    UART_config();
    ADC_config();
    EA = 1;
    OLED_Init();

    while (1)
    {
        if (B_1ms)
        {
            B_1ms = 0;
            freq_ctrl++;

            // 读取电压值，范围是 0-1023，映射到 0-63
            voltage = Get_ADCResult(i) / 16;
            voltage = (voltage > 63) ? 63 : voltage;

            AddToQueue(voltage);
            voltage_filtered = GetFilteredVoltage();

            measure_time = (measure_time >= 127) ? 0 : measure_time + 1;

            if (measure_time > 0)
							OLED_DrawLine(prev_x, prev_y, measure_time, voltage_filtered);

            prev_x = measure_time;
            prev_y = voltage_filtered;
        }
    }
}

void AddToQueue(uint8 value)
{
	uint8 i;

	for (i = 0; i < QUEUE_LENGTH - 1; i++)
			voltage_queue[i] = voltage_queue[i + 1];  // 队列左移
	voltage_queue[QUEUE_LENGTH - 1] = value;  		// 添加新值到队列末尾
}

uint8 GetFilteredVoltage()
{
	uint8 i;
	uint16 sum = 0;
	for (i = 0; i < QUEUE_LENGTH; i++)
			sum += voltage_queue[i];
	return (uint8)(sum / QUEUE_LENGTH);						// 返回滤波后的电压值
}
