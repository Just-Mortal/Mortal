#include "gimbal.h"

//	输出函数
static void Gimbal_Init(struct Gimbal_Struct_t* Gimbal);
static void Gimbal_Work(struct Gimbal_Struct_t* Gimbal);
static void Gimbal_Heartbeat(struct Gimbal_Struct_t* Gimbal);

//	工作函数
static void Gimal_Update(Gimbal_t* Gimbal);
static void Gimbal_Control(Gimbal_t* Gimbal);
static void Gimbal_Selfprotect(Gimbal_t *Gimbal);

//	处理函数
static void Gimbal_Yaw_Angle_Check(Gimbal_t* Gimbal);
static void Gimbal_Pch_Angle_Check(Gimbal_t* Gimbal);
static void Gimbal_Yaw_Imu_PidCtrl(Gimbal_t* Gimbal);
static void Gimbal_Pch_Imu_PidCtrl(Gimbal_t* Gimbal);
static void Gimbal_Yaw_Mec_PidCtrl(Gimbal_t* Gimbal);
static void Gimbal_Pch_Mec_PidCtrl(Gimbal_t* Gimbal);

Gimbal_Target_t 		gimbal_target;
Gimbal_Measure_t 		gimbal_measure;
Gimbal_Cnt_t 				gimbal_cnt;
Gimbal_Flag_t				gimbal_flag;
Gimbal_Conf_t				gimbal_conf=
{
	.yaw_mid_angle	=	GIMBAL_YAW_MIDDLE_ANGLE
};

Gimbal_Dev_t gimbal_dev = 
{
	.yaw 		= &motor[GIMBAL_YAW],
	.pch 		= &motor[GIMBAL_PITCH],
	
	.imu		=	&imu,
};

Gimbal_Info_t gimbal_info = 
{
	.target 	= &gimbal_target,
	.measure 	= &gimbal_measure,
};

Gimbal_t gimbal = 
{
	.dev	= &gimbal_dev,
	.info = &gimbal_info,
	
	.cnt  =	&gimbal_cnt,
	.flag = &gimbal_flag,
	.conf	=	&gimbal_conf,
	
	.init 			= Gimbal_Init,
	.work 			= Gimbal_Work,
	.heartbeat 	= Gimbal_Heartbeat,
};

/*-------------------- Exported functions begin------------------------------------*/
void Gimbal_Init(struct Gimbal_Struct_t* Gimbal)
{
	Gimbal->flag->GIMBAL_INIT_OK_FLAG 	= false;
	
	Gimbal->info->target->yaw_mec_angle = 0;
	Gimbal->info->target->pch_mec_angle = 0;
	
	Gimbal->cnt->init_cnt				= 0;
	Gimbal->cnt->head_swap_cnt	= 0;
}

void Gimbal_Work(struct Gimbal_Struct_t* Gimbal)
{
	Gimal_Update(Gimbal);
	
	Gimbal_Control(Gimbal);
	
	Gimbal_Selfprotect(Gimbal);
}

void Gimbal_Heartbeat(struct Gimbal_Struct_t* Gimbal)
{
	Gimbal->dev->yaw->heartbeat(Gimbal->dev->yaw);
	Gimbal->dev->pch->heartbeat(Gimbal->dev->pch);
	
	Gimbal->dev->imu->heartbeat(Gimbal->dev->imu);
}

/*-------------------- Exported functions end------------------------------------*/

/*--------------------- Private function begin--------------------------*/
void Gimbal_Selfprotect(Gimbal_t *Gimbal)
{
//	if(	Gimbal->dev->yaw->state.work_state 	== M_OFFLINE ||
//			Gimbal->dev->pch->state.work_state	== M_OFFLINE ||
//			Gimbal->dev->imu->info.state				!= IMU_ONLINE||
//			Gimbal->dev->imu->info.correct			!= IMU_OK)
//	{
//		Gimbal->info->pch_motor_set = 0;
//		Gimbal->info->yaw_motor_set	= 0;
//	}
}

