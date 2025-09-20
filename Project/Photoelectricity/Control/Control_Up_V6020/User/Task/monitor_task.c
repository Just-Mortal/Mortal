/* Includes ------------------------------------------------------------------*/
#include "monitor_task.h"

// led 失联检测显示
// R	->	1
// B	->	2
// G	->	3	

/* Exported functions --------------------------------------------------------*/
void StartMonitorTask(void const * argument)
{
	char status_buf[9];
	static float last_battery_v;

	for(;;)
	{
		motor[GIMBAL_YAW].heartbeat(&motor[GIMBAL_YAW]);
		motor[GIMBAL_PCH].heartbeat(&motor[GIMBAL_PCH]);

		vision.heartbeat(&vision);
		
		ctrl.Heartbeat(&ctrl);
		
		// 第一行
		snprintf(status_buf, sizeof(status_buf), "State: %d", robot.info.strategy + 1);
		OLED_ShowString(0, 0, status_buf, 1);
		
		// 第二行
		if(vision.work_state == DEV_ONLINE)
			OLED_ShowString(0, 1, "Vis:111", 1);
		else
			OLED_ShowString(0, 1, "Vis:000", 1);
		
		if(vision.info->rx_info->find_target == 1)
			OLED_ShowString(8, 1, "Find:111", 1);
		else
			OLED_ShowString(8, 1, "Find:000 ", 1);
		
		// 第三行
		uint8_t yaw_state = motor[GIMBAL_YAW].state.work_state;
		uint8_t pth_state = motor[GIMBAL_PCH].state.work_state;
		const char* yaw_str = (yaw_state == M_ONLINE) ? "1" : "0";
		const char* pth_str = (pth_state == M_ONLINE) ? "1" : "0";
		snprintf(status_buf, sizeof(status_buf), "Y:%s P:%s", yaw_str, pth_str);
		OLED_ShowString(0, 2, status_buf, 1);
		
		// 低通滤波
		float battery_v = Read_Battery_Voltage() * 0.05f + last_battery_v * 0.95f;
		last_battery_v = battery_v;
		OLED_ShowString(8, 2, "BAT:", 1);
		OLED_ShowFloat(12, 2, battery_v, 2, 1, 1);
		
		// 第四行
		OLED_ShowString(0, 3, "The best wishes.", 1);
		
		OLED_Refresh_Gram();

		osDelay(1);
	}
}
