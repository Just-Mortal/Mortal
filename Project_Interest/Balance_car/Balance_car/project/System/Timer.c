#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	TIM_InternalClockConfig(TIM4);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;		//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 144-1;		//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
/*
//	f = 72MHZ / (PSC+1)(ARR+1)      0<PSC,ARR<65535
//	f = 50HZ   T = 0.02s			PSC = 144	ARR = 10000
//	
//	Ultralsound:
//	V = 343 m/s
//	length = T*V/2 = 343cm
	
	MPU的读取频率为100HZ	10mS
	
	f = 72MHZ / (PSC+1)(ARR+1)      0<PSC,ARR<65535
	f = 5000HZ   T = 0.0002s = 0.2ms = 200uS	PSC = 144	ARR = 100
	
	Ultralsound:
	V = 34300 cm/s
	length = T*V/2 = 34300*0.0002/2cm = 3.4cm
*/
	
	TIM_ClearFlag(TIM4,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;						//开启中断
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;	//设置优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4, ENABLE );
}
