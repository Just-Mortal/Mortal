#include "ctrl_down.h"

static uint8_t send_data[8] = {0}; // 发送数据缓冲区
extern CAN_HandleTypeDef hcan1;

Ctrl_t ctrl =
{
    .Init = Ctrl_Init,       // 初始化函数指针
};

void Ctrl_Init(Ctrl_t *ctrl)
{
    // 初始化函数指针
    ctrl->Work			= Ctrl_Work;
    ctrl->Update		= Ctrl_Update;
    ctrl->Heartbeat = Ctrl_Heartbeat;

    // 初始化云台接收数据
    ctrl->status = CTRL_OFFLINE; // 初始状态为离线
    ctrl->offline_cnt = 0; // 离线计数
    ctrl->offline_cnt_max = 100; // 最大离线计数
}

// can 发送给底盘
void Ctrl_Work(Ctrl_t *ctrl)
{
		ctrl->up_info.x_pos = float_to_int16(ctrl->x_pos, 4.f, 	0.f, +32000, -32000);
		ctrl->up_info.y_pos = float_to_int16(ctrl->y_pos, 0.f, -4.f, +32000, -32000);
		ctrl->up_info.yaw_a = float_to_int16(ctrl->a_yaw, 2.f*PI, 0.f, +32000, -32000);

    memcpy(send_data, &ctrl->up_info, sizeof(ctrl->up_info));
    CAN_Send_Data(&hcan1, 0x111, send_data);
}

void Ctrl_Update(Ctrl_t *ctrl, uint8_t *buff)
{
    // 更新云台接收数据
    if (buff != NULL)   memcpy(&ctrl->up_info, buff, sizeof(ctrl->up_info));

    ctrl->offline_cnt = 0; // 重置离线计数
}

void Ctrl_Heartbeat(Ctrl_t *ctrl)
{
    // 云台心跳检测
    ctrl->offline_cnt++;

    if(ctrl->offline_cnt > ctrl->offline_cnt_max)
		{
			ctrl->offline_cnt = ctrl->offline_cnt_max;
			ctrl->status = CTRL_OFFLINE;
		}
    else if(ctrl->status == CTRL_OFFLINE)
        ctrl->status = CTRL_ONLINE; // 在最大离线计数内，设置为在线状态
}
