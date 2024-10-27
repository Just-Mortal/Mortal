#ifndef __RC_SENSOR_H
#define __RC_SENSOR_H

/* Includes ------------------------------------------------------------------*/
#include "rp_config.h"

#include "haltick_drv.h"
/*------------------------- Exported macro -----------------------------------*/
#define KEY_Z_TIME				200.f			//ms
#define KEY_X_TIME				200.f			//ms
#define KEY_C_TIME				200.f			//ms

#define KEY_B_TIME				1000.f		//ms
#define KEY_CTRL_TIME			2000.f		//ms
#define KEY_MOVE_TIME			200.f			//ms	// WASD
#define KEY_SHIFT_TIME		100.f			//ms
#define KEY_MOUSE_L_TIME	300.f			//ms

#define REMOTE_SMOOTH_TIMES 10			//鼠标平滑滤波次数

/* Exported types ------------------------------------------------------------*/
typedef enum {
	RELEASE	= 0,				// 松开
  PRESS   = 1					// 按下
}button_state_t;

typedef enum {
	BTN_NONE_FLIP 		= 0,	// 无电平翻转
	PRESS_TO_RELEASE 	= 1,	// 按下->松开
	RELEASE_TO_PRESS 	= 2		// 松开->按下
}button_flip_t;

typedef struct {
	button_state_t  state;      			// 当前状态
	button_flip_t   flip;       			// 翻转状态		跳变状态
	uint32_t        update_time;			// 更新时间(HALTICK: 1ms为单位)
	uint32_t        hold_time;  			// 保持时间(HALTICK: 1ms为单位)
	
	uint32_t				press_time_max;		// 按下最大时常
	uint32_t 				press_time;				// 按下时长

	bool						LONG_PRESS_FLAG; 	// 长按标志
} button_t;

typedef enum {
	SW_UP 	= 1,
	SW_MID 	= 3,
	SW_DOWN = 2
}switch_state_t;

typedef enum {
	SW_NONE_FLIP 		= 0,
	
	SW_UP_TO_MID 		= 1,  
	SW_MID_TO_DOWN 	= 2,
	SW_MID_TO_UP 		= 3,
	SW_DOWN_TO_MID 	= 4
}switch_flip_t;

typedef struct {
	switch_state_t  state;      // 当前状态
	switch_flip_t   flip;       // 翻转状态
	uint32_t        update_time;// 更新时间(HALTICK: 1ms为单位)
	uint32_t        hold_time;  // 保持时间(HALTICK: 1ms为单位)
}switch_t;

typedef struct {
	int16_t	val;
	
	int8_t 	thumbwheel_status;
	int8_t 	thumbwheel_filp;
	
}stick_t;

typedef struct {
	int16_t     val;
}mouse_speed_t;

typedef struct rc_info_struct {
	switch_t        SW1;
	switch_t        SW2;
	stick_t         CH0;
	stick_t         CH1;
	stick_t         CH2;
	stick_t         CH3;
	stick_t         THUMBWHEEL;
	button_t        W;
	button_t        S;
	button_t        A;
	button_t        D;
	button_t        SHIFT;
	button_t        CTRL;
	button_t        Q;
	button_t        E;
	button_t        R;
	button_t        F;
	button_t        G;
	button_t        Z;
	button_t        X;
	button_t        C;
	button_t        V;
	button_t        B;
	button_t        MOUSE_L;
	button_t        MOUSE_R;
	mouse_speed_t   MOUSE_VX;
	mouse_speed_t   MOUSE_VY;
	mouse_speed_t   MOUSE_VZ;
}rc_info_t;

