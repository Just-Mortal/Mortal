#include "driver.h"

void	GPIO_config(void)
{
	P0_MODE_IO_PU(GPIO_Pin_0 | GPIO_Pin_1);		//P0.0,P0.1 设置为准双向口
	
	P1_MODE_IN_HIZ(GPIO_Pin_All);							//P1.0~P1.7 设置为高阻输入
	
	P3_MODE_IO_PU(GPIO_Pin_6 | GPIO_Pin_7);		//P3.6,P3.7 设置为准双向口
}

void	Timer_config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;						//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;  //指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;         //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;              //是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536UL - (MAIN_Fosc / 1000UL);   //初值,
	TIM_InitStructure.TIM_Run       = ENABLE;               //是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);               //初始化Timer0	  Timer0,Timer1,Timer2,Timer3,Timer4
	NVIC_Timer0_Init(ENABLE,Priority_0);    //中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}

void	I2C_config(void)
{
	I2C_InitTypeDef		I2C_InitStructure;

	I2C_InitStructure.I2C_Mode      = I2C_Mode_Slave;		//主从选择   I2C_Mode_Master, I2C_Mode_Slave
	I2C_InitStructure.I2C_Enable    = ENABLE;						//I2C功能使能,   ENABLE, DISABLE
	I2C_InitStructure.I2C_SL_MA     = ENABLE;						//使能从机地址比较功能,   ENABLE, DISABLE
	I2C_InitStructure.I2C_SL_ADR    = 0x2d;							//从机设备地址,  0~127  (0x2d<<1 = 0x5a)
	I2C_Init(&I2C_InitStructure);
	NVIC_I2C_Init(I2C_Mode_Slave,I2C_ESTAI|I2C_ERXI|I2C_ETXI|I2C_ESTOI,Priority_0);	//主从模式, I2C_Mode_Master, I2C_Mode_Slave; 中断使能, I2C_ESTAI/I2C_ERXI/I2C_ETXI/I2C_ESTOI/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	I2C_SW(I2C_P24_P25);					//I2C_P14_P15,I2C_P24_P25,I2C_P33_P32
}

void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义

	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;	//模式, UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer1;			//选择波特率发生器, BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 115200ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	UART_Configuration(UART1, &COMx_InitStructure);		//初始化串口1 UART1,UART2,UART3,UART4
	NVIC_UART1_Init(ENABLE,Priority_1);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3

	UART1_SW(UART1_SW_P36_P37);		//UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17,UART1_SW_P43_P44
}

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;		//结构定义

	ADC_InitStructure.ADC_SMPduty   = 31;		//ADC 模拟信号采样时间控制, 0~31（注意： SMPDUTY 一定不能设置小于 10）
	ADC_InitStructure.ADC_CsSetup   = 0;		//ADC 通道选择时间控制 0(默认),1
	ADC_InitStructure.ADC_CsHold    = 1;		//ADC 通道选择保持时间控制 0,1(默认),2,3
	ADC_InitStructure.ADC_Speed     = ADC_SPEED_2X16T;		//设置 ADC 工作时钟频率	ADC_SPEED_2X1T~ADC_SPEED_2X16T
	ADC_InitStructure.ADC_AdjResult = ADC_RIGHT_JUSTIFIED;	//ADC结果调整,	ADC_LEFT_JUSTIFIED,ADC_RIGHT_JUSTIFIED
	ADC_Inilize(&ADC_InitStructure);		//初始化
	ADC_PowerControl(ENABLE);				//ADC电源开关, ENABLE或DISABLE
	NVIC_ADC_Init(DISABLE,Priority_0);		//中断使能, ENABLE/DISABLE; 优先级(低到高) Priority_0,Priority_1,Priority_2,Priority_3
}
