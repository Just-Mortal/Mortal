#include "vision.h"

/* Private function prototypes -----------------------------------------------*/
void vision_init(Vision_t *vis_sen);
void vision_heartbeat(Vision_t *vis_sen);
void vision_update(Vision_t *vis_sen, uint8_t *rxBuf);
void vision_check(Vision_t *vis_sen);	

uint8_t Vision_txBuf[20];
uint16_t x_mid_pixel = 960, y_mid_pixel = 580;

/* Private variables ---------------------------------------------------------*/

vision_rx_info_t vision_rx_info;

Vision_tx_info_t Vision_tx_info;

vision_info_t 	vision_info = {
	.tx_info = &Vision_tx_info,
	.rx_info = &vision_rx_info,

	.offline_max_cnt = 1000,
};

/* Exported variables --------------------------------------------------------*/
Vision_t vision = {
	.work_state = DEV_OFFLINE,
	.info 			= &vision_info,
	
	.init 			= vision_init,
	.update 		= vision_update,
	.check 			= vision_check,
	.heartbeat	= vision_heartbeat,
};

/* Exported variables ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
void vision_init(Vision_t *vis_sen)
{
	vision_rx_info_t *rx_info = vis_sen->info->rx_info;
	
	vis_sen->info->x_diff = 0;
	vis_sen->info->y_diff = 0;

	rx_info->find_target = 0;
	rx_info->x_pixel = 0;
	rx_info->y_pixel = 0;
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
		if(vis_sen->work_state == DEV_OFFLINE)
			vis_sen->work_state = DEV_ONLINE;
	}
}

bool vision_send_data(void)
{
	Vision_tx_info.head = 611;

	memcpy(Vision_txBuf, &Vision_tx_info, sizeof(Vision_tx_info_t));

	if(CDC_Transmit_FS(Vision_txBuf, sizeof(Vision_tx_info_t)) == 0U)
			return true;
	
	return false;
}

/*-------- protocol ���� begin --------*/
void vision_update(Vision_t *vis_sen, uint8_t *rxBuf)
{
	memcpy(&vision_rx_info, rxBuf, sizeof(vision_rx_info_t));
	
	return;
}

void vision_check(Vision_t *vis_sen)
{
	vis_sen->info->offline_cnt = 0;		// ���߼�������
	
	vis_sen->info->x_diff = vis_sen->info->rx_info->x_pixel - x_mid_pixel;
	vis_sen->info->y_diff = vis_sen->info->rx_info->y_pixel - y_mid_pixel;
	
}
/*-------- protocol ���� end --------*/