void Gimal_Update(Gimbal_t* Gimbal)
{
	motor_t		*Yaw, *Pch;
	imu_t			*Imu;
	
	Yaw = Gimbal->dev->yaw;
	Pch = Gimbal->dev->pch;
	Imu = Gimbal->dev->imu;

	Gimbal->info->measure->pch_imu_angle_pre = Gimbal->info->measure->pch_imu_angle;

	Gimbal->info->measure->yaw_mec_angle 	= Gimbal->conf->yaw_mid_angle -	Yaw->rx_info.angle;
	Gimbal->info->measure->pch_mec_angle 	= GIMBAL_PCH_MIDDLE_ANGLE -	Pch->rx_info.angle;
	Gimbal->info->measure->yaw_mec_speed 	= Imu->data.worldGyr.z ;
	Gimbal->info->measure->pch_mec_speed 	= Imu->data.worldGyr.y ;

	Gimbal->info->measure->yaw_imu_angle = Imu->data.rpy.yaw ;
	Gimbal->info->measure->pch_imu_angle = Imu->data.rpy.pitch ;
	Gimbal->info->measure->pch_imu_speed = Imu->data.worldGyr.y ;
	Gimbal->info->measure->yaw_imu_speed = Imu->data.worldGyr.z ;
	Gimbal->info->measure->yaw_imu_speed	= Imu->data.worldGyr.z ;
	
	Gimbal_Yaw_Angle_Check(Gimbal);
	Gimbal_Pch_Angle_Check(Gimbal);
	
	if(Gimbal->flag->GIMBAL_INIT_OK_FLAG == false && Gimbal->mode != GIM_SLEEP)
	{
		Gimbal->mode = GIM_MEC;
		Gimbal->cnt->init_cnt ++;
		
		if(Gimbal->cnt->init_cnt > GIMBAL_INIT_RPOCESS_TIME)
		{
			Gimbal->cnt->init_cnt = 0;
			Gimbal->flag->GIMBAL_INIT_OK_FLAG = true;
		}
	}
}

void Gimbal_Control(Gimbal_t* Gimbal)
{
	switch(Gimbal->mode)
	{
		case GIM_SLEEP:
			Gimbal->info->yaw_motor_set = 0;
			Gimbal->info->pch_motor_set = 0;
			break;
			
		case GIM_IMU:
			Gimbal_Yaw_Imu_PidCtrl(Gimbal);
			Gimbal_Pch_Imu_PidCtrl(Gimbal);
			
			/*机械角度目标值同步*/
		  Gimbal->info->target->pch_mec_angle = Gimbal->info->measure->pch_mec_angle;
			Gimbal->info->target->yaw_mec_angle = 0;
			break;
			
		case GIM_MEC:
			Gimbal_Yaw_Mec_PidCtrl(Gimbal);
			Gimbal_Pch_Mec_PidCtrl(Gimbal);
			
			if(abs(Gimbal->info->measure->pch_mec_angle) 			<= GIMBAL_INIT_ANGLE_ERR && \
				(abs(Gimbal->info->measure->yaw_mec_angle) 			<= GIMBAL_INIT_ANGLE_ERR ||	\
				 abs(Gimbal->info->measure->yaw_mec_angle-4096)	<= GIMBAL_INIT_ANGLE_ERR))
			{
					Gimbal->flag->GIMBAL_INIT_OK_FLAG =true;
					Gimbal->cnt->init_cnt = 0;
			}
			
			/*陀螺仪角度目标值同步*/
			Gimbal->info->target->yaw_imu_angle = Gimbal->info->measure->yaw_imu_angle;
		  Gimbal->info->target->pch_imu_angle = Gimbal->info->measure->pch_imu_angle;
			
			break;
		
		default:
			break;
	}
}

/*-------------------- 处理函数 begin --------------------*/
void Gimbal_Yaw_Angle_Check(Gimbal_t* Gimbal)
{
	// 限制 yaw 角度：	(target 和 measure)
	// imu -180~180			|		mec -4096~4096
	
	Gimbal_Info_t *Info = Gimbal->info;
	float angle;
	
	angle = Info->measure->yaw_imu_angle;
	while (abs(angle) > 180)
	{
		angle -= 360 * sgn(angle);
	}
	Info->measure->yaw_imu_angle = angle;
	
	angle = Info->target->yaw_imu_angle;
	while (abs(angle) > 180)
	{
		angle -= 360 * sgn(angle);
	}
	Info->target->yaw_imu_angle = angle;
	
	angle = Info->measure->yaw_mec_angle;
	while (abs(angle) > 4096)
	{
		angle -= 8192 * sgn(angle);
	}
	Info->measure->yaw_mec_angle = angle;
	
	angle = Info->target->yaw_mec_angle;
	while (abs(angle) > 4096)
	{
		angle -= 8192 * sgn(angle);
	}
	Info->target->yaw_mec_angle = angle;
}

void Gimbal_Pch_Angle_Check(Gimbal_t* Gimbal)
{
	// 限制 pitch 角度:
	//			|	measure		|		target
	// imu	|	不需要		|		软件限位
	// mec 	|-4096~4096	|		软件限位
	
	Gimbal_Info_t *Info = Gimbal->info;
	float angle;
	
	float pch_imu = Info->measure->pch_imu_angle;
	float pch_mec = Info->measure->pch_mec_angle;
	float pch_imu_pre	=	Info->measure->pch_imu_angle_pre;
	float pch_filter	=	pch_imu_pre*0.8f + pch_imu*0.2f;
	
	angle = Info->measure->pch_mec_angle;
	while (abs(angle) > 4096)
	{
		angle -= 8192 * sgn(angle);
	}
	Info->measure->pch_mec_angle = angle;
	
	angle = Info->target->pch_mec_angle;
	if(angle > GIMBAL_PCH_MEC_UP_ANGLE)
	{
		angle = GIMBAL_PCH_MEC_UP_ANGLE;
	}
	if(angle < GIMBAL_PCH_MEC_DOWN_ANGLE)
	{
		angle = GIMBAL_PCH_MEC_DOWN_ANGLE;
	}
	Info->target->pch_mec_angle = angle;
	
	angle				= Info->target->pch_imu_angle;
	if(angle > GIMBAL_PCH_IMU_UP_ANGLE + pch_filter + pch_mec/22.755f)
		angle = GIMBAL_PCH_IMU_UP_ANGLE + pch_filter + pch_mec/22.755f;
	if(angle < GIMBAL_PCH_IMU_DOWN_ANGLE + pch_filter + pch_mec/22.755f)
		angle = GIMBAL_PCH_IMU_DOWN_ANGLE + pch_filter + pch_mec/22.755f;

	Info->target->pch_imu_angle = angle;
}


/*---------- 输出函数 ----------*/
// err_cal_mode 角度误差计算方式	->	1=>8192			3=>360
void Gimbal_Yaw_Mec_PidCtrl(Gimbal_t* Gimbal)
{
	motor_t *motor = Gimbal->dev->yaw;
	/*输出前检查目标值*/
	Gimbal_Yaw_Angle_Check(Gimbal);
	/*输出*/
	Gimbal->info->yaw_motor_set = \
	motor->c_pid2(&motor->pid.mec_angle,&motor->pid.mec_angle_in,
								Gimbal->info->measure->yaw_mec_angle,
								Gimbal->info->measure->yaw_mec_speed,
								Gimbal->info->target->yaw_mec_angle,2);
}

void Gimbal_Pch_Mec_PidCtrl(Gimbal_t* Gimbal)
{
	motor_t *motor = Gimbal->dev->pch;
	/*输出前检查目标值*/
	Gimbal_Pch_Angle_Check(Gimbal);
	/*输出*/
	Gimbal->info->pch_motor_set = \
	motor->c_pid2(&motor->pid.mec_angle,&motor->pid.mec_angle_in,
								Gimbal->info->measure->pch_mec_angle,
								Gimbal->info->measure->pch_mec_speed,
								Gimbal->info->target->pch_mec_angle,1);
}

void Gimbal_Yaw_Imu_PidCtrl(Gimbal_t* Gimbal)
{
	motor_t *motor = Gimbal->dev->yaw;
	
	/*输出前检查目标值*/
	Gimbal_Yaw_Angle_Check(Gimbal);
	
//	if(pch->pid.imu_angle.info.target < -26.f)
//	{
//		motor->pid.imu_angle.set.kp			= 10.f;
//		motor->pid.imu_angle_in.set.kp	= 300.f;
//	}
//	else
//	{
//		motor->pid.imu_angle.set.kp			= 20.f;
//		motor->pid.imu_angle_in.set.kp	= 600.f;
//	}
	
	/*输出*/
	Gimbal->info->yaw_motor_set = \
	motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
								Gimbal->info->measure->yaw_imu_angle,
								Gimbal->info->measure->yaw_imu_speed,
								Gimbal->info->target->yaw_imu_angle,3);
}

void Gimbal_Pch_Imu_PidCtrl(Gimbal_t* Gimbal)
{
	motor_t *motor = Gimbal->dev->pch;
	/*输出前检查目标值*/
	Gimbal_Pch_Angle_Check(Gimbal);
	
//	if(motor->pid.imu_angle.info.target < -26.f)
//	{
//		motor->pid.imu_angle.set.kp			= 160.f;
//		motor->pid.imu_angle_in.set.kp	= 20.f;
//	}
//	else
//	{
//		motor->pid.imu_angle.set.kp			= 40.f;
//		motor->pid.imu_angle_in.set.kp	= 90.f;
//	}
	
	/*输出*/
	Gimbal->info->pch_motor_set = \
	motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
	              Gimbal->info->measure->pch_imu_angle,
								Gimbal->info->measure->pch_imu_speed,
								Gimbal->info->target->pch_imu_angle,3);
	
	// pch feedforward
	// 拟合函数： set = 5.23 * mec_angle + 530.51
	#if PCH_FEEDFORWARD_SWITCH	== 1
	Gimbal->info->pch_feedforward = 5.23f * (float)Gimbal->info->measure->pch_mec_angle + 530.51f;
	Gimbal->info->pch_motor_set	+=	Gimbal->info->pch_feedforward;
	Gimbal->info->pch_motor_set = constrain(Gimbal->info->pch_motor_set, -28000, 28000);
	#endif
}
/*-------------------- 处理函数 end   --------------------*/
