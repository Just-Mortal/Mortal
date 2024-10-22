/* Includes ------------------------------------------------------------------*/
#include "rc_sensor.h"

#include "rp_math.h"

/* Private function prototypes -----------------------------------------------*/
static void rc_init(void);
static void rc_update(void);
static void rc_reset(void);

static void rc_sensor_init(rc_sensor_t *rc_sen);
static void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf);
static void rc_sensor_check(rc_sensor_t *rc_sen);
static void rc_sensor_heartbeat(rc_sensor_t *rc_sen);
static bool rc_sensor_if_channel_reset(rc_sensor_t *rc_sen);
static void rc_sensor_reset(rc_sensor_t *rc_sen);

static button_flip_t rc_update_button(button_t *btn, button_state_t new_btn_state);
static switch_flip_t rc_update_switch(switch_t *sw, switch_state_t new_sw_state);
static void rc_update_stick(stick_t *stick, int16_t val);
static void rc_update_mouse(mouse_speed_t *mouse_speed, int16_t val);
static void rc_reset_button(button_t *btn, button_state_t new_btn_state);
static void rc_reset_switch(switch_t *sw, switch_state_t new_sw_state);
static void rc_reset_stick(stick_t *stick, int16_t val);
static void rc_reset_mouse(mouse_speed_t *mouse_speed, int16_t val);

static void rc_update_press_time(button_t *btn);
static void rc_update_thumbwheel(stick_t *stick);
static void rc_mouse_mean_filtering(void);

/* Exported variables --------------------------------------------------------*/
// 遥控器信息
rc_sensor_info_t rc_sensor_info = {
    .offline_max_cnt = 60,
};

// 遥控器传感器
rc_sensor_t rc_sensor = {
		.work_state = DEV_OFFLINE,
    .info 			= &rc_sensor_info,
		
    .init 			= rc_sensor_init,
    .update 		= rc_sensor_update,
    .check 			= rc_sensor_check,
    .heartbeat 	= rc_sensor_heartbeat,
    .reset 			= rc_sensor_reset,
		.if_channel_reset = rc_sensor_if_channel_reset
};

rc_info_t rc_info ={
		.Z.press_time_max 			= KEY_Z_TIME,
    .X.press_time_max 			= KEY_X_TIME,
    .C.press_time_max 			= KEY_C_TIME,
		.B.press_time_max				=	KEY_B_TIME,
		
    .A.press_time_max 			= KEY_MOVE_TIME,
    .S.press_time_max 			= KEY_MOVE_TIME,
    .D.press_time_max 			= KEY_MOVE_TIME,
    .W.press_time_max 			= KEY_MOVE_TIME,
		
		.CTRL.press_time_max 		= KEY_CTRL_TIME,
		.SHIFT.press_time_max 	= KEY_SHIFT_TIME,
		.MOUSE_L.press_time_max = KEY_MOUSE_L_TIME,
};

rc_t rc = {
    .dev 		= &rc_sensor,
    .info 	= &rc_info,
	
    .init 	= rc_init,
    .update = rc_update,
    .reset 	= rc_reset,
	
    .update_button 	= rc_update_button,		// 传递键盘按键值
    .update_switch 	= rc_update_switch,		// 传递遥控拨杆值
    .update_stick 	= rc_update_stick,		// 传递遥控摇杆值
    .update_mouse 	= rc_update_mouse,		// 传递鼠标变化值
    .reset_button 	= rc_reset_button,		// 初始化键盘按键
    .reset_switch 	= rc_reset_switch,		// 初始化遥控拨杆
    .reset_stick 		= rc_reset_stick,			// 初始化遥控摇杆
    .reset_mouse 		= rc_reset_mouse			// 初始化鼠标变化
};

/* private functions --------------------------------------------------------*/
static void rc_init(void)
{
	rc_sensor.init(&rc_sensor);
	
	rc.reset();
}

