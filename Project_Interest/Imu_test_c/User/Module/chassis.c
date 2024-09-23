/* Includes ------------------------------------------------------------------*/
#include "chassis.h"

// ������
#include "imu.h"

/* Private function prototypes -----------------------------------------------*/
static void Chassis_Update(Chassis_t* Chassis);
static void Chassis_Speed_Calculating(Chassis_t *Chassis);
static void Chassis_PidCtrl(Chassis_t* Chassis);
static void Chassis_SelfProtect(Chassis_t* Chassis);

//static void Chassis_Measure(Chassis_t* Chassis);

static void Chassis_Init(struct Chassis_Struct_t* Chassis);
static void Chassis_Work(struct Chassis_Struct_t* Chassis);
static void Chassis_Heartbeat(struct Chassis_Struct_t* Chassis);

/* Private variables ---------------------------------------------------------*/
/*---------------- ��������begin ----------------------*/
Chassis_Dev_t		chas_dev = {
	.chas_motor[CHAS_LF] = &motor[CHASSIS_LF],
	.chas_motor[CHAS_RF] = &motor[CHASSIS_RF],
	
	.chas_motor[CHAS_LB] = &motor[CHASSIS_LB],
	.chas_motor[CHAS_RB] = &motor[CHASSIS_RB],
};

Chassis_Measure_t	chassis_measure;
Chassis_Info_t		chassis_info ={
	.measure				= &chassis_measure,
};

Chassis_Conf_t		chassis_conf={
	.chas_spin_conf		= CHASSIS_TOP_SPEED,
	.chas_speed_limit	=	CHASSIS_SPEED_MAX,
};

Chassis_t chassis = {
	.mode 		= CHAS_SLEEP,
	
	.dev 			= &chas_dev,
	.info			= &chassis_info,
	.conf			=	&chassis_conf,
	
	.init 		= Chassis_Init,
	.work 		= Chassis_Work,
	.heartbeat= Chassis_Heartbeat,
};
/*---------------- ��������end ----------------------*/

/*-------------------- Exported functions begin------------------------------------*/
void Chassis_Init(struct Chassis_Struct_t* Chassis)
{
	Chassis->info->front_speed = 0;
	Chassis->info->right_speed = 0;
	Chassis->info->cycle_speed = 0;
	
	Chassis->info->chas_lf_set = 0;
	Chassis->info->chas_rf_set = 0;
	Chassis->info->chas_lb_set = 0;
	Chassis->info->chas_rb_set = 0;
}

void Chassis_Work(struct Chassis_Struct_t* Chassis)
{
	Chassis_Update(Chassis);
	
	Chassis_Speed_Calculating(Chassis);
	
	Chassis_PidCtrl(Chassis);
	
	Chassis_SelfProtect(Chassis);
	
	// test
//	Chassis_Measure(Chassis);
}

void Chassis_Heartbeat(struct Chassis_Struct_t* Chassis)
{
	Chassis->dev->chas_motor[CHAS_LF]->heartbeat(Chassis->dev->chas_motor[CHAS_LF]);
	Chassis->dev->chas_motor[CHAS_RF]->heartbeat(Chassis->dev->chas_motor[CHAS_RF]);
	Chassis->dev->chas_motor[CHAS_LB]->heartbeat(Chassis->dev->chas_motor[CHAS_LB]);
	Chassis->dev->chas_motor[CHAS_RB]->heartbeat(Chassis->dev->chas_motor[CHAS_RB]);
}
/*-------------------- Exported functions end------------------------------------*/


/*------------------------- Private functions begin --------------------------------*/
/*---------------------------- �����begin -----------------------------------*/
void Chassis_Update(Chassis_t* Chassis)
{
	Chassis->info->measure->chas_measure_lf = Chassis->dev->chas_motor[CHAS_LF]->rx_info.speed;
	Chassis->info->measure->chas_measure_rf = Chassis->dev->chas_motor[CHAS_RF]->rx_info.speed;
	Chassis->info->measure->chas_measure_lb = Chassis->dev->chas_motor[CHAS_LB]->rx_info.speed;
	Chassis->info->measure->chas_measure_rb = Chassis->dev->chas_motor[CHAS_RB]->rx_info.speed;
}

