#include "motor.h"

motor_t motor[MOTOR_LIST] =
{
	[CHASSIS_LF] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id      = CHASSIS_LF_ID,
		
		.init = &motor_class_init,
	},
	
	[CHASSIS_RF] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id      = CHASSIS_RF_ID,
		
		.init = &motor_class_init,
	},
		
	[CHASSIS_LB] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id      = CHASSIS_LB_ID,
		
		.init = &motor_class_init,
	},
	
	[CHASSIS_RB] = {
	
		.id.drive_type = M_CAN1,
		.id.motor_type = RM3508,
		.id.rx_id      = CHASSIS_RB_ID,
		
		.init = &motor_class_init,
	},
	
	[GIMBAL_YAW] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = GM6020,
		.id.rx_id 		 = GIMBAL_YAW_ID,
		
		.init = motor_class_init,
	},
	
	[GIMBAL_PITCH] = {
	
		.id.drive_type = M_CAN2,
		.id.motor_type = GM6020,
		.id.rx_id 		 = GIMBAL_PITCH_ID,
		
		.init = motor_class_init,
	},

	[LAUNCH_FRIC_L] = {

		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id 		 = LAUNCH_FRIC_L_ID,

		.init = motor_class_init,
	},


	[LAUNCH_FRIC_R] = {

		.id.drive_type = M_CAN2,
		.id.motor_type = RM3508,
		.id.rx_id 		 = LAUNCH_FRIC_R_ID,

		.init = motor_class_init,
	},

	[LAUNCH_DIAL] = {

		.id.drive_type = M_CAN2,
		.id.motor_type = RM2006,
		.id.rx_id 		 = LAUNCH_DIAL_ID,

		.init = motor_class_init,
	},

};

/*--------------------------------------------------------------------------*/


/**
 *	@brief	�����ʼ��
 */
void motor_class_init(struct motor_class_t *motor)
{
	if(motor == NULL)
	{
		return;
	}
	
	motor->state.work_state = M_OFFLINE;
	
	motor->state.offline_cnt = 0;
	motor->state.offline_cnt_max = 100;	
	

	if(motor->id.drive_type == M_CAN1 || motor->id.drive_type == M_CAN2)
	{
		if(motor == NULL)
		{
			return;
		}
	
		motor->rx = can_rx_buff;
	}
	
	motor->pid_init     	= motor_class_pid_init;
	motor->heartbeat    	= motor_class_heartbeat;	
	
	motor->c_stuck_flag 	= motor_class_stucking_flag;
	motor->c_idling_flag	= motor_class_idling_flag;
	motor->c_offset     	= motor_offset;
	motor->c_judge_dir  	= motor_judge_dir;
	
	motor->c_speed = motor_pid_speed;
	motor->c_angle = motor_pid_angle;
	motor->c_posit = motor_pid_position;	
	
	motor->c_pid1 = motor_pid_single;
	motor->c_pid2 = motor_pid_double;		
	
	motor->state.init_flag = M_INIT;
}

/**
 *	@brief	���pid��ʼ��
 */
void motor_class_pid_init(struct motor_pid *pid, float *buff)
{
	if(pid == NULL || buff == NULL)
	{
		return;
	}
	
	pid->info.target   = 0;
	pid->info.measure  = 0;	
	pid->info.err      = 0;	
	pid->info.last_err = 0;
	pid->info.integral = 0;
	pid->info.pout     = 0;
	pid->info.iout     = 0;
	pid->info.dout     = 0;	
	pid->info.out      = 0;		
	
	pid->set.kp           = buff[0];
	pid->set.ki           = buff[1];	
	pid->set.kd           = buff[2];
	pid->set.blind_err    = buff[3];
	pid->set.integral_max = buff[4];
	pid->set.iout_max     = buff[5];	
	pid->set.out_max      = buff[6];
	
	pid->info.init_flag = M_INIT;
}	

/**
 *	@brief	�������
 */
void motor_class_heartbeat(struct motor_class_t *motor)
{
	if(motor == NULL)
	{
		return;
	}

	if(motor->state.init_flag == M_DEINIT)
	{
		return;	
	}
	
	motor->state.offline_cnt++;
	
	if(motor->state.offline_cnt > motor->state.offline_cnt_max) 
	{
		motor->state.offline_cnt = motor->state.offline_cnt_max;
		motor->state.work_state = M_OFFLINE;
	}
	else 
	{
		if(motor->state.work_state == M_OFFLINE)
			motor->state.work_state = M_ONLINE;
	}
}

