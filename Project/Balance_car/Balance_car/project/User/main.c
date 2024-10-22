#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "Encoder.h"
#include "Motor.h"
#include "PWM.h"
#include "Timer.h"
#include "Exti.h"
#include "Control.h"
#include "sys.h"
//#include "Buletooth.h"
#include "Ultralsound.h"

// A -> Right
// B -> left

extern double Vertical_Kp ;
extern int ECOH_Num ;

float Pitch ,Roll ,Yaw;
int16_t GYROX ,GYROY ,GYROZ ,ACCX ,ACCY ,ACCZ;

float Angle_aim = 0;				//？？？？

int8_t Vertical_Out ,Turn_Out , PWM_out;
float Velocity_Out ;

int16_t SpeedA_Real ,SpeedB_Real;
int8_t  SpeedA_Set ,SpeedB_Set ;

//uint16_t Date_RX;
double len = 0;
int Num = 0;

int main(void)
{
	OLED_Init();
	Encoder_Init();
	Motor_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	MPU6050_EXIT_Init();
	Ultralsound_Init();
//	Serial_Init();
	
	OLED_Clear();
	
	while(1)
	{
		len = get_length();
		Num ++;									//Num用于检测while循环的次数
		OLED_ShowSignedNum(1,1,len,3);
		OLED_ShowSignedNum(2,1,Num,3);
		OLED_ShowSignedNum(3,1,ECOH_Num,3);		//ECOH_Num大概表示进入定时器中断的次数
		OLED_ShowString(1,5,"cm");
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetFlagStatus(EXTI_Line5) != RESET)			//10ms进入一次中断
	{
		if(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_5) == 0)
		{
			EXTI_ClearITPendingBit(EXTI_Line5);
			
			SpeedA_Real = Encoder_A_Get();					//RIGHT
			SpeedB_Real = Encoder_B_Get();					//LEFT
			
			MPU_Get_Gyroscope(&GYROX ,&GYROY ,&GYROZ);
			MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw);
			
//			// Pitch = Angle_real	->	GYROY
//			// GYROZ = GyroZ_Real
//			
			Vertical_Out = Vertical_Control(Pitch ,Angle_aim , GYROY);
			Velocity_Out = Velocity_Control(SpeedA_Real ,SpeedB_Real);
			Turn_Out = Turn_Control(GYROZ);
			
			PWM_out = Vertical_Out - Vertical_Kp * Velocity_Out;
			SpeedA_Set = PWM_out - Turn_Out ;
			SpeedB_Set = PWM_out + Turn_Out ;
			
			MotorA_SetSpeed(SpeedA_Set);
			MotorB_SetSpeed(SpeedB_Set);
		}
	}
}

//外部中断每10mS进入一次中断
//但是外部中断的持续时间可能很短，不影响定时器中断的执行
