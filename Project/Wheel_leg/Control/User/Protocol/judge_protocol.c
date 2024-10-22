 /* Includes ------------------------------------------------------------------*/
#include "judge_protocol.h"

/* 弹速统计 */
static void Speed_Statistic(void);

float cap_u;
bool CAP_STATE;
bool CHASSIS_POWER_UPDATE_FLAG;

/* Exported functions --------------------------------------------------------*/
uint16_t power_cnt;
uint16_t robot_cnt;
uint16_t hp_rx_cnt;
uint16_t cap_cnt;
void judge_update(Judge_t *self, uint16_t canId, uint8_t *rxbuf)
{
	switch (canId)
	{		
		case GAME_ROBOT_HP_ID:			// 1Hz
				
			hp_rx_cnt ++;
		
//			vision.info->tx_info->blood_1 = rxbuf[0];
//			vision.info->tx_info->blood_2 = rxbuf[1];
//			vision.info->tx_info->blood_3 = rxbuf[2];
//			vision.info->tx_info->blood_4 = rxbuf[3];
//			vision.info->tx_info->blood_5 = rxbuf[4];
//			vision.info->tx_info->blood_6 = rxbuf[5];
//			vision.info->tx_info->blood_7 = rxbuf[6];
//			vision.info->tx_info->blood_8 = rxbuf[7];
			break;

		case POWER_HEAT_ID:
			power_cnt ++;
		
			//	0		1
			memcpy((void*)&judge.info->power_heat_data.chassis_power_buffer, (void*)rxbuf, 2);
			//	2		3
			memcpy((void*)&judge.info->power_heat_data.shooter_id1_17mm_cooling_heat, (void*)&rxbuf[2], 2);
			//	4		5
			memcpy((void*)&judge.info->power_heat_data.chassis_power, (void*)&rxbuf[4], 4);
			
			CHASSIS_POWER_UPDATE_FLAG = true;
			
			break;
		
		case GAME_ROBOT_STATUS_ID:
		
			robot_cnt ++;

			// 0 1
			memcpy((void*)&judge.info->game_robot_status.shooter_barrel_heat_limit, (void*)&rxbuf[0], 2);
			// 2 3
			memcpy((void*)&judge.info->game_robot_status.chassis_power_limit, (void*)&rxbuf[2], 2);
			// 4
			judge.info->my_color	= rxbuf[4];
			vision.info->tx_info->my_color = judge.info->my_color;
			// 5 6
			memcpy((void*)&judge.info->game_robot_status.shooter_barrel_cooling_value, (void*)&rxbuf[5], 2);
			break;
	
		case SHOOT_DATA_ID:
			// 中断发送
			memcpy((void*)&judge.info->shoot_data.bullet_speed, (void*)rxbuf, 4);
			memcpy((void*)&judge.info->shoot_data.bullet_freq,	 &rxbuf[4],		1);
			judge.info->judge_measure_cnt ++;
			Speed_Statistic();
			break;

		// slave other info
		case SLAVE_INFO_ID:
			cap_cnt ++;
		
			CAP_STATE = rxbuf[0];
			memcpy((void*)&cap_u,	 &rxbuf[1],		4);
			break;
		
		default:
			break;
	}
	
	self->info->offline_cnt = 0;
}

#if 1

shoot_data_t 	shoot_statistics;
uint8_t 			cali_flag = 1;

