#include "remote_ctrl.h"
#include "main.h"

remote_ctrl_t remote_ctrl;

// 引脚定义 (通过初始化函数设置)
static GPIO_TypeDef* CLK_PORT;
static uint16_t CLK_PIN;
static GPIO_TypeDef* CS_PORT;
static uint16_t CS_PIN;
static GPIO_TypeDef* CMD_PORT;
static uint16_t CMD_PIN;
static GPIO_TypeDef* DAT_PORT;
static uint16_t DAT_PIN;

const		uint8_t PS2_cmnd[9] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static	uint8_t PS2_data[9] = {0};

// 宏定义简化操作
#define CLK_H()     HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET)
#define CLK_L()     HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET)
#define CS_H()      HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)
#define CS_L()      HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)
#define CMD_H()     HAL_GPIO_WritePin(CMD_PORT, CMD_PIN, GPIO_PIN_SET)
#define CMD_L()     HAL_GPIO_WritePin(CMD_PORT, CMD_PIN, GPIO_PIN_RESET)
#define DAT_READ()  HAL_GPIO_ReadPin(DAT_PORT, DAT_PIN)

void AX_PS2_Init(GPIO_TypeDef* CLK_Port,	uint16_t CLK_Pin,
                 GPIO_TypeDef* CS_Port,		uint16_t CS_Pin,
                 GPIO_TypeDef* CMD_Port,	uint16_t CMD_Pin,
                 GPIO_TypeDef* DAT_Port,	uint16_t DAT_Pin)
{
    CLK_PORT = CLK_Port;	CLK_PIN = CLK_Pin;
    CS_PORT = CS_Port;		CS_PIN = CS_Pin;
    CMD_PORT = CMD_Port;	CMD_PIN = CMD_Pin;
    DAT_PORT = DAT_Port;	DAT_PIN = DAT_Pin;
    
    // 初始化默认状态
    CS_H();     // CS高电平
    CLK_H();    // CLK高电平
    CMD_H();    // CMD高电平
}

static void PS2_Delayus(uint16_t us)
{
    uint32_t ticks = us * (SystemCoreClock / 1000000) / 5;
    volatile uint32_t i;
    for(i = 0; i < ticks; i++);
}

static uint8_t PS2_ReadWriteData(uint8_t data)
{
    uint8_t ref = 0x01;
    uint8_t res = 0;
    
    for(; ref != 0; ref <<= 1)
    {
        CLK_L();
        
        if(data & ref)	CMD_H();
				else						CMD_L();
        
        PS2_Delayus(5);
        CLK_H();
        
        if(DAT_READ() == GPIO_PIN_SET)
					res |= ref;
        
        PS2_Delayus(5);
    }
		
    CMD_H();
    
    return res;
}

void AX_PS2_ScanKey(remote_ctrl_t *controller)
{
    CS_L();
    PS2_Delayus(1);

    for(uint8_t i = 0; i < 9; i++)
    {
        PS2_data[i] = PS2_ReadWriteData(PS2_cmnd[i]);
        PS2_Delayus(16);
    }
    
    CS_H();
    
    controller->info.mode = PS2_data[1];
    controller->info.btn.btn_raw = ((uint16_t)PS2_data[4] << 8) | PS2_data[3];
    controller->info.RJoy_LR = PS2_data[5];
    controller->info.RJoy_UD = PS2_data[6];
    controller->info.LJoy_LR = PS2_data[7];
    controller->info.LJoy_UD = PS2_data[8];
		
		// raw -> need
		if(controller->info.mode == 65)
			controller->mode = KEY_MODE;
		else if(controller->info.mode == 115)
			controller->mode = ADC_MODE;
		else
			controller->mode = ERR_MODE;
			
		controller->info.SW_R_LR = controller->info.RJoy_LR - 128;
    controller->info.SW_R_UD = controller->info.RJoy_UD - 127;
    controller->info.SW_L_LR = controller->info.LJoy_LR - 128;
    controller->info.SW_L_UD = controller->info.LJoy_UD - 127;
}