static void rc_update(void)
{
    rc_sensor_info_t *rc_sen = rc.dev->info;
    
    // update rc buttons
    rc.update_button(&rc_info.W, (rc_sen->key_v & KEY_PRESSED_OFFSET_W) ? PRESS : RELEASE);
    rc.update_button(&rc_info.S, (rc_sen->key_v & KEY_PRESSED_OFFSET_S) ? PRESS : RELEASE);
    rc.update_button(&rc_info.A, (rc_sen->key_v & KEY_PRESSED_OFFSET_A) ? PRESS : RELEASE);
    rc.update_button(&rc_info.D, (rc_sen->key_v & KEY_PRESSED_OFFSET_D) ? PRESS : RELEASE);
    rc.update_button(&rc_info.SHIFT, (rc_sen->key_v & KEY_PRESSED_OFFSET_SHIFT) ? PRESS : RELEASE);
    rc.update_button(&rc_info.CTRL, (rc_sen->key_v & KEY_PRESSED_OFFSET_CTRL) ? PRESS : RELEASE);
    rc.update_button(&rc_info.Q, (rc_sen->key_v & KEY_PRESSED_OFFSET_Q) ? PRESS : RELEASE);
    rc.update_button(&rc_info.E, (rc_sen->key_v & KEY_PRESSED_OFFSET_E) ? PRESS : RELEASE);
    rc.update_button(&rc_info.R, (rc_sen->key_v & KEY_PRESSED_OFFSET_R) ? PRESS : RELEASE);
    rc.update_button(&rc_info.F, (rc_sen->key_v & KEY_PRESSED_OFFSET_F) ? PRESS : RELEASE);
    rc.update_button(&rc_info.G, (rc_sen->key_v & KEY_PRESSED_OFFSET_G) ? PRESS : RELEASE);
    rc.update_button(&rc_info.Z, (rc_sen->key_v & KEY_PRESSED_OFFSET_Z) ? PRESS : RELEASE);
    rc.update_button(&rc_info.X, (rc_sen->key_v & KEY_PRESSED_OFFSET_X) ? PRESS : RELEASE);
    rc.update_button(&rc_info.C, (rc_sen->key_v & KEY_PRESSED_OFFSET_C) ? PRESS : RELEASE);
    rc.update_button(&rc_info.V, (rc_sen->key_v & KEY_PRESSED_OFFSET_V) ? PRESS : RELEASE);
    rc.update_button(&rc_info.B, (rc_sen->key_v & KEY_PRESSED_OFFSET_B) ? PRESS : RELEASE);
    rc.update_button(&rc_info.MOUSE_L, (rc_sen->mouse_btn_l) ? PRESS : RELEASE);
    rc.update_button(&rc_info.MOUSE_R, (rc_sen->mouse_btn_r) ? PRESS : RELEASE);
		
    // update rc switches
    rc.update_switch(&rc_info.SW1, (switch_state_t)rc_sen->s1);	// 实际就是一个赋值过程
    rc.update_switch(&rc_info.SW2, (switch_state_t)rc_sen->s2);
		
    // update rc sticks
    rc.update_stick(&rc_info.CH0, rc_sen->ch0);
    rc.update_stick(&rc_info.CH1, rc_sen->ch1);
    rc.update_stick(&rc_info.CH2, rc_sen->ch2);
    rc.update_stick(&rc_info.CH3, rc_sen->ch3);
		rc.update_stick(&rc_info.THUMBWHEEL, rc_sen->thumbwheel);
		rc_update_thumbwheel(&rc_info.THUMBWHEEL);
		
		
		// update rc mouses
		rc.update_mouse(&rc_info.MOUSE_VX, rc_sen->mouse_vx);
		rc.update_mouse(&rc_info.MOUSE_VY, rc_sen->mouse_vy);
		rc.update_mouse(&rc_info.MOUSE_VZ, rc_sen->mouse_vz);
		
		rc_update_press_time(&rc_info.W);
		rc_update_press_time(&rc_info.S);
		rc_update_press_time(&rc_info.A);
		rc_update_press_time(&rc_info.D);
		rc_update_press_time(&rc_info.B);
		rc_update_press_time(&rc_info.Z);
		rc_update_press_time(&rc_info.X);
		rc_update_press_time(&rc_info.C);
		rc_update_press_time(&rc_info.MOUSE_L);
		rc_update_press_time(&rc_info.SHIFT);
		rc_update_press_time(&rc_info.CTRL);
			
		// 对鼠标数据进行滤波
		rc_mouse_mean_filtering();
}