void Speed_Statistic(void)
{
	float s_speed = judge.info->shoot_data.bullet_speed;

	if (s_speed <= 27.0f)
	{
		shoot_statistics.speed_260++;
		shoot_statistics.shoot_num--;
	}
	else if (s_speed >= 27.0f && s_speed <= 27.1f)
	{
		shoot_statistics.speed_270++;
	}
	else if (s_speed >= 27.1f && s_speed <= 27.2f)
	{
		shoot_statistics.speed_271++;
	}
	else if (s_speed >= 27.2f && s_speed <= 27.3f)
	{
		shoot_statistics.speed_272++;
	}
	else if (s_speed >= 27.3f && s_speed <= 27.4f)
	{
		shoot_statistics.speed_273++;
	}
	else if (s_speed >= 27.4f && s_speed <= 27.5f)
	{
		shoot_statistics.speed_274++;
	}
	else if (s_speed >= 27.5f && s_speed <= 27.6f)
	{
		shoot_statistics.speed_275++;
	}
	else if (s_speed >= 27.6f && s_speed <= 27.7f)
	{
		shoot_statistics.speed_276++;
	}
	else if (s_speed >= 27.7f && s_speed <= 27.8f)
	{
		shoot_statistics.speed_277++;
	}
	else if (s_speed >= 27.8f && s_speed <= 27.9f)
	{
		shoot_statistics.speed_278++;
	}
	else if (s_speed >= 27.9f && s_speed <= 28.0f)
	{
		shoot_statistics.speed_279++;
	}
	else if (s_speed >= 28.0f && s_speed <= 28.1f)
	{
		shoot_statistics.speed_280++;
	}
	else if (s_speed >= 28.1f && s_speed <= 28.2f)
	{
		shoot_statistics.speed_281++;
	}
	else if (s_speed >= 28.2f && s_speed <= 28.3f)
	{
		shoot_statistics.speed_282++;
	}
	else if (s_speed >= 28.3f && s_speed <= 28.4f)
	{
		shoot_statistics.speed_283++;
	}
	else if (s_speed >= 28.4f && s_speed <= 28.5f)
	{
		shoot_statistics.speed_284++;
	}
	else if (s_speed >= 28.5f && s_speed <= 28.6f)
	{
		shoot_statistics.speed_285++;
	}
	else if (s_speed >= 28.6f && s_speed <= 28.7f)
	{
		shoot_statistics.speed_286++;
	}
	else if (s_speed >= 28.7f && s_speed <= 28.8f)
	{
		shoot_statistics.speed_287++;
	}
	else if (s_speed >= 28.8f && s_speed <= 28.9f)
	{
		shoot_statistics.speed_288++;
	}
	else if (s_speed >= 28.9f && s_speed <= 29.0f)
	{
		shoot_statistics.speed_289++;
	}
	else if (s_speed >= 29.0f && s_speed <= 29.1f)
	{
		shoot_statistics.speed_290++;
	}
	else if (s_speed >= 29.1f && s_speed <= 29.2f)
	{
		shoot_statistics.speed_291++;
	}
	else if (s_speed >= 29.2f && s_speed <= 29.3f)
	{
		shoot_statistics.speed_292++;
	}
	else if (s_speed >= 29.3f && s_speed <= 29.4f)
	{
		shoot_statistics.speed_293++;
	}
	else if (s_speed >= 29.4f && s_speed <= 29.5f)
	{
		shoot_statistics.speed_294++;
	}
	else if (s_speed >= 29.5f && s_speed <= 29.6f)
	{
		shoot_statistics.speed_295++;
	}
	else if (s_speed >= 29.6f && s_speed <= 29.7f)
	{
		shoot_statistics.speed_296++;
	}
	else if (s_speed >= 29.7f && s_speed <= 29.8f)
	{
		shoot_statistics.speed_297++;
	}
	else if (s_speed >= 29.8f && s_speed <= 29.9f)
	{
		shoot_statistics.speed_298++;
	}
	else if (s_speed >= 29.9f && s_speed <= 30.0f)
	{
		shoot_statistics.speed_299++;
	}
	else if (s_speed >= 30.0f)
	{
		shoot_statistics.speed_300++;
		shoot_statistics.shoot_num--;
	}

	/*统计部分*/
	shoot_statistics.shoot_num++;

	if (cali_flag == 1)
	{
		shoot_statistics.mean = ( shoot_statistics.speed_270 * 27.0f + \
															shoot_statistics.speed_271 * 27.1f + \
															shoot_statistics.speed_272 * 27.2f + \
															shoot_statistics.speed_273 * 27.3f + \
															shoot_statistics.speed_274 * 27.4f + \
															shoot_statistics.speed_275 * 27.5f + \
															shoot_statistics.speed_276 * 27.6f + \
															shoot_statistics.speed_277 * 27.7f + \
															shoot_statistics.speed_278 * 27.8f + \
															shoot_statistics.speed_279 * 27.9f + \
															shoot_statistics.speed_280 * 28.0f + \
															shoot_statistics.speed_281 * 28.1f + \
															shoot_statistics.speed_282 * 28.2f + \
															shoot_statistics.speed_283 * 28.3f + \
															shoot_statistics.speed_284 * 28.4f + \
															shoot_statistics.speed_285 * 28.5f + \
															shoot_statistics.speed_286 * 28.6f + \
															shoot_statistics.speed_287 * 28.7f + \
															shoot_statistics.speed_288 * 28.8f + \
															shoot_statistics.speed_289 * 28.9f + \
															shoot_statistics.speed_290 * 29.0f + \
															shoot_statistics.speed_291 * 29.1f + \
															shoot_statistics.speed_292 * 29.2f + \
															shoot_statistics.speed_293 * 29.3f + \
															shoot_statistics.speed_294 * 29.4f + \
															shoot_statistics.speed_295 * 29.5f + \
															shoot_statistics.speed_296 * 29.6f + \
															shoot_statistics.speed_297 * 29.7f + \
															shoot_statistics.speed_298 * 29.8f + \
															shoot_statistics.speed_299 * 29.9f ) / (float)shoot_statistics.shoot_num;
		
		// Calculate the variance
		float sum_of_squares = \
		 (shoot_statistics.speed_270 * ((27.0f - shoot_statistics.mean) * (27.0f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_271 * ((27.1f - shoot_statistics.mean) * (27.1f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_272 * ((27.2f - shoot_statistics.mean) * (27.2f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_273 * ((27.3f - shoot_statistics.mean) * (27.3f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_274 * ((27.4f - shoot_statistics.mean) * (27.4f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_275 * ((27.5f - shoot_statistics.mean) * (27.5f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_276 * ((27.6f - shoot_statistics.mean) * (27.6f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_277 * ((27.7f - shoot_statistics.mean) * (27.7f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_278 * ((27.8f - shoot_statistics.mean) * (27.8f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_279 * ((27.9f - shoot_statistics.mean) * (27.9f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_280 * ((28.0f - shoot_statistics.mean) * (28.0f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_281 * ((28.1f - shoot_statistics.mean) * (28.1f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_282 * ((28.2f - shoot_statistics.mean) * (28.2f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_283 * ((28.3f - shoot_statistics.mean) * (28.3f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_284 * ((28.4f - shoot_statistics.mean) * (28.4f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_285 * ((28.5f - shoot_statistics.mean) * (28.5f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_286 * ((28.6f - shoot_statistics.mean) * (28.6f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_287 * ((28.7f - shoot_statistics.mean) * (28.7f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_288 * ((28.8f - shoot_statistics.mean) * (28.8f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_289 * ((28.9f - shoot_statistics.mean) * (28.9f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_290 * ((29.0f - shoot_statistics.mean) * (29.0f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_291 * ((29.1f - shoot_statistics.mean) * (29.1f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_292 * ((29.2f - shoot_statistics.mean) * (29.2f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_293 * ((29.3f - shoot_statistics.mean) * (29.3f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_294 * ((29.4f - shoot_statistics.mean) * (29.4f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_295 * ((29.5f - shoot_statistics.mean) * (29.5f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_296 * ((29.6f - shoot_statistics.mean) * (29.6f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_297 * ((29.7f - shoot_statistics.mean) * (29.7f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_298 * ((29.8f - shoot_statistics.mean) * (29.8f - shoot_statistics.mean))) +
		 (shoot_statistics.speed_299 * ((29.9f - shoot_statistics.mean) * (29.9f - shoot_statistics.mean))) ;

		shoot_statistics.variance = sum_of_squares / (float)shoot_statistics.shoot_num;
	}
}
#endif
