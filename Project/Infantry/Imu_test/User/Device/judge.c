/* Includes ------------------------------------------------------------------*/
#include "judge.h"

/* Private function prototypes -----------------------------------------------*/
extern void judge_update(Judge_t *self, uint16_t canId, uint8_t *rxbuf);

static void judge_heartbeat(Judge_t *jud_sen);

/* Private variables ---------------------------------------------------------*/
judge_info_t judge_info = {
	
	.offline_max_cnt = 500,
	
	.game_robot_status.chassis_power_limit					= 45,
	.game_robot_status.shooter_barrel_heat_limit		= 200,
	.game_robot_status.shooter_barrel_cooling_value	=	10,
	
	
};

/* Exported variables --------------------------------------------------------*/
Judge_t judge = 
{
	.work_state = DEV_OFFLINE,
	.info 			= &judge_info,
	
	.update 		= judge_update,
	.heartbeat 	= judge_heartbeat,
};

/* Private functions ---------------------------------------------------------*/
void judge_heartbeat(Judge_t *jud_sen)
{
	judge_info_t *jud_info = jud_sen->info;

	jud_info->offline_cnt++;
	
	if(jud_info->offline_cnt > jud_info->offline_max_cnt)
	{
		jud_info->offline_cnt = jud_info->offline_max_cnt;
		jud_sen->work_state = DEV_OFFLINE;
	}
	else
	{
		/* ÀëÏß->ÔÚÏß */
		if(jud_sen->work_state == DEV_OFFLINE)
		{
			jud_sen->work_state = DEV_ONLINE;
		}
	}
}
