#include "module.h"

Module_Flag_t module_flag;
Module_Info_t	module_info;
Module_t	module = 
{
	.flag	=	&module_flag,
	.info	=	&module_info,
};

static void Module_Update(void);

void Module_Ctrl(void)
{
	four_link.init(&four_link);

	if(rc.dev->work_state == DEV_OFFLINE)
	{
		module.mode = SLEEP;

		module.info->joint_true_set = 0;
		module.info->joint_virt_set = 0;
		
		module.flag->INIT_OK				=	false;
		module.flag->REAL_INIT_OK		=	false;
		module.flag->VIRT_INIT_OK		=	false;
	}
	else
	{
		if(!module.flag->INIT_OK)
		{
			module.mode = INIT;
		
			if(!module.flag->REAL_INIT_OK)
				module.info->joint_true_set = motor[JOINT_REAL].c_speed(&motor[JOINT_REAL], -100.f);
			if(!module.flag->VIRT_INIT_OK)
				module.info->joint_virt_set = motor[JOINT_VIRT].c_speed(&motor[JOINT_VIRT], -100.f);
			
			if(motor[JOINT_REAL].c_stuck_flag(&motor[JOINT_REAL], 1, 30, 120))
			{
				module.info->joint_true_set = 0.f;
				module.flag->REAL_INIT_OK	=	true;
				motor[JOINT_REAL].rx_info.angle_sum = 0.f;
			}
			if(motor[JOINT_VIRT].c_stuck_flag(&motor[JOINT_VIRT], 1, 30, 120))
			{
				module.info->joint_virt_set = 0.f;
				module.flag->VIRT_INIT_OK	=	true;
				motor[JOINT_VIRT].rx_info.angle_sum = 0.f;
			}
				
			if(module.flag->REAL_INIT_OK && module.flag->VIRT_INIT_OK)
				module.flag->INIT_OK = true;
		}
		else
		{
			module.mode = WORK;
			
			four_link.length->l0_target += rc.dev->info->ch3 / 66000.f;
			four_link.length->l0_target = constrain(four_link.length->l0_target, 14.f, 22.f);
			
//			four_link.length->phi0_target += rc.dev->info->ch2 / 660000.f;
//			four_link.length->phi0_target = constrain(four_link.length->phi0_target, -0.5f, 0.5f);
			
			Module_Update();
			
			module.info->joint_true_set = constrain(four_link.real_set, -6000.f, 6000.f);
			module.info->joint_virt_set = -constrain(four_link.virt_set, -6000.f, 6000.f);
		}
		
		CAN1_Send_200_Motor(module.info->joint_true_set, module.info->joint_virt_set, 0, 0);
		
	}
	
	module.info->real_i_measure	=	motor[JOINT_REAL].rx_info.current;
	module.info->virt_i_measure	=	motor[JOINT_VIRT].rx_info.current;

}

void Module_Update(void)
{
	four_link.angle->phi1 = PI - motor[JOINT_REAL].rx_info.angle_sum / (8192.f * 36.f) * 2.f * PI;
	four_link.angle->phi4 = motor[JOINT_VIRT].rx_info.angle_sum / (8192.f * 36.f) * 2.f * PI;
	
	four_link.update(&four_link);
	
	
}