static void rc_reset(void)
{
    // reset rc buttons
    rc.reset_button(&rc_info.W, RELEASE);
    rc.reset_button(&rc_info.S, RELEASE);
    rc.reset_button(&rc_info.A, RELEASE);
    rc.reset_button(&rc_info.D, RELEASE);
    rc.reset_button(&rc_info.SHIFT, RELEASE);
    rc.reset_button(&rc_info.CTRL, RELEASE);
    rc.reset_button(&rc_info.Q, RELEASE);
    rc.reset_button(&rc_info.E, RELEASE);
    rc.reset_button(&rc_info.R, RELEASE);
    rc.reset_button(&rc_info.F, RELEASE);
    rc.reset_button(&rc_info.G, RELEASE);
    rc.reset_button(&rc_info.Z, RELEASE);
    rc.reset_button(&rc_info.X, RELEASE);
    rc.reset_button(&rc_info.C, RELEASE);
    rc.reset_button(&rc_info.V, RELEASE);
    rc.reset_button(&rc_info.B, RELEASE);
    rc.reset_button(&rc_info.MOUSE_L, RELEASE);
    rc.reset_button(&rc_info.MOUSE_R, RELEASE);
	
    // reset rc switches
    rc.reset_switch(&rc_info.SW1, SW_MID);
    rc.reset_switch(&rc_info.SW2, SW_MID);
		
    // reset rc sticks
    rc.reset_stick(&rc_info.CH0, 0);
    rc.reset_stick(&rc_info.CH1, 0);
    rc.reset_stick(&rc_info.CH2, 0);
    rc.reset_stick(&rc_info.CH3, 0);
		
    // reset rc mouses
    rc.reset_mouse(&rc_info.MOUSE_VX, 0);
    rc.reset_mouse(&rc_info.MOUSE_VY, 0);
    rc.reset_mouse(&rc_info.MOUSE_VZ, 0); 
}

/* Private functions ---------------------------------------------------------*/
static button_flip_t rc_update_button(button_t *btn, button_state_t new_btn_state)
{
    button_flip_t trig = BTN_NONE_FLIP;
    
    /* 按键状态跳变 */
    if(new_btn_state != btn->state)
		{
        btn->hold_time = 0;
        /* RELEASE -> PRESS */
        if(new_btn_state == PRESS)
		{
            trig = RELEASE_TO_PRESS;
        }
        /* PRESS -> RELEASE */
        else if(new_btn_state == RELEASE)
				{
            trig = PRESS_TO_RELEASE;
        }
    } 
    /* 按键状态保持 */
    else
		{
        btn->hold_time += millis() - btn->update_time;
    }
    
    btn->update_time 	= millis();
    btn->state 				= new_btn_state;
    btn->flip 				= trig;
    
    return trig;
}

static switch_flip_t  rc_update_switch(switch_t *sw, switch_state_t new_sw_state)
{
    switch_flip_t trig = SW_NONE_FLIP;
    
    /* 拨杆状态跳变 */
    if(new_sw_state != sw->state)
		{
        // 保持时间清零
        sw->hold_time = 0;
			
        if(sw->state == RC_SW_UP)
				{
            /* MID -> UP */
            if(new_sw_state == RC_SW_MID)
						{
                trig = SW_MID_TO_UP;
            }
        }
        else if(sw->state == RC_SW_MID)
				{
            /* UP -> MID */
            if(new_sw_state == RC_SW_UP)
						{
                trig = SW_UP_TO_MID;
            }
            /* DOWN -> MID */
            else if(new_sw_state == RC_SW_DOWN)
						{
                trig = SW_DOWN_TO_MID;
            }
        }
        else if(sw->state == RC_SW_DOWN)
				{
            /* MID -> DOWN */
            if(new_sw_state == RC_SW_MID)
						{
                trig = SW_MID_TO_DOWN;
            }
        }
    }
    /* 拨杆状态保持 */
    else
		{
        sw->hold_time += millis() - sw->update_time;
    }
    
    sw->update_time = millis();
    sw->state 			= new_sw_state;
    sw->flip 				= trig;
    
    return trig;
}

static void rc_update_stick(stick_t *stick, int16_t val)
{
    stick->val = val;
}

