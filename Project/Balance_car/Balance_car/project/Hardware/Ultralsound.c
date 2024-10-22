#include "stm32f10x.h"                  // Device header
#include "Timer.h"
#include "delay.h"

unsigned char toofar;
int ECOH_Num = 0;

void Ultralsound_Init(void)
{
	Timer_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	 /*TRIG 信号 */
 	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
 	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 /*ECOH 信号 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

double get_length(void)
{
	uint16_t t ;
	double time ,length ;
	
	toofar = 0;
	
 	GPIO_SetBits(GPIOB,GPIO_Pin_0);
 	delay_us(11);
  	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
 	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) == RESET)
	{
	}
   	TIM4->CNT = 0;				//清零计时器
  	TIM_Cmd(TIM4,ENABLE);		//回响信号到来，开启定时器计数
  	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==SET)
	{
	}
  	TIM_Cmd(TIM4,DISABLE);		//关闭定时器
  
/*
	修改：能不能把get_lenth的持续时间降到uS级？
//	ARR+1 = 10000  ->  0.02s
//	time = t*0.02/10000
	
	toofar	340cm
	length_one = T*V/2 = 34300*0.0002/2cm = 3.4cm
	ECOH_Num > 100	toofar
*/
	if(ECOH_Num > 100)
		toofar = 1;
	
 	if(!toofar)									//没有完成一次计数周期,即距离不是很远 
  	{
   		t = TIM_GetCounter(TIM4)+100*ECOH_Num;	//获取 TIM4 寄存器中的计数值
   		time = t*0.02/100.0; 					//往返时间,单位ms
   		length = 34.3*time/2; 					//单程长度,单位cm
		ECOH_Num = 0;
   		TIM4->CNT=0; 							//将 TIM4计数寄存器的计数值清零
  	}
  	else
 	{
//   	toofar = 0; 
   		length = -1; 						//足够远,不用考虑 
   		TIM4 -> CNT=0; 						//将TIM4计数寄存器的计数值清零
  	}
 	return length;
}

void TIM4_IRQHandler(void)
{
 	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
   	{
		ECOH_Num++;
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	// 清除中断标志
   	}
}
