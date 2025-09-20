#ifndef __VISION_H
#define __VISION_H

/* Includes ------------------------------------------------------------------*/
#include "haltick_drv.h"

//#include "usbd_cdc_if.h"

#include "rp_config.h"
#include "string.h"

/* Exported types ------------------------------------------------------------*/
typedef __packed struct 
{
	uint16_t head;
	uint8_t	game_start;			//	���߱�־λ
	
}Vision_tx_info_t;  //Byte: 28

typedef __packed struct 
{
	uint8_t		find_target;	//	ʶ��Ŀ��
	uint8_t		winner_flag;	//	��ʤ������־λ
	
	uint16_t 	x_pixel;
	uint16_t	y_pixel;
	uint16_t	height;				//	���ظ߶�
	
}vision_rx_info_t;							// �����Ӿ�����

typedef struct
{
	Vision_tx_info_t  *tx_info;
	vision_rx_info_t  *rx_info;

	int16_t	x_diff;
	int16_t	y_diff;
	
	// -------- ʶ�� begin -------- //
	uint8_t	vision_ctrl_flag;			//	��̨�Ƿ����Ӿ�����
	
	uint16_t flag_cnt;
	uint16_t flag_max_cnt;
	// -------- ʶ�� end  	-------- //
	
	//-------- ͨ�� begin--------//
	int16_t	 offline_cnt;
	int16_t	 offline_max_cnt;
	//-------- ͨ�� end--------//

}vision_info_t;

typedef struct vision_struct {
	vision_info_t	    *info;
	
	dev_work_state_t	work_state;
	
	void				     (*init)(struct vision_struct *self);
	void				     (*update)(struct vision_struct *self, uint8_t *rxBuf);
	void				     (*check)(struct vision_struct *self);	
	void				     (*heartbeat)(struct vision_struct *self);
	
} Vision_t;

extern Vision_t vision;
extern bool vision_send_data(void);

#endif