static void rc_update_thumbwheel(stick_t *stick)
{
	int8_t	thumbwheel_status, thumbwheel_filp;
	int16_t thumbwheel_val 					= stick->val;
	int8_t	thumbwheel_status_last	=	stick->thumbwheel_status;
	
	if(thumbwheel_val <= -600)
	{
		thumbwheel_status = SW_UP;
	}
	else if(thumbwheel_val >= 600)
	{
		thumbwheel_status = SW_DOWN;
	}
	else
	{
		thumbwheel_status = SW_MID;
	}
	
	if(thumbwheel_status == SW_UP && thumbwheel_status_last == SW_MID)
	{
		thumbwheel_filp = SW_MID_TO_UP;
	}
	else if(thumbwheel_status == SW_MID && thumbwheel_status_last == SW_UP)
	{
		thumbwheel_filp = SW_UP_TO_MID;
	}
	else if(thumbwheel_status == SW_MID && thumbwheel_status_last == SW_DOWN)
	{
		thumbwheel_filp = SW_DOWN_TO_MID;
	}
	else if(thumbwheel_status == SW_DOWN && thumbwheel_status_last == SW_MID)
	{
		thumbwheel_filp = SW_MID_TO_DOWN;
	}
	else if(thumbwheel_status == thumbwheel_status_last)
	{
		thumbwheel_filp = SW_NONE_FLIP;
	}
	
	stick->thumbwheel_status = thumbwheel_status;
	stick->thumbwheel_filp	 = thumbwheel_filp;
}

static void rc_update_mouse(mouse_speed_t *mouse_speed, int16_t val)
{
    mouse_speed->val = val;
}

static void rc_reset_button(button_t *btn, button_state_t rst_btn_state)
{
    btn->state = rst_btn_state;
    btn->flip = BTN_NONE_FLIP;
    btn->hold_time = 0;
    btn->update_time = millis();
}

static void rc_reset_switch(switch_t *sw, switch_state_t rst_sw_state)
{
    sw->state 			= rst_sw_state;
    sw->flip 				= SW_NONE_FLIP;
    sw->hold_time 	= 0;
    sw->update_time = millis();
}

static void rc_reset_stick(stick_t *stick, int16_t val)
{
    stick->val = val;
}

static void rc_reset_mouse(mouse_speed_t *mouse_speed, int16_t val)
{
    mouse_speed->val = val;
}

static void rc_mouse_mean_filtering(void)
{
	/* 鼠标速度均值滤波 */
	static int16_t mouse_x[REMOTE_SMOOTH_TIMES], mouse_y[REMOTE_SMOOTH_TIMES];
	static int16_t index = 0;
	
	index++;
	
	if(index == REMOTE_SMOOTH_TIMES)
	{
		index = 0;
	}
	
	rc.info->MOUSE_VX.val -= (float)mouse_x[index] / (float)REMOTE_SMOOTH_TIMES;
	rc.info->MOUSE_VY.val -= (float)mouse_y[index] / (float)REMOTE_SMOOTH_TIMES;
	
	mouse_x[index] = rc.info->MOUSE_VX.val;
	mouse_y[index] = rc.info->MOUSE_VY.val;
	
	rc.info->MOUSE_VX.val += (float)mouse_x[index] / (float)REMOTE_SMOOTH_TIMES;
	rc.info->MOUSE_VY.val += (float)mouse_y[index] / (float)REMOTE_SMOOTH_TIMES;
}

static void rc_update_press_time(button_t *btn)
{
	if(btn->state == PRESS)			// PRESS = 1
	{
		btn->press_time = btn->hold_time ;
	}
	else
	{
		btn->press_time = 0 ;
	}
	
	if(btn->press_time >= btn->press_time_max)
	{
		btn->press_time = btn->press_time_max;
		btn->LONG_PRESS_FLAG = true;
	}
	else
	{
		btn->LONG_PRESS_FLAG = false;
	}
}

/* Private functions ---------------------------------------------------------*/
/**
 *    @brief    遥控器数据解析协议
 */
