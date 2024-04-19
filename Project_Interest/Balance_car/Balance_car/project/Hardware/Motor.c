#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	//AIN1 -> PA4
	//AIN2 -> PA5
	//控制方向
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initstructure);
	
	//BIN1 -> PB6
	//BIN2 -> PB7
	//控制方向
	
	PWM_Init();
}

void MotorA_SetSpeed(int8_t Speed)
{
	if(Speed >= 0)
	{
		if(Speed > 99 )
			Speed = 100;
		
		GPIO_ResetBits(GPIOA , GPIO_Pin_4);
		GPIO_SetBits(GPIOA , GPIO_Pin_5);
		PWMA_SetCompare(Speed);             //CCR 0-100
	}
	if(Speed < 0)
	{
		if(Speed < -99 )
			Speed = -100;
		
		GPIO_SetBits(GPIOA , GPIO_Pin_4);
		GPIO_ResetBits(GPIOA , GPIO_Pin_5);
		PWMA_SetCompare(-Speed);
	}
}

void MotorB_SetSpeed(int8_t Speed)
{
	if(Speed >= 0)
	{
		if(Speed > 99 )
			Speed = 100;
		
		GPIO_SetBits(GPIOB , GPIO_Pin_6);
		GPIO_ResetBits(GPIOB , GPIO_Pin_7);
		PWMB_SetCompare(Speed);
	}
	if(Speed < 0)
	{
		if(Speed < -99 )
			Speed = -100;
		
		GPIO_ResetBits(GPIOB , GPIO_Pin_6);
		GPIO_SetBits(GPIOB , GPIO_Pin_7);
		PWMB_SetCompare(-Speed);             //CCR 0-100
	}
}