uint8_t 	chassis_acc_flag;
uint16_t	chassis_acc_cnt;
void Chassis_Speed_Calculating(Chassis_t* Chassis)
{
	float	speed_lf, speed_rf, speed_lb, speed_rb;
	float max_speed = Chassis->conf->chas_speed_limit ;

	float front = Chassis->info->front_speed;	// ǰΪ��
	float right = Chassis->info->right_speed;	// ��Ϊ��
	float cycle = Chassis->info->cycle_speed;	// ����Ϊ��
	
	// ˳ʱ��Ϊ������ʱ��Ϊ��
	if(Chassis->mode == CHAS_NORMAL)
	{
		speed_lf = +front + right + cycle;
		speed_rf = -front + right + cycle;
		speed_lb = +front - right + cycle;
		speed_rb = -front - right + cycle;
	}
	
	
	// ----------- ֱ�߽��� begin -------------- //
	float lf, rf, lb, rb;
	
//	float err_front, err_right, err_cycle;
//	float chassis_kp = 0.1f;
//	float measure_cycle, measure_front, measure_right;
//	
//	// ���ӷ���
//	lf = -Chassis->info->measure->chas_measure_lf;
//	rf = -Chassis->info->measure->chas_measure_rf;
//	lb = -Chassis->info->measure->chas_measure_lb;
//	rb = -Chassis->info->measure->chas_measure_rb;

//	Chassis->info->measure->front = (lf - rf + lb - rb) / 4.f;
//	Chassis->info->measure->right = (lf + rf - lb - rb) / 4.f;
//	Chassis->info->measure->cycle = (lf + rf + lb + rb) / 4.f;

//	
//	// Ŀ��ֵ - ����ֵ
//	err_front = front - measure_front;
//	err_right = right - measure_right;
//	err_cycle = cycle - measure_cycle;
//	
//	speed_lf += (+err_front + err_right + err_cycle) * chassis_kp;
//	speed_rf += (-err_front + err_right + err_cycle) * chassis_kp;
//	speed_lb += (+err_front - err_right + err_cycle) * chassis_kp;
//	speed_rb += (-err_front - err_right + err_cycle) * chassis_kp;

	// ----------- ֱ�߽��� end   -------------- //
	
	// ----------- ���Ƽ��ٹ��ʣ������� begin --------- //
	float f, r;
	
	lf = -Chassis->info->measure->chas_measure_lf;
	rf = -Chassis->info->measure->chas_measure_rf;
	lb = -Chassis->info->measure->chas_measure_lb;
	rb = -Chassis->info->measure->chas_measure_rb;

	Chassis->info->measure->front = (lf - rf + lb - rb) / 4.f;
	Chassis->info->measure->right = (lf + rf - lb - rb) / 4.f;
	Chassis->info->measure->cycle = (lf + rf + lb + rb) / 4.f;

	f = Chassis->info->measure->front;
	r = Chassis->info->measure->right;

	Chassis->info->measure->move = sqrt(pow(f, 2.f)+pow(r, 2.f));
	if(Chassis->info->measure->move < Chassis->info->move_speed - 4000.f)
		chassis_acc_flag = 1;
	else if(Chassis->info->measure->move > Chassis->info->move_speed - 1500.f)
		chassis_acc_flag = 0;
		
	if(chassis_acc_flag == 1)
	{
		chassis_acc_cnt ++ ;
		
		if(chassis_acc_cnt > 200)
		{
			chassis_acc_flag	= 0;
			chassis_acc_cnt		=	0;
		}
	}
	else
		chassis_acc_cnt = 0;
	// ----------- ���Ƽ��ٹ��ʣ������� end   --------- //
	
	// ----------- ��ת��⣬���ƹ��ʣ���ֹ�� begin --------- //
//	Chassis->dev->chas_motor[CHAS_LF]->c_idling_flag(
	Chassis->info->measure->test_current = Chassis->dev->chas_motor[CHAS_LF]->rx_info.current;
	// ----------- ��ת��⣬���ƹ��ʣ���ֹ�� end  	--------- //
	
	
	if (fabs(speed_lf) > max_speed || fabs(speed_rf) > max_speed || \
			fabs(speed_lb) > max_speed || fabs(speed_rb) > max_speed)
	{
		float max_tar_speed = fmaxf(fabs(speed_lf),fmaxf(fabs(speed_rf),fmaxf(fabs(speed_lb), fabs(speed_rb))));
		float scale = max_speed / max_tar_speed;
	
		speed_lf *= scale;
		speed_rf *= scale;
		speed_lb *= scale;
		speed_rb *= scale;
	}

	// �����䷴��
	Chassis->info->chas_target_lf = -speed_lf;
	Chassis->info->chas_target_rf = -speed_rf;
	Chassis->info->chas_target_lb = -speed_lb;
	Chassis->info->chas_target_rb = -speed_rb;
}