static void rc_sensor_update(rc_sensor_t *rc_sen, uint8_t *rxBuf)
{
    rc_sensor_info_t *rc_info = rc_sen->info;
    
    rc_info->ch0 = (rxBuf[0] | rxBuf[1] << 8) & 0x07FF;
    rc_info->ch0 -= 1024;
    rc_info->ch1 = (rxBuf[1] >> 3 | rxBuf[2] << 5) & 0x07FF;
    rc_info->ch1 -= 1024;
    rc_info->ch2 = (rxBuf[2] >> 6 | rxBuf[3] << 2 | rxBuf[4] << 10) & 0x07FF;
    rc_info->ch2 -= 1024;
    rc_info->ch3 = (rxBuf[4] >> 1 | rxBuf[5] << 7) & 0x07FF;
    rc_info->ch3 -= 1024;

    rc_info->s1 = ((rxBuf[5] >> 4) & 0x000C) >> 2;
    rc_info->s2 = (rxBuf[5] >> 4) & 0x0003;    
    
    rc_info->mouse_vx 		= rxBuf[6]  | (rxBuf[7 ] << 8);
    rc_info->mouse_vy 		= rxBuf[8]  | (rxBuf[9 ] << 8);
    rc_info->mouse_vz 		= rxBuf[10] | (rxBuf[11] << 8);
    rc_info->mouse_btn_l 	= rxBuf[12];
    rc_info->mouse_btn_r 	= rxBuf[13];
    
    rc_info->key_v = rxBuf[14] | (rxBuf[15] << 8);    
    
    rc_info->thumbwheel = ((int16_t)rxBuf[16] | ((int16_t)rxBuf[17] << 8)) & 0x07ff;
    rc_info->thumbwheel -= 1024;
    
    rc_info->offline_cnt = 0;
	
}

static void rc_sensor_init(rc_sensor_t *rc_sen)
{
    rc_sen->info->offline_cnt = rc_sen->info->offline_max_cnt + 1;
    rc_sen->work_state = DEV_OFFLINE;
}

/**
 *  @brief  遥控器数据检查
 */
static void rc_sensor_check(rc_sensor_t *rc_sen)
{
    rc_sensor_info_t *rc_info = rc_sen->info;
    
    if(abs(rc_info->ch0) > 660 ||
       abs(rc_info->ch1) > 660 ||
       abs(rc_info->ch2) > 660 ||
       abs(rc_info->ch3) > 660)
    {
        rc_info->ch0 = 0;
        rc_info->ch1 = 0;
        rc_info->ch2 = 0;
        rc_info->ch3 = 0;        
        rc_info->s1 = RC_SW_MID;
        rc_info->s2 = RC_SW_MID;
        rc_info->thumbwheel = 0;
    }
}

/**
 *  @brief  遥控器心跳包
 */
static void rc_sensor_heartbeat(rc_sensor_t *rc_sen)
{
    rc_sensor_info_t *rc_info = rc_sen->info;

    rc_info->offline_cnt++;
    if(rc_info->offline_cnt > rc_info->offline_max_cnt)
		{
        rc_info->offline_cnt = rc_info->offline_max_cnt;
        rc_sen->work_state = DEV_OFFLINE;
    } 
    else
		{
        /* 离线->在线 */
        if(rc_sen->work_state == DEV_OFFLINE)
            rc_sen->work_state = DEV_ONLINE;
    }
}

/**
 *  @brief  遥控器判断通道是否归位
 */
static bool rc_sensor_if_channel_reset(rc_sensor_t *rc_sen)
{
    bool ifChannelReset = false;
    rc_sensor_info_t *rc_info = rc_sen->info;
    
    if(  (DeathZoom(rc_info->ch0, 0, 50) == 0) && 
         (DeathZoom(rc_info->ch1, 0, 50) == 0) && 
         (DeathZoom(rc_info->ch2, 0, 50) == 0) && 
         (DeathZoom(rc_info->ch3, 0, 50) == 0)   )    
    {
        ifChannelReset = true;
    }
    return ifChannelReset;
}

/**
 *  @brief  遥控器强制复位遥控值
 */
static void rc_sensor_reset(rc_sensor_t *rc_sen)
{
    rc_sensor_info_t *rc_info = rc_sen->info;
    
    // 通道值强行设置成中间值(不拨动摇杆的状态)
    rc_info->ch0 = 0;
    rc_info->ch1 = 0;
    rc_info->ch2 = 0;
    rc_info->ch3 = 0;
    // 左右开关选择强行设置成中间值状态
    rc_info->s1 = RC_SW_MID;
    rc_info->s2 = RC_SW_MID;
    // 鼠标
    rc_info->mouse_vx = 0;
    rc_info->mouse_vy = 0;
    rc_info->mouse_vz = 0;
    rc_info->mouse_btn_l = 0;
    rc_info->mouse_btn_r = 0;
    // 键盘
    rc_info->key_v = 0;
    // 左拨轮
    rc_info->thumbwheel = 0;    
}

/* Exported functions --------------------------------------------------------*/
