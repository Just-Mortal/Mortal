#include "vision.h"

/* Private function prototypes -----------------------------------------------*/
void vision_init(Vision_t *vis_sen);
void vision_heartbeat(Vision_t *vis_sen);
void vision_update(Vision_t *vis_sen, uint8_t *rxBuf);
void vision_check(Vision_t *vis_sen);	

/* Private variables ---------------------------------------------------------*/
drv_uart_t	vision_driver = {
	.id = DRV_UART3,
};

vision_rx_info_t vision_rx_info;

Vision_tx_info_t Vision_tx_info = {
	.SOF 		 = 0xA5,
	.blood_0 = 0,
	.blood_1 = 100,
	.blood_2 = 100,
	.blood_3 = 100,
	.blood_4 = 100,
	.blood_5 = 100,
	.blood_6 = 100,
	.blood_7 = 100,
	.blood_8 = 100,
	.size_3  = 0,
	.size_4  = 0,
	.size_5  = 0,
};

vision_info_t 	vision_info = {
	.tx_info = &Vision_tx_info,
	.rx_info = &vision_rx_info,

	.offline_max_cnt = 1000,
};

/* Exported variables --------------------------------------------------------*/
Vision_t vision = {
	.work_state = DEV_OFFLINE,
	.info 			= &vision_info,
	.driver 		= &vision_driver,
	
	.init 			= vision_init,
	.update 		= vision_update,
	.check 			= vision_check,
	.heartbeat	= vision_heartbeat,
};

uint8_t Vision_txBuf[100];
uint32_t t1, t2, tmp;
//uint32_t test_t1, test_tmp;
//uint8_t		test_flag;
uint32_t rx_cnt = 0, rxr_cnt = 0;

/* Exported variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

/* Exported functions --------------------------------------------------------*/
void vision_init(Vision_t *vis_sen)
{
	Vision_tx_info_t *tx_info = vis_sen->info->tx_info;
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	
	vis_sen->info->rx_flag = 0;
	
	tx_info->pitch_angle 			= 0.0f; 	// data f_1
	tx_info->yaw_angle 				= 0.0f;   // data f_2
	tx_info->mode 						= AIM_OFF;// data u8_1
	tx_info->my_color 				= 0;      // data u8_3
	tx_info->is_change_target = 0;			// data u8_4
	
	rx_info->pitch_angle 			= 0.0f; 	// data f_1
	rx_info->yaw_angle 				= 0.0f;   // data f_2
	rx_info->mode 						= AIM_OFF;// data u8_1
	rx_info->is_find_target 	= 0; 			// data u8_2
	rx_info->is_find_buff 		= 0;	 		// data u8_3
	rx_info->is_hit_enable 		= 0;  		// data u8_4
}

void vision_heartbeat(Vision_t *vis_sen)
{
	vision_info_t *vis_info = vis_sen->info;

	vis_info->offline_cnt++;
	
	if(vis_info->offline_cnt > vis_info->offline_max_cnt)
	{
		vis_info->offline_cnt = vis_info->offline_max_cnt;
		vis_sen->work_state 	= DEV_OFFLINE;
	}
	else
	{
		/* 离线->在线 */
		if(vis_sen->work_state == DEV_OFFLINE)
		{
			vis_sen->work_state = DEV_ONLINE;
		}
	}
}

bool vision_send_data(void)
{
	memcpy(Vision_txBuf, &Vision_tx_info, sizeof(Vision_tx_info_t));

	Append_CRC8_Check_Sum(Vision_txBuf, 3);
	Vision_tx_info.CRC8 = Vision_txBuf[2];
	Append_CRC16_Check_Sum(Vision_txBuf, sizeof(Vision_tx_info_t));
	Vision_tx_info.CRC16 = (uint16_t)((Vision_txBuf[sizeof(Vision_tx_info_t)-2] << 8)
                                   | Vision_txBuf[sizeof(Vision_tx_info_t)-1]);

	if(HAL_UART_Transmit_DMA(&huart3,Vision_txBuf,sizeof(Vision_tx_info_t)) == HAL_OK)
	{
			return true;
	}
	
	return false;
}

/*-------- protocol 函数 begin --------*/
void vision_update(Vision_t *vis_sen, uint8_t *rxBuf)
{
	// 接收计数
	rx_cnt ++;

	// 接收标志位清零
	vis_sen->info->rx_flag = 0;

	if(rxBuf[0] == 0xA5)
	{
		if(Verify_CRC8_Check_Sum(rxBuf, 3) == true)
		{
			if(Verify_CRC16_Check_Sum(rxBuf, sizeof(vision_rx_info_t)) == true)
			{
				memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
				
				// 接收成功
				vis_sen->info->rx_flag = 1;
				
				// 接收率
				rxr_cnt++;

				// 接收时间间隔
				t1 = t2;
				t2 = micros();		// us
				tmp = t2 - t1;
				
//				if(vis_sen->info->tx_info->test_j == 1)
//				{
//					test_t1 = micros();
//					vis_sen->info->tx_info->test_j = 0;
//				}	
//				
//				if(vis_sen->info->rx_info->test_i == 1)
//				{
//					if(test_flag == 0)
//						test_tmp = micros() - test_t1;
//					else
//						test_flag = 1;
//				}

				return;
			}
		}
	}
}

void vision_check(Vision_t *vis_sen)
{
	vision_rx_info_t* rx_info = vis_sen->info->rx_info;
	vision_info_t* info 			= vis_sen->info;
	
	info->offline_cnt = 0;		// 在线计数清零
	
	if (info->rx_flag == 1)		// 接收成功
	{
		memcpy(&info->mode, &rx_info->mode, 1);
		memcpy(&info->target_pitch_angle, (void*)&rx_info->pitch_angle, 4);
		memcpy(&info->target_yaw_angle, (void*)&rx_info->yaw_angle, 4);
		memcpy(&info->is_find_target, &rx_info->is_find_target, 1);
		memcpy(&info->is_find_buff, &rx_info->is_find_buff, 1);
		memcpy(&info->is_hit_enable, &rx_info->is_hit_enable, 1);
//		memcpy(&info->is_spin, &rx_info->is_spin, 1);
		memcpy(&info->shoot_type, &rx_info->shoot_type, 1);
//		memcpy(&info->armor_num, &rx_info->armor_num, 1);
//		memcpy(&info->armor_id, &rx_info->armor_id, 1);
	}
}
/*-------- protocol 函数 end --------*/