void Chassis_PidCtrl(Chassis_t* Chassis)
{
	motor_t *motor_lf = Chassis->dev->chas_motor[CHAS_LF];
	motor_t *motor_rf = Chassis->dev->chas_motor[CHAS_RF];
	motor_t *motor_lb = Chassis->dev->chas_motor[CHAS_LB];
	motor_t *motor_rb = Chassis->dev->chas_motor[CHAS_RB];

	if(Chassis->mode != CHAS_SLEEP )
	{
		Chassis->info->chas_lf_set =	motor_lf->c_speed(motor_lf, Chassis->info->chas_target_lf);
		Chassis->info->chas_rf_set =	motor_rf->c_speed(motor_rf, Chassis->info->chas_target_rf);
		Chassis->info->chas_lb_set =	motor_lb->c_speed(motor_lb, Chassis->info->chas_target_lb);
		Chassis->info->chas_rb_set =	motor_rb->c_speed(motor_rb, Chassis->info->chas_target_rb);
	}
	else
	{
		Chassis->info->chas_lf_set =	0;
		Chassis->info->chas_rf_set =	0;
		Chassis->info->chas_lb_set =	0;
		Chassis->info->chas_rb_set =	0;
		
//		measure_front = 0.f;
//		measure_right = 0.f;
//		measure_cycle = 0.f;
	}
}

void Chassis_SelfProtect(Chassis_t* Chassis)
{
	// һ���������Ҫ��֤�Լ����Զ�
}

/*---------------------------- �����end -----------------------------------*/

// �����ã���������ʵ���ٶ� m/s ��ת�� rpm
//float measure_move, measure_spin , last_move;
//extern imu_t imu;
//void Chassis_Measure(Chassis_t* Chassis)
//{
//	float rate = 17.f / 268.f;		// ���ٱ�
//	float wheel_d = 155;					// ����ֱ�� mm
//	float cicle_d = 2*212;				// ����ֱ��	mm
//	float sqrt2_2 = 0.70710678;		// ����֮���Ŷ�
//	
//	float lf, rf, lb, rb;
//	float cycle, front, right;
//	float acc_x, acc_y, acc_m, a_move, imu_speed;
//	
//	// ���� rpm
//	lf = Chassis->info->chas_measure_lf * rate;
//	rf = Chassis->info->chas_measure_rf * rate;
//	lb = Chassis->info->chas_measure_lb * rate;
//	rb = Chassis->info->chas_measure_rb * rate;
//	
//	cycle = (lf + rf + lb + rb) / 4.f;
//	front = (lf + lb - rf - rb) / 4.f;
//	right = (lf + rb - rf - lb) / 4.f;

//	last_move		 = measure_move;
//	measure_spin = cycle * wheel_d / cicle_d;		// rpm
//	measure_move = sqrt(pow(front, 2.f) + pow(right, 2.f)) * PI * wheel_d * sqrt2_2;	// m/s
//	a_move			 = (measure_move - last_move) * 1000.f;	//	0.001 s
//	
//	// �ǲ��ǿ������ڴ򻬲��ԣ������Ա����ֲ��������ļ��ٶ�
//	// ��������ǲ�����ĺ�С�����ת���ܴ�˵����			Ӧ�ñȽ��ٶȰ�
//	// ���Ը��� yaw_angle �����õ� measure_front �� measure_right
//	// �Ӷ������� imu ��õ� acc_x��acc_y �� acc_z ���жԱ�
//	acc_x = imu.data.acc_gyr.acc_x;
//	acc_y = imu.data.acc_gyr.acc_y;
//	acc_m	=	sqrt(pow(acc_x, 2.f) + pow(acc_y, 2.f));
//	
//	// �������
//	imu_speed += acc_m;
//	// ��д��
//	if(acc_m < a_move / 2)
//		measure_move = imu_speed;
//}
/*------------------------- Private functions end --------------------------------*/
