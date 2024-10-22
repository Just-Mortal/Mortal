#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"
#include "Encoder.h"
#include "Control.h"

/*
关于数据类型的问题：
1)Vertical_Control和Turn_Control的输出是Set_Speed(int8_t)的输入，所以函数输出一定是int8_t
2)void MPU6050_GetDate(int16_t *ACCX ,int16_t *ACCY ,int16_t *ACCZ,
					 int16_t *GYROX,int16_t *GYROY,int16_t *GYROZ)
u8 MPU6050_DMP_Get_Data(float *pitch,float *roll,float *yaw)
int16_t Encoder_A_Get(void)		int16_t Encoder_B_Get(void)
加速度，角速度和角度的数据类型参考MPU函数和Encoder函数
3)Velocity_Control的输出为期望角度，为float
*/

double 	Vertical_Kp = 4.8,
		Vertical_Kd = 0.018;
double 	Velocity_Kp = -0.275,
		Velocity_Ki = -0.001375;
double 	Turn_Kp 	= -0.007;

int16_t Speed_aim = 0;				//期望速度	用于蓝牙和循迹
	
/*
	直立环控制		PD : PID_OUT = Kp*Ek + Kd*D(Ek)
	输出：控制速度Set_Speed(int8_t)
	输入：期望角度 ,真实角度 ,真实角速度(int16_t)
		期望角度是速度环的输出 Angle_aim(float)
		期望角速度 = 0 ,真实角速度 = D(角度)
*/
int8_t Vertical_Control(float Angle_real ,float Angle_aim ,int16_t GyroY_real)	//数据类型待定
{
	int8_t Vertical_OUT;
	int8_t GyroY_aim = 0 ;			//期望角速度 = 0 即处在某角度保持稳定状态	没问题
	
	//直立环控制输出计算
	Vertical_OUT = Vertical_Kp*(Angle_real - Angle_aim) + Vertical_Kd*(GyroY_real - GyroY_aim) ;
	
	//Vertical_Kp*Angle_real			？？？？
	
	return Vertical_OUT ;
}

/*
	直立环控制		PI :	PID_OUT = Kp*Ek + Ki*S(Ek)
	输出：期望角度 -> 直立环的输入 Angle_aim(float)
	输入：真实速度 -> 通过左右编码电机读出
	期望速度可以自行设置，结果是对应不同的期望角度（保持匀速运动且不倒的角度）
	这里期望角度先给0
*/
float Velocity_Control(int16_t SpeedA_Real ,int16_t SpeedB_Real)
{
	float Velocity_OUT , a;
	static int16_t Speed_Err ,Speed_Err_Lowout ,Speed_Err_Lowout_Last ,Speed_Err_S ;
	
	a = 0.7 ;
	
	//计算速度偏差
	Speed_Err = SpeedA_Real + SpeedB_Real - Speed_aim ;
	//对速度偏差进行低通滤波
	Speed_Err_Lowout = (1-a)*Speed_Err + a*Speed_Err_Lowout_Last ;
	Speed_Err_Lowout_Last = Speed_Err_Lowout ;
	//对速度偏差进行积分
	Speed_Err_S +=  Speed_Err_Lowout ;
	//积分限幅
	Speed_Err_S = Speed_Err_S>10000?10000:(Speed_Err_S<-10000?-10000:Speed_Err_S);
	//速度环控制输出计算
	Velocity_OUT = Velocity_Kp*Speed_Err_Lowout + Velocity_Ki*Speed_Err_S ;
	
	return Velocity_OUT;
}

/*
	转向环控制		P ：	PID_OUT = Kp*Ek
	输出：速度环控制	Set_Speed(int8_t)的部分输入
	输入：Z轴角速度(int16_t)
	目的：保持直线前行
*/
int8_t Turn_Control(int16_t GyroZ_Real)
{
	int8_t Turn_OUT ;

	//转向环控制输出计算
	Turn_OUT = Turn_Kp*(GyroZ_Real-0);
	
	return Turn_OUT;
}

/*		转向环PD控制
int8_t Turn_Control(int16_t SpeedA_Real ,int16_t SpeedB_Real,int16_t GyroZ_Real)
{
	int8_t Turn_OUT ;
	int16_t encoder_temp ,bias ,Turn_Amplitude=50;
	float Turn_Kp ,Turn_Kd;
	
	Turn_Kd = -0.5 ;
	
	encoder_temp = SpeedA_Real - SpeedB_Real;
	bias += encoder_temp; 					//对角速度积分
	
	if(bias > Turn_Amplitude)  
    	bias = Turn_Amplitude;
	if(bias < -Turn_Amplitude) 
		bias = -Turn_Amplitude;

	//转向环控制输出计算
	Turn_OUT = Turn_Kp*bias + Turn_Kd*(GyroZ_Real-0);
	
	return Turn_OUT;
}
*/

