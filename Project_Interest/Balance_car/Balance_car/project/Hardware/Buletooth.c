#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint16_t Serial_RXDate;
uint8_t Serial_RXFlag;

/*

PA2 -> USART2_TX
PA3 -> USART2_RX

*/

void Serial_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_2 ;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_3 ;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1 ;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStruct);
	
	USART_ITConfig(USART2 ,USART_IT_RXNE ,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init( &NVIC_InitStruct);
	
	USART_Cmd(USART2,ENABLE);
}

void Serial_SendByte(uint16_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
}

void Serial_SendArray(uint8_t *Array ,uint16_t Length)
{
	uint16_t i;
	for(i=0;i<Length;i++)
		Serial_SendByte(Array[i]);
}

void Serial_SendString(char *String)
{
	uint16_t i;
	for(i=0;String[i] != '\0';i++)
		Serial_SendByte(String[i]);
}

uint32_t Serial_Pow(uint32_t x,uint32_t y)
{
	uint32_t i,result = 1;
	for(i=0;i<y;i++)
		result *= x;
	return result;
}

void Serial_SendNumber(uint32_t Number ,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
		Serial_SendByte((Number/Serial_Pow(10,Length-i-1))%10+'0');
}

//int fputc(int ch, FILE *f)
//{
//	Serial_SendByte(ch);
//	return ch;
//}

void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

uint8_t Serial_GetRXFlag(void)
{
	if(Serial_RXFlag == 1)
	{
		Serial_RXFlag = 0;    //读取中断标志位时清零
		return 1;
	}
	else
		return 0;
}

uint16_t Serial_GetRXDate(void)
{
	return Serial_RXDate;
}	

void USART2_IRQHandler(void)
{
	if(USART_GetITStatus(USART2 ,USART_IT_RXNE) == SET  )
	{
		Serial_RXDate = USART_ReceiveData(USART2);  //中断中DR值给Serial_RXDate，可通过GET函数调用
		Serial_RXFlag = 1;                          //标志已经进入中断，和USART——Get作用一样
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
