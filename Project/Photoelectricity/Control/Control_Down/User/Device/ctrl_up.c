#include "ctrl_up.h"

#include "my_math.h"

static uint8_t send_data[8] = {0}; // 发送数据缓冲区

Ctrl_t ctrl =
{
    .Init = Ctrl_Init,       // 初始化函数指针
};

void Ctrl_Init(Ctrl_t *ctrl)
{
    // 初始化函数指针
    ctrl->Work = Ctrl_Work;
    ctrl->Update = Ctrl_Update;
    ctrl->Heartbeat = Ctrl_Heartbeat;

    // 初始化云台接收数据
    ctrl->status = CTRL_OFFLINE; // 初始状态为离线
    ctrl->offline_cnt = 0; // 离线计数
    ctrl->offline_cnt_max = 100; // 最大离线计数
}

// can 发送给云台
// CAN_Send_Data(CAN_HandleTypeDef *hcan, uint32_t stdId, uint8_t *dat);
void Ctrl_Work(Ctrl_t *ctrl)
{
    memcpy(send_data, &ctrl->down_info, sizeof(ctrl->down_info)); // 复制底盘发送数据到发送缓冲区
//    CAN_Send_Data(&hcan1, 0x200, send_data); // 发送数据到云台
}

void Ctrl_Update(Ctrl_t *ctrl, uint8_t *buff)
{
    // 更新云台接收数据
    if (buff != NULL)   memcpy(&ctrl->up_info, buff, sizeof(ctrl->up_info));
		
		ctrl->x_pos = int16_to_float(ctrl->up_info.front, +32000, -32000, 4.f, 0.f);
		ctrl->y_pos = int16_to_float(ctrl->up_info.right, +32000, -32000,	0.f, -4.f);
		ctrl->a_yaw = int16_to_float(ctrl->up_info.cycle, +32000, -32000, 2.f*PI, 0.f);
		

    ctrl->offline_cnt = 0; // 重置离线计数
}

void Ctrl_Heartbeat(Ctrl_t *ctrl)
{
	ctrl->offline_cnt++;

	// 云台心跳检测
	if(ctrl->offline_cnt > ctrl->offline_cnt_max)
	{
		ctrl->offline_cnt = ctrl->offline_cnt_max;
		ctrl->status = CTRL_OFFLINE;
	}
	else if(ctrl->status == CTRL_OFFLINE)
			ctrl->status = CTRL_ONLINE;
}
