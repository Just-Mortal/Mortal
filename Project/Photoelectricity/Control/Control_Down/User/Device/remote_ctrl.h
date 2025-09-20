#ifndef __AX_PS2_H
#define __AX_PS2_H

#include "stm32f4xx_hal.h"

typedef enum
{
	KEY_MODE,
	ADC_MODE,
	
	ERR_MODE,
}remote_ctrl_e;


typedef struct {
    uint8_t mode;
    
    union
		{
        uint16_t btn_raw;
        struct
				{
            uint16_t select : 1;  // ѡ���
            uint16_t L3     : 1;  // ��ҡ�˰�ť
            uint16_t R3     : 1;  // ��ҡ�˰�ť
            uint16_t start  : 1;  // ��ʼ��
            uint16_t up     : 1;  // ������
            uint16_t right  : 1;  // ������
            uint16_t down   : 1;  // ������
            uint16_t left   : 1;  // ������
						
						uint16_t L2       : 1;  // ����2
            uint16_t R2       : 1;  // �Ҽ��2
            uint16_t L1       : 1;  // ����1
            uint16_t R1       : 1;  // �Ҽ��1
						uint16_t triangle : 1;  // ���Ǽ�
            uint16_t circle   : 1;  // ԲȦ��
            uint16_t cross    : 1;  // ���
            uint16_t square   : 1;  // �����
        }btn_bit;
    }btn;

    uint8_t RJoy_LR;  // ��ҡ������
    uint8_t RJoy_UD;  // ��ҡ������
    uint8_t LJoy_LR;  // ��ҡ������
    uint8_t LJoy_UD;  // ��ҡ������
		
		
		int16_t	SW_R_LR;
		int16_t	SW_R_UD;
		int16_t	SW_L_LR;
		int16_t	SW_L_UD;
} remote_ctrl_info_t;

typedef struct
{
	remote_ctrl_e				mode;
	remote_ctrl_info_t	info;
} remote_ctrl_t;

void AX_PS2_ScanKey(remote_ctrl_t *JoystickStruct);
void AX_PS2_Init(GPIO_TypeDef* CLK_Port, uint16_t CLK_Pin,
                 GPIO_TypeDef* CS_Port, uint16_t CS_Pin,
                 GPIO_TypeDef* CMD_Port, uint16_t CMD_Pin,
                 GPIO_TypeDef* DAT_Port, uint16_t DAT_Pin);
								 
extern remote_ctrl_t remote_ctrl;

#endif
