#include "stm32f10x.h"                  // Device header

//	PWMA -> PA11 -> TIM1 CH4

//	PWMB -> PA8  -> TIM1 CH1

void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11 ;	//OC Channel 1 and 4
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOA , &GPIO_InitStruct );
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //Up count
	TIM_TimeBaseInitStruct.TIM_Period = 100-1;          //ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 36-1;        //PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse = 0;                       //CCR
	
	TIM_OC1Init( TIM1 , &TIM_OCInitStruct);
	TIM_OC4Init( TIM1 , &TIM_OCInitStruct);
	
	TIM_Cmd(TIM1, ENABLE );
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWMA_SetCompare(uint16_t Compare)
{
	TIM_SetCompare4(TIM1,Compare);                //change compare of channel 4
}

void PWMB_SetCompare(uint16_t Compare)
{
	TIM_SetCompare1(TIM1,Compare);                //change compare of channel 1
}
