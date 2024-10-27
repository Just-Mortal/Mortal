#include "slave.h"

static void Slave_Data_Send(struct Slave_Struct_t *Slave);
static void Slave_Heartbeat(struct Slave_Struct_t *Slave);

extern CAN_HandleTypeDef hcan1;

Slave_Info_t	slave_info;

Slave_t slave ={
	.info			=	&slave_info,
	
	.send			= Slave_Data_Send,
	.heartbeat= Slave_Heartbeat,
};

void Slave_Data_Send(struct Slave_Struct_t *Slave)
{
	CAN_Send_Data(&hcan1, 0xA5, Slave->tx_buf);
}

void Slave_Heartbeat(struct Slave_Struct_t *Slave)
{
	Slave_Info_t *slave_info = Slave->info;

	slave_info->offline_cnt++;
	
	if(slave_info->offline_cnt > slave_info->offline_max_cnt)
	{
		slave_info->offline_cnt = slave_info->offline_max_cnt;
		Slave->work_state 			= DEV_OFFLINE;
	} 
	else
	{
		if(Slave->work_state == DEV_OFFLINE)	/* ÀëÏß->ÔÚÏß */
			Slave->work_state = DEV_ONLINE;
	}
}