/* Exported types ------------------------------------------------------------*/
typedef struct rc_sensor_info_struct {
	int16_t 	ch0;
	int16_t 	ch1;
	int16_t 	ch2;
	int16_t 	ch3;
	uint8_t  	s1;
	uint8_t  	s2;
	
	int16_t		mouse_vx;
	int16_t 	mouse_vy;
	int16_t 	mouse_vz;
	uint8_t 	mouse_btn_l;
	uint8_t 	mouse_btn_r;
	uint16_t	key_v;

	int16_t 	thumbwheel;
	
	int16_t		offline_cnt;
	int16_t		offline_max_cnt;
} rc_sensor_info_t;

typedef struct rc_sensor_struct {
	rc_sensor_info_t*	info;
	drv_uart_t*				driver;

	dev_work_state_t	work_state;
	
	void	(*init)(struct rc_sensor_struct *self);
	void	(*update)(struct rc_sensor_struct *self, uint8_t *rxBuf);
	void	(*check)(struct rc_sensor_struct *self);	
	void	(*heartbeat)(struct rc_sensor_struct *self);

	bool	(*if_channel_reset)(struct rc_sensor_struct *self);
	void	(*reset)(struct rc_sensor_struct *self);
	
}rc_sensor_t;

typedef struct rc_struct {
    rc_sensor_t     *dev;
    rc_info_t       *info;
	
    void            (*init)(void);
    void            (*update)(void);
    void            (*reset)(void);
    button_flip_t   (*update_button)(button_t *btn, button_state_t new_btn_state);
    switch_flip_t   (*update_switch)(switch_t *sw, switch_state_t new_sw_state);
    void            (*update_stick)(stick_t *stick, int16_t val);
    void            (*update_mouse)(mouse_speed_t *mouse_speed, int16_t val);
    void            (*reset_button)(button_t *btn, button_state_t new_btn_state);
    void            (*reset_switch)(switch_t *sw, switch_state_t new_sw_state);
    void            (*reset_stick)(stick_t *stick, int16_t val);
    void            (*reset_mouse)(mouse_speed_t *mouse_speed, int16_t val);    
} rc_t;

extern rc_t         rc;
extern rc_sensor_t  rc_sensor;

/* Exported macro ------------------------------------------------------------*/
/* ----------------------- RC Channel Definition------------------------------*/
#define    RC_CH_VALUE_MIN       ((uint16_t)364 )
#define    RC_CH_VALUE_OFFSET    ((uint16_t)1024)
#define	   RC_CH_VALUE_MAX       ((uint16_t)1684)
#define	   RC_CH_VALUE_SIDE_WIDTH	((RC_CH_VALUE_MAX-RC_CH_VALUE_MIN)/2)

/* ----------------------- RC Switch Definition-------------------------------*/
#define    RC_SW_UP              ((uint16_t)1)
#define    RC_SW_MID             ((uint16_t)3)
#define    RC_SW_DOWN            ((uint16_t)2)

/* ----------------------- PC Key Definition-------------------------------- */
#define    KEY_PRESSED_OFFSET_W        ((uint16_t)0x01<<0)
#define    KEY_PRESSED_OFFSET_S        ((uint16_t)0x01<<1)
#define    KEY_PRESSED_OFFSET_A        ((uint16_t)0x01<<2)
#define    KEY_PRESSED_OFFSET_D        ((uint16_t)0x01<<3)
#define    KEY_PRESSED_OFFSET_SHIFT    ((uint16_t)0x01<<4)
#define    KEY_PRESSED_OFFSET_CTRL     ((uint16_t)0x01<<5)
#define    KEY_PRESSED_OFFSET_Q        ((uint16_t)0x01<<6)
#define    KEY_PRESSED_OFFSET_E        ((uint16_t)0x01<<7)
#define    KEY_PRESSED_OFFSET_R        ((uint16_t)0x01<<8)
#define    KEY_PRESSED_OFFSET_F        ((uint16_t)0x01<<9)
#define    KEY_PRESSED_OFFSET_G        ((uint16_t)0x01<<10)
#define    KEY_PRESSED_OFFSET_Z        ((uint16_t)0x01<<11)
#define    KEY_PRESSED_OFFSET_X        ((uint16_t)0x01<<12)
#define    KEY_PRESSED_OFFSET_C        ((uint16_t)0x01<<13)
#define    KEY_PRESSED_OFFSET_V        ((uint16_t)0x01<<14)
#define    KEY_PRESSED_OFFSET_B        ((uint16_t)0x01<<15)