/**
*	@brief	��ת�жϣ����Ǳ�Ҫ������������һ����ס torque_limit:Ť����ֵ return:1Ϊ�� 0Ϊ�� 

�û����к����޸ģ��ع�����λ�Ͳ��̣��ٶȺ�λ�ã�1�ٶȣ�2λ��
 */
uint8_t motor_class_stucking_flag(struct motor_class_t *motor, uint8_t pid_type, float err_limit, uint16_t lock_cnt_max)
{
	uint8_t res = 0;
	
	
	if(motor->state.init_flag == M_DEINIT || motor->state.work_state == M_OFFLINE)return 0;	
	
	/*�ٶ�pid�Ķ�ת�ж�*/
	if(pid_type == 1)
	{
		if(m_abs(motor->pid.speed.info.err) > err_limit && m_abs(motor->rx_info.speed) < 5)
	  { 
	  	motor->mec_info.lock_cnt++;
		
		  if (motor->mec_info.lock_cnt == lock_cnt_max)
			  res = 1;
		  else
			  res = 0;
	  }
	  else
	  {
		  res = 0;
		  motor->mec_info.lock_cnt = 0;
  	}
	}
	
	/*λ��pid�Ķ�ת�ж�*/
	if(pid_type == 2)
	{
		if(m_abs(motor->pid.position.info.err) > err_limit && m_abs(motor->rx_info.speed) < 180)
	  { 
	  	motor->mec_info.lock_cnt++;
		
		  if (motor->mec_info.lock_cnt == lock_cnt_max)
			  res = 1;
		  else
			  res = 0;
	  }
	  else
	  {
		  res = 0;
		  motor->mec_info.lock_cnt = 0;
  	}
	}
	
	return res;
	
}

/**
  * @brief	�����ת��⣬������ת�ٴﵽ��ֵ����������һ��ʱ���ں�С���ж�Ϊ��ת��
  * @param	motor_class_t *motor
	* @author	JSJ
  * @retval uint8_t flag
  */
uint8_t motor_class_idling_flag(struct motor_class_t *motor, uint16_t speed_limit, int16_t i_min, uint8_t t_limit)
{
	uint8_t res = 0;
	
	if(m_abs(motor->rx_info.speed) > speed_limit)
	{
		if(m_abs(motor->rx_info.current) < i_min)		// ע��������С��
		{
			motor->mec_info.idle_cnt ++;
			
			if(motor->mec_info.idle_cnt >= t_limit)
				res = 1;
			else
				res = 0;
		}
		else
		{
			res = 0;
			motor->mec_info.idle_cnt = 0;
		}
	}
	else
	{
		res = 0;
		motor->mec_info.idle_cnt = 0;
	}
	
	return res;
}


/*-----------------------------------------------------------------
*���ݴ���
-----------------------------------------------------------------*/
/**
 *	@brief	���ڴ��� tar��Դ���� cycle:���ݷ�Χ
 */
static float motor_cycle(float tar,float cycle)
{
	if(tar < 0)          tar += cycle;
	else if(tar >=cycle) tar -= cycle;
	
	return tar;
}

/**
 *	@brief	����Ȧ���� angle��Դ���� cycle:���ݷ�Χ
 */
static float motor_half_cycle(float angle,float max)
{
	if(m_abs(angle) > (max/2))
	{	
		if(angle >= 0)
			angle += -max;		
		else
			angle +=  max;
	}
	return angle;
}

/**
 *	@brief	����������  limit:������ݷ�Χ
 */
void motor_judge_dir(struct motor_class_t *motor,uint16_t range)
{
	int16_t angle = 0;
	
	if(motor->rx_info.angle < motor->mec_info.mec_mid)
		 angle = motor->rx_info.angle - motor->mec_info.mec_mid + range;
	else
	   angle = motor->rx_info.angle - motor->mec_info.mec_mid;

	if(m_abs(angle - range/2) < range/4)
	{
		motor->mec_info.dir = MOTOR_B;
	}
	else 
	{
		motor->mec_info.dir = MOTOR_F;
	}	
}

/**
 *	@brief	�Ե���Ƕ���ƫ�� ����yaw��������ԭʼ��ǰΪ1777���޸ĺ�ǰΪ0 
 *            limit:������ݷ�Χ
 */