/* ----------------------- Function Definition-------------------------------- */
/* 遥控摇杆通道偏移值 */
#define		RC_SW1_VALUE						(rc_sensor_info.s1)
#define		RC_SW2_VALUE						(rc_sensor_info.s2)
#define		RC_LEFT_CH_LR_VALUE			(rc_sensor_info.ch2)
#define		RC_LEFT_CH_UD_VALUE			(rc_sensor_info.ch3)
#define		RC_RIGH_CH_LR_VALUE			(rc_sensor_info.ch0)
#define		RC_RIGH_CH_UD_VALUE			(rc_sensor_info.ch1)
#define		RC_THUMB_WHEEL_VALUE		(rc_sensor_info.thumbwheel)

/* 检测遥控器开关状态 */
#define    IF_RC_SW1_UP      (rc_sensor_info.s1 == RC_SW_UP)
#define    IF_RC_SW1_MID     (rc_sensor_info.s1 == RC_SW_MID)
#define    IF_RC_SW1_DOWN    (rc_sensor_info.s1 == RC_SW_DOWN)
#define    IF_RC_SW2_UP      (rc_sensor_info.s2 == RC_SW_UP)
#define    IF_RC_SW2_MID     (rc_sensor_info.s2 == RC_SW_MID)
#define    IF_RC_SW2_DOWN    (rc_sensor_info.s2 == RC_SW_DOWN)

/* 获取鼠标三轴的移动速度 */
#define    MOUSE_X_MOVE_SPEED    (rc_sensor_info.mouse_vx)
#define    MOUSE_Y_MOVE_SPEED    (rc_sensor_info.mouse_vy)
#define    MOUSE_Z_MOVE_SPEED    (rc_sensor_info.mouse_vz)

/* 检测鼠标按键状态 
   按下为1，没按下为0*/
#define    IF_MOUSE_PRESSED_LEFT    (rc_sensor_info.mouse_btn_l == 1)
#define    IF_MOUSE_PRESSED_RIGH    (rc_sensor_info.mouse_btn_r == 1)

/* 检测键盘按键状态 
   若对应按键被按下，则逻辑表达式的值为1，否则为0 */
#define    IF_KEY_PRESSED         (  rc_sensor_info.key_v  )
#define    IF_KEY_PRESSED_W       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_W)    != 0 )
#define    IF_KEY_PRESSED_S       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_S)    != 0 )
#define    IF_KEY_PRESSED_A       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_A)    != 0 )
#define    IF_KEY_PRESSED_D       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_D)    != 0 )
#define    IF_KEY_PRESSED_Q       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_Q)    != 0 )
#define    IF_KEY_PRESSED_E       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_E)    != 0 )
#define    IF_KEY_PRESSED_G       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_G)    != 0 )
#define    IF_KEY_PRESSED_X       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_X)    != 0 )
#define    IF_KEY_PRESSED_Z       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_Z)    != 0 )
#define    IF_KEY_PRESSED_C       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_C)    != 0 )
#define    IF_KEY_PRESSED_B       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_B)    != 0 )
#define    IF_KEY_PRESSED_V       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_V)    != 0 )
#define    IF_KEY_PRESSED_F       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_F)    != 0 )
#define    IF_KEY_PRESSED_R       ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_R)    != 0 )
#define    IF_KEY_PRESSED_CTRL    ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_CTRL) != 0 )
#define    IF_KEY_PRESSED_SHIFT   ( (rc_sensor_info.key_v & KEY_PRESSED_OFFSET_SHIFT) != 0 )
	
#endif