void motor_offset(struct motor_class_t *motor, uint16_t range)
{
	motor_rx_info_t *info = &motor->rx_info;
	
	int16_t angle = 0;
	
	if(info->angle < motor->mec_info.mec_mid)
		 angle = info->angle - motor->mec_info.mec_mid + range;
	else
	   angle = info->angle - motor->mec_info.mec_mid;
	
  angle = -angle + range + range/4;
	
	if(angle > range)angle = angle - range;

	angle = motor_cycle(angle - range/4,range);
	
	info->angle_offset = angle;			
}


/*-----------------------------------------------------------------
*���pid
-----------------------------------------------------------------*/

/**
 *	@brief	��ȡ��� tar - mea
 */
float motor_pid_err(motor_pid_t *pid,float measure)
{
	motor_pid_info_t *pid_info = &pid->info;
	
	if(pid->info.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	pid_info->measure = measure;
	pid_info->err = pid_info->target - pid_info->measure;
	return pid_info->err;

}

/**
 *	@brief	pid���� ������err����
 */
float motor_pid_cal(motor_pid_t *pid)
{
	
	if(pid->info.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	motor_pid_info_t *pid_info = &pid->info;
	motor_pid_set_t  *pid_set = &pid->set;	
	
	//�������ֵ(��Ҫ���������м������)
	//pid->err = err;
	if(m_abs(pid_info->err)<=(pid_set->blind_err))
		pid_info->err = 0;
	
	//����
	pid_info->integral += pid_info->err;
	pid_info->integral = m_constrain(pid_info->integral, -pid_set->integral_max, +pid_set->integral_max);
	
	//pid ��������
	pid_info->pout = pid_set->kp * pid_info->err;
	pid_info->iout = pid_set->ki * pid_info->integral;
	pid_info->dout = pid_set->kd * (pid_info->err - pid_info->last_err);
	
	pid_info->iout = m_constrain(pid_info->iout, -pid_set->iout_max, pid_set->iout_max);	
	//�ۼ�pid���ֵ
	pid_info->out = pid_info->pout + pid_info->iout + pid_info->dout;
	pid_info->out = m_constrain(pid_info->out, -pid_set->out_max, pid_set->out_max);

	pid_info->last_err = pid_info->err;
	
	return pid_info->out;
}

/**
 *	@brief	pid�ܿ��� �������⻷ �ڻ� �⻷�۲�ֵ �ڻ��۲�ֵ err������ʽ
 *          err_cal_mode��err������ʽ ��Ȧ�����ķ�֮һȦ 0��1��2 �ٶȻ�ʹ��0 yaw��ʹ��1
 *          ���ڻ�ΪNULL��ֻ�����⻷���⻷����ΪNULL
 *
 *  @return ���ؼ�����
 */
float motor_pid_ctrl(motor_pid_t *out, motor_pid_t *inn, float meas1, float meas2, char err_cal_mode)
{
	if(out == NULL)return 0;
	
	if(inn == NULL)
	{
		motor_pid_err(out , meas1);	
		switch(err_cal_mode)
		{
			case 0:			
				break;
			
			case 1:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				break;				
			
			case 2:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				out->info.err = motor_half_cycle(out->info.err, 4095);
				break;			
			
			case 3://Imuģʽ��-180~180
				out->info.err = motor_half_cycle(out->info.err, 360);
		}
		motor_pid_cal(out);
		
		return out->info.out;	
	}
	else 
	{
		/*--�⻷����--*/
		motor_pid_err(out , meas1);	
		switch(err_cal_mode)
		{
			case 0:			
				break;
			
			case 1:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				break;				
			
			case 2:
				out->info.err = motor_half_cycle(out->info.err, 8191);
				out->info.err = motor_half_cycle(out->info.err, 4095);
				break;			
			
			case 3:			//Imuģʽ��-180~180
				out->info.err = motor_half_cycle(out->info.err, 360);
		}
		motor_pid_cal(out);
		
		inn->info.target = out->info.out;	//Ŀ��ֵת�Ƶ��ٶȻ�
		
		/*--�ڻ�����--*/
		motor_pid_err(inn , meas2);  
		motor_pid_cal(inn);	
		
		return inn->info.out;	
	}
}

/**
 *	@brief	˫��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_double(motor_pid_t *out, motor_pid_t *inn, float meas1, float meas2, float tar, char err_cal_mode)
{

	out->info.target = tar;

	return motor_pid_ctrl(out,inn,meas1,meas2,err_cal_mode);

}


/**
 *	@brief	��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_single(motor_pid_t *out, float meas1, float tar)
{

	out->info.target = tar;

	return motor_pid_ctrl(out,NULL,meas1,NULL,0);

}

/**
 *	@brief	λ��pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_position(struct motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}
	
	if(motor->pid.position.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.position.info.target = target;
	
	
	motor->motor_set = motor_pid_ctrl(&motor->pid.position,&motor->pid.position_in,motor->rx_info.angle_sum,motor->rx_info.speed,0);
	
	return motor->motor_set;
	
}

/**
 *	@brief	�Ƕ�pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_angle(struct motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}

	if(motor->pid.angle.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.angle.info.target = target;
	
	motor->motor_set = motor_pid_ctrl(&motor->pid.angle,&motor->pid.angle_in,motor->rx_info.angle,motor->rx_info.speed,1);
	
	return motor->motor_set;
	
}

/**
 *	@brief	�ٶ�pid���� 
 *  @return ���ؼ�����
 */
float motor_pid_speed(struct motor_class_t *motor,float target)
{
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;
	}	

	if(motor->pid.speed.info.init_flag == M_DEINIT)
	{
		return 0;
	}	
	
	motor->pid.speed.info.target = target;
	
	motor->motor_set = motor_pid_ctrl(&motor->pid.speed,NULL,motor->rx_info.speed,NULL,0);  
	
	return motor->motor_set;
	
}



/**
 *	@brief	���������Ϣ
 */
uint8_t can_rx_buff(struct motor_class_t *motor, uint8_t *buff,uint8_t len)
{
	uint8_t res;
	
	if(motor == NULL || buff == NULL)
	{
		return 0;
	}
	
	if(motor->state.init_flag == M_DEINIT)
	{
		return 0;	
	}
	
	if(motor->id.motor_type > 0 && motor->id.motor_type <= 3)
	{
		// 3508 6020 2006
		get_rm_info(motor,buff);
	}
	
	return res;
}

/*-----------------------------------------------------------------
*�󽮵������
-----------------------------------------------------------------*/
/**
 *	@brief	��CAN�����ж�ȡ�����λ�÷���
 */
static uint16_t CAN_GetMotorAngle(uint8_t *rxData)
{
	uint16_t angle;
	angle = ((uint16_t)rxData[0] << 8| rxData[1]);
	return angle;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ת��ת�ٷ���
 */
static int16_t CAN_GetMotorSpeed(uint8_t *rxData)
{
	int16_t speed;
	speed = ((uint16_t)rxData[2] << 8| rxData[3]);
	return speed;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ��ת�ص�������
 */
static int16_t CAN_GetMotorCurrent(uint8_t *rxData)
{
	int16_t current;
	current = ((int16_t)rxData[4] << 8 | rxData[5]);
	return current;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ�����ת��
 */
static int16_t CAN_GetMotorTorque(uint8_t *rxData)
{
	int16_t torque;
	torque = ((int16_t)rxData[4] << 8 | rxData[5]);
	return torque;
}

/**
 *	@brief	��CAN�����ж�ȡ�����ʵ���¶�
 */
static uint8_t CAN_GetMotorTemperature(uint8_t *rxData)
{
	uint8_t temperature;
	temperature = rxData[6];
	return temperature;
}

/**
 *	@brief	can������Ϣ����
 *  @return
 */
static void get_rm_info(struct motor_class_t *motor, uint8_t *rxBuf)
{
	int16_t err;

	motor_rx_info_t *motor_info = &motor->rx_info;
	
	motor_info->angle   		= CAN_GetMotorAngle(rxBuf);	
	motor_info->speed   		= CAN_GetMotorSpeed(rxBuf);
	motor_info->current 		= CAN_GetMotorCurrent(rxBuf);
	motor_info->torque  		= CAN_GetMotorTorque(rxBuf);	
	motor_info->temperature = CAN_GetMotorTemperature(rxBuf);

	if(!motor_info->angle_prev && !motor_info->angle_sum)
		err = 0;
	else
		err = motor_info->angle - motor_info->angle_prev;
	
	/* ����� */
	if(m_abs(err) > 4095)
	{
		/* 0�� -> 8191 */
		if(err >= 0)
			motor_info->angle_sum += -8191 + err;
		/* 8191�� -> 0 */
		else
			motor_info->angle_sum += 8191 + err;
	}
	/* δ����� */
	else
	{
		motor_info->angle_sum += err;
	}
	
	motor_info->angle_prev = motor_info->angle;		
	
	motor->state.offline_cnt = 0;
}