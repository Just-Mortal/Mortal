/**
 * @file			judge_def.h
 * @author		JSJ
 * @date			2024.03.11
 * @Version		1.6.1
 * @brief			
 * @update		修订命令码 0x0101、0x0201、0x0203、0x0209、0x020A、0x0303
							新增0x020D、0x020E、0x0308
							修改judge_data_length_t
							确保0x0101、0x0102、0x0104、0x0105、
							0x0201、0x0203、0x0209、0x020A的内容和手册相同
 */
 
#ifndef __JUDGE_DEF_H
#define __JUDGE_DEF_H

/* Includes ------------------------------------------------------------------*/
#include "type.h"

/*
potocal_frame:
frame_header (5-byte) cmd_id (2-byte) data (n-byte) frame_tail (2-byte，CRC16，整包校验)

frame_header:
SOF (1-byte)  data_length (2-byte)  seq (1-byte)  CRC8 (1-byte)
*/

/* ------------------------- 裁判系统数据定义 begin -------------------------- */
enum
{ 
	ID_game_state       						= 0x0001,	// 比赛状态数据，1Hz
	ID_game_result 	   							= 0x0002,	// 比赛结果数据，比赛结束触发发送
	ID_game_robot_HP       					= 0x0003,	// 机器人血量数据，3Hz
	ID_event_data  									= 0x0101,	// 场地事件数据，1Hz
	ID_supply_projectile_action   	= 0x0102,	// 补给站动作标识数据，补给站弹丸释放时触发发送
	ID_referee_warning					 		= 0x0104,	// 裁判警告数据，己方判罚/判负时触发发送，其余时间1Hz
	ID_dart_remaining_time					= 0x0105,	// 飞镖发射时间数据，1Hz
	ID_game_robot_state    					= 0x0201,	// 机器人性能体系数据，10Hz
	ID_power_heat_data    					= 0x0202,	// 实时底盘功率和枪口热量数据，10Hz
	ID_game_robot_pos        				= 0x0203,	// 机器人位置数据，1Hz
	ID_buff_musk										= 0x0204,	// 机器人增益数据，3Hz
	ID_aerial_robot_energy					= 0x0205,	// 空中支援时间数据，1Hz
	ID_robot_hurt										= 0x0206,	// 伤害状态数据，伤害发生后发送
	ID_shoot_data										= 0x0207,	// 实时射击数据，子弹发射后发送
	ID_projectile_allowance	  			= 0x0208,	// 允许发弹量，10Hz
	ID_rfid_status									= 0x0209,	// 机器人RFID状态，3Hz
	
	ID_dart_client_directive        = 0x020A,	// 飞镖机器人客户端指令书, 10Hz
	ID_ground_robot_position        = 0x020B,	// 地面机器人位置数据, 仅哨兵机器人主控，1Hz
	ID_radar_mark_data              = 0x020C,	// 雷达标记进度数据，仅雷达机器人主控， 1Hz
	ID_sentry_auto_info							= 0x020D, // 哨兵自主决策信息同步，仅哨兵机器人主控，1Hz
	ID_radar_auto_info							= 0x020E, // 雷达自主决策信息同步，仅雷达机器人主控，1Hz
	
	ID_robot_interactive_header_data			= 0x0301,	// 机器人交互数据，发送方触发发送，频率上限为 10Hz
	ID_controller_interactive_header_data = 0x0302,	// 自定义控制器与机器人交互数据，发送方触发发送，频率上限为 30Hz
	ID_map_interactive_header_data        = 0x0303,	// 选手端小地图交互数据，选手端触发发送
	ID_keyboard_information               = 0x0304,	// 键键鼠遥控数据，30Hz
  ID_smallmap                           = 0x0305,	// 选手端小地图接收雷达数据，频率上限为 10Hz
  ID_custom_client_data                 = 0x0306,	// 自定义控制器与选手端交互数据，发送方触发发送，30Hz
  ID_map_sentry_data                    = 0x0307,	// 选手端小地图接收哨兵数据， 仅云台手客户端接收，3Hz
	ID_map_robot_date											= 0x0308,	// 选手端小地图接收机器人数据，频率上限为 3Hz
};

//命令码枚举CMD_ID
// ---------- 裁判系统各个信息的长度（单位/字节）---------- /
enum judge_data_length_t {

	/* Std */
	LEN_FRAME_HEAD 	                 = 5,		// 帧头长度
	LEN_CMD_ID 		                   = 2,		// 命令码长度
	LEN_FRAME_TAIL 	                 = 2,		// 帧尾CRC16
	
	/* Ext */
	// 0x000x
	LEN_GAME_STATUS 				         = 11,	// 0x0001
	LEN_GAME_RESULT 				         = 1,		// 0x0002
	LEN_GAME_ROBOT_HP 			         = 32,	// 0x0003
	
	// 0x010x
	LEN_EVENT_DATA					         = 4,		// 0x0101
	LEN_SUPPLY_PROJECTILE_ACTION	   = 4,		// 0x0102
	LEN_REFEREE_WARNING				       = 3,		// 0x0104
	LEN_DART_REMAINING_TIME		     	 = 3,		// 0x0105
	
	// 0x020x
	LEN_GAME_ROBOT_STATUS			       = 13,	// 0x0201
	LEN_POWER_HEAT_DATA 			       = 16,	// 0x0202
	LEN_GAME_ROBOT_POS				       = 16,	// 0x0203
	LEN_BUFF_MASK		 				         = 6,		// 0x0204
	LEN_AERIAL_ROBOT_ENERGY 	     	 = 2,		// 0x0205
	LEN_ROBOT_HURT				         	 = 1,		// 0x0206
	LEN_SHOOT_DATA					         = 7,		// 0x0207
	LEN_BULLET_REMAINING	 		       = 6,		// 0x0208
	LEN_RFID_STATUS					         = 4,		// 0x0209
	LEN_DART_CLIENT_DIRECTIVE        = 6,		// 0x020A
	LEN_GROUND_ROBOT_POSITION        = 40,	// 0x020B
	LEN_RADAR_MARK_DATA				       = 6,		// 0x020C
	LED_SENTRY_AUTO_INFO						 = 4,		// 0x020D
	LED_RADAR_AUTO_INFO							 = 1,		// 0x020E

	// 0x030x
	LEN_ROBOT_INTERACTIVE_HEADER_DATA				 = 128,	// 0x0301
	LEN_CONTROLLER_INTERACTIVE_HEADER_DATA	 = 30,	// 0x0302
	LEN_MAP_INTERACTIVE_HEADERDATA           = 15,	// 0x0303
	LEN_KEYBOARD_INFORMATION                 = 12,	// 0x0304
	LEN_SMALLMAP														 = 10,	// 0x0305

	LEN_SENTRY_INFO                          = 1,
};

/*---------- 裁判系统信息内容排序 ----------*/
//交互信息
typedef __packed struct
{
	uint8_t find_enemy   :1;
	uint8_t find_hero    :1;
	uint8_t fine_infantry:2;	// 后面可以加发现几个步兵
	uint8_t stealed_tower:2;	// 被偷塔
	uint8_t meaningless  :2;	// 后面加功能
}ext_sentry_t;

typedef __packed struct
{
	uint8_t  sof;
	uint16_t data_length;
	uint8_t  seq;
	uint8_t  crc8;
} std_frame_header_t;				// LEN_FRAME_HEAD = 5

/* ID: 0x0001	Byte: 	11	比赛状态数据 */
typedef __packed struct 
{ 
	uint8_t game_type     : 4;		// 比赛类型
	uint8_t game_progress : 4;		// 比赛阶段
	uint16_t stage_remain_time;		// 当前阶段剩余时间(单位:s)
	uint64_t SyncTimeStamp;       // 机器人接收到该指令的精确 Unix 时间，当机载端收到有效的 NTP 服务器授时后生效
} ext_game_status_t; 						// LEN_GAME_STATUS   表2-2

/* ID: 0x0002	Byte:	1	比赛结果数据 */
typedef __packed struct
{
	uint8_t winner;
} ext_game_result_t; 						// LEN_GAME_RESULT   表2-3

/* ID: 0x0003	Byte:	32	机器人血量数据数据 */
typedef __packed struct 
{ 
	uint16_t red_1_robot_HP;	// 红1英雄机器人血量(未上场及罚下血量为0)
	uint16_t red_2_robot_HP;	// 红2工程机器人血量
	uint16_t red_3_robot_HP;	// 红3步兵机器人血量
	uint16_t red_4_robot_HP;	// 红4步兵机器人血量
	uint16_t red_5_robot_HP;	// 红5步兵机器人血量
	uint16_t red_7_robot_HP;	// 红7哨兵机器人血量
	uint16_t red_outpost_HP;	// 红方前哨站血量
	uint16_t red_base_HP;			// 红方基地血量
	uint16_t blue_1_robot_HP;	// 蓝1英雄机器人血量
	uint16_t blue_2_robot_HP;	// 蓝2工程机器人血量
	uint16_t blue_3_robot_HP;	// 蓝3步兵机器人血量
	uint16_t blue_4_robot_HP;	// 蓝4步兵机器人血量
	uint16_t blue_5_robot_HP;	// 蓝5步兵机器人血量
	uint16_t blue_7_robot_HP;	// 蓝7哨兵机器人血量
	uint16_t blue_outpost_HP;	// 蓝方前哨站血量
	uint16_t blue_base_HP;		// 蓝方基地血量	
} ext_game_robot_HP_t; 			// LEN_GAME_ROBOT_HP  表3-3

/* ID: 0x0101  Byte:  4    场地事件数据 */
typedef __packed struct 
{ 
	uint32_t event_data;		// 能量机关状态
													// 己方前哨战状态
													// 环形高地占领状态
													// 己方基地护盾状态
													// 己方补给站补血点占领状态
													
} ext_event_data_t; 			// LEN_EVENT_DATA   表3-6

/* ID: 0x0102  Byte:  4    补给站动作标识数据 */
typedef __packed struct
{
 uint8_t reserved;								// 保留
 uint8_t supply_robot_id;					// 补弹机器人ID
 uint8_t supply_projectile_step;	// 出弹口开闭状态
 uint8_t supply_projectile_num;		// 补弹数量
} ext_supply_projectile_action_t;	// LEN_SUPPLY_PROJECTILE_ACTION

/* ID: 0X0104  Byte:  2	   裁判警告信息 */		
typedef __packed struct
{
 uint8_t level;										// 己方最后一次受到判罚的等级
 uint8_t offending_robot_id;			// 己方最后一次受到判罚的违规机器人ID
 uint8_t count;										// 己方最后一次受到判罚的违规机器人对应判罚等级的违规次数
}ext_referee_warning_t;								// LEN_REFEREE_WARNING

/* ID: 0X0105  Byte:  1	   飞镖发射口倒计时 */
typedef __packed struct
{
	uint8_t		dart_remaining_time;	// 15s 倒计时
	uint16_t	dart_info;						// 最近一次己方飞镖击中的目标					0-1
																	// 对方最近被击中的目标累计被击中计数	2-4
																	// 飞镖此时选定的击打目标							5-6
																	// 保留																7-15
} ext_dart_remaining_time_t;			// LEN_DART_REMAINING_TIME

/* ID: 0X0201  Byte: 12    机器人状态数据 */
typedef __packed struct
{
	uint8_t robot_id;															// 本机器人 ID
	uint8_t robot_level;													// 机器人等级
	uint16_t current_HP; 													// 机器人当前血量
	uint16_t maximum_HP;													// 机器人血量上限
	uint16_t shooter_barrel_cooling_value;				// 机器人枪口热量每秒冷却值
	uint16_t shooter_barrel_heat_limit;						// 机器人枪口热量上限
	uint16_t chassis_power_limit; 								// 机器人底盘功率上限

	//主控电源输出情况
	uint8_t power_management_gimbal_output : 1;		// gimbal  口输出：0 为无输出，1 为 24V 输出
	uint8_t power_management_chassis_output : 1;	// chassis 口输出：0 为无输出，1 为 24V 输出
	uint8_t power_management_shooter_output : 1;	// shooter 口输出：0 为无输出，1 为 24V 输出
}ext_game_robot_status_t;												// LEN_GAME_ROBOT_STATUS  表3-10

/* ID: 0X0202  Byte: 16    实时功率热量数据 */
typedef __packed struct 
{ 
	uint16_t chassis_volt;   				// 底盘输出电压，单位：mV
	uint16_t chassis_current;				// 底盘输出电流，单位：mA
	float chassis_power;   					// 底盘瞬时功率，单位：W
	uint16_t chassis_power_buffer;	// 底盘功率缓冲，单位：60J焦耳缓冲能量(飞坡根据规则增加至250J)
	uint16_t shooter_id1_17mm_cooling_heat; //1 号 17mm 枪口热量
	uint16_t shooter_id2_17mm_cooling_heat; //2 号 17mm 枪口热量
	uint16_t shooter_id1_42mm_cooling_heat;	//42mm 枪口热量
} ext_power_heat_data_t; //LEN_POWER_HEAT_DATA   表3-11

/* ID: 0x0203  Byte: 16    机器人位置数据 */
typedef __packed struct
{
 float x;			// 本机器人位置 x 坐标
 float y;			// 本机器人位置 y 坐标
 float angle;	// 本机器人测速模块的朝向
} ext_game_robot_pos_t;

/* ID: 0x0204  Byte:  1    机器人增益数据 */
typedef __packed struct 
{ 
	uint8_t recovery_buff;
	uint8_t cooling_buff;
	uint8_t defence_buff;
	uint16_t attack_buff;
} ext_buff_t; //LEN_BUFF_MASK  表3-13

/* ID: 0x0205  Byte:  1    空中机器人能量状态数据 */
typedef __packed struct 
{ 
	uint8_t airforce_status;		// 空中机器人状态（0 为正在冷却，1 为冷却完毕，2 为空中支援期间）
	uint8_t time_remain;				// 此状态的剩余时间（单位为 s，向下取整，即冷却时间剩余 1.9s 时，此值为 1）
} ext_aerial_robot_energy_t;	// LEN_AERIAL_ROBOT_ENERGY

/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef __packed struct 
{ 
	uint8_t armor_id : 4; 	// 装甲伤害时代表装甲ID
	uint8_t hurt_type : 4; 	// 0x0装甲伤害 0x1模块掉线 0x2超射速 0x3超热量 0x4超功率 0x5撞击
} ext_robot_hurt_t; //LEN_ROBOT_HURT  表3-15

/* ID: 0x0207  Byte:  7    实时射击数据 */
typedef __packed struct 
{ 
	uint8_t bullet_type; 	// 子弹类型(1-17mm, 2-42mm)
	uint8_t shooter_id;   // 发射机构 ID（1：1 号 17mm 发射机构，2：2 号 17mm 发射机构，3：42mm 发射机构）
	uint8_t bullet_freq;  // 子弹射频(Hz)
	float bullet_speed;		// 子弹射速(m/s)
} ext_shoot_data_t; //LEN_SHOOT_DATA  表3-16

/* ID: 0x0208  Byte:  6    子弹剩余发射数数据 */
typedef __packed struct
{
	uint16_t bullet_remaining_num_17mm;//17mm 子弹剩余发射数目
	uint16_t bullet_remaining_num_42mm;//42mm 子弹剩余发射数目
	uint16_t coin_remaining_num;       //剩余金币数量
} ext_bullet_remaining_t; //LEN_BULLET_REMAINING   表3-17

/* ID: 0x0209  Byte:  4 	机器人RFID状态 */
typedef __packed struct
{
	uint32_t rfid_status;
} ext_rfid_status_t;				//LEN_RFID_STATUS

/*ID：0x020A   Byte：6  飞镖机器人客户端指令数据*/
typedef __packed struct{
	uint8_t		dart_launch_opening_status;	// 当前飞镖发射口的状态
	uint8_t		reserved;										// 保留位
	uint16_t	target_change_time;					// 切换击打目标时的比赛剩余时间
	uint16_t	latest_launch_cmd_time;			// 最后一次操作手确定发射指令时的比赛剩余时间
} ext_dart_client_cmd_t; 								// LEN_DART_CLIENT_DIRECTIVE

/*ID：0x020B   Byte：40  地面机器人位置数据*/
/*场地围挡在红方补给站附近的交点为坐标原点，沿场地长边向蓝方为 X 轴正方向，沿场地短边向红方停机坪为 Y 轴正方向*/
typedef __packed struct
{
 float hero_x;				//己方英雄机器人位置 x 轴坐标，单位：m
 float hero_y;				//己方英雄机器人位置 y 轴坐标
 float engineer_x;		//己方工程机器人位置 x 轴坐标
 float engineer_y;		//己方工程机器人位置 y 轴坐标
 float standard_3_x;	//己方 3 号步兵机器人位置 x 轴坐标
 float standard_3_y;	//己方 3 号步兵机器人位置 y 轴坐标
 float standard_4_x;	//己方 4 号步兵机器人位置 x 轴坐标
 float standard_4_y;	//己方 4 号步兵机器人位置 y 轴坐标
 float standard_5_x;	//己方 5 号步兵机器人位置 x 轴坐标
 float standard_5_y;	//己方 5 号步兵机器人位置 y 轴坐标
} ext_ground_robot_position_t;

/*ID：0x020C   Byte：6  雷达标记进度数据*/
typedef __packed struct
{
 uint8_t mark_hero_progress;				//对方英雄机器人被标记进度：0—120
 uint8_t mark_engineer_progress;		//对方工程机器人被标记进度：0—120
 uint8_t mark_standard_3_progress;	//对方 3 号步兵机器人被标记进度：0—120
 uint8_t mark_standard_4_progress;	//对方 4 号步兵机器人被标记进度：0—120
 uint8_t mark_standard_5_progress;	//对方 5 号步兵机器人被标记进度：0—120
 uint8_t mark_sentry_progress;			//对方哨兵机器人被标记进度：0—120
} ext_radar_mark_data_t;

/*******************************************************************************/
/*
	机器人 ID：
	1，英雄(红)；
	2，工程(红)；
	3/4/5，步兵(红)；
	6，空中(红)；
	7，哨兵(红)；
	9，雷达（红）
	101，英雄(蓝)；
	102，工程(蓝)；
	103/104/105，步兵(蓝)；
	106，空中(蓝)；
	107，哨兵(蓝)；
	109，雷达（蓝）

	客户端 ID： 
	0x0101 为英雄操作手客户端(红) ；
	0x0102 为工程操作手客户端( 红 )；
	0x0103/0x0104/0x0105 为步兵操作手客户端(红)；
	0x0106 为空中操作手客户端((红)； 

	0x0165，英雄操作手客户端(蓝)；
	0x0166，工程操作手客户端(蓝)；
	0x0167/0x0168/0x0169，步兵操作手客户端(蓝)；
	0x016A，空中操作手客户端(蓝)。 
*/
enum judge_robot_ID{
	hero_red       = 1,
	engineer_red   = 2,
	infantry3_red  = 3,
	infantry4_red  = 4,
	infantry5_red  = 5,
	plane_red      = 6,
	
	hero_blue      = 101,
	engineer_blue  = 102,
	infantry3_blue = 103,
	infantry4_blue = 104,
	infantry5_blue = 105,
	plane_blue     = 106,
};

typedef struct{
	uint16_t teammate_hero;
	uint16_t teammate_engineer;
	uint16_t teammate_infantry3;
	uint16_t teammate_infantry4;
	uint16_t teammate_infantry5;
	uint16_t teammate_plane;
	uint16_t teammate_sentry;
	
	uint16_t client_hero;
	uint16_t client_engineer;
	uint16_t client_infantry3;
	uint16_t client_infantry4;
	uint16_t client_infantry5;
	uint16_t client_plane;
} ext_interact_id_t;









/* 
	学生机器人间通信 cmd_id 0x0301，内容 data_ID:0x0200~0x02FF
	交互数据 机器人间通信：0x0301。
	发送频率：数据上下行合计带宽不超过 5000 Byte。 上下行发送频率分别不超过30Hz。
 * +------+------+-------------+------------------------------------+
 * | byte | size |    breif    |            note                    |
 * |offset|      |             |                                    |
 * +------+------+-------------+------------------------------------+
 * |  0   |  2   | 	 data_ID   | 0x0200~0x02FF,可以在这些 ID 段选取 |
 * |      |      |             | 具体ID含义由参赛队自定义							|
 * +------|------|-------------|------------------------------------|
 * |  2   |  2   | 	sender_ID  | 需要校验发送者的 ID 正确性					  |
 * +------|------|-------------|------------------------------------|
 * |  4   |  2   | receiver_ID | 需要校验接收者的 ID 正确性					  |
 * |      |      |             | 例如不能发送到敌对机器人的ID				  | 
 * +------|------|-------------|------------------------------------|
 * |  6   |  n   | 		Data     | n 需要小于 113 										  |
 * +------+------+-------------+------------------------------------+
*/
/******************************客户端交互数据**************************************/
#define INTERACT_DATA_LEN	113
typedef __packed struct //数据段内容格式
{ 
	uint16_t data_cmd_id;    
	uint16_t send_ID;    
	uint16_t receiver_ID; 
} ext_client_data_header_t; 
enum
{
	//0x200-0x02ff 	队伍自定义命令 格式  INTERACT_ID_XXXX
	INTERACT_ID_delete_graphic 			= 0x0100,	/*客户端删除图形*/
	INTERACT_ID_draw_one_graphic 		= 0x0101,	/*客户端绘制一个图形*/
	INTERACT_ID_draw_two_graphic 		= 0x0102,	/*客户端绘制2个图形*/
	INTERACT_ID_draw_five_graphic 	= 0x0103,	/*客户端绘制5个图形*/
	INTERACT_ID_draw_seven_graphic 	= 0x0104,	/*客户端绘制7个图形*/
	INTERACT_ID_draw_char_graphic 	= 0x0110,	/*客户端绘制字符图形*/
	INTERACT_ID_radar_sentry_ex			= 0x0200  /*雷达发给哨兵角度信息*/
};
typedef __packed struct 
{ 
	uint8_t data[INTERACT_DATA_LEN]; //数据段,n需要小于113
} robot_interactive_data_t;
//单位（字节）
enum
{
	LEN_INTERACT_delete_graphic     = 8,  //删除图层 2(数据内容ID)+2(发送者ID)+2（接收者ID）+2（数据内容）  
	LEN_INTERACT_draw_one_graphic   = 21, // 以上2+2+2+15
	LEN_INTERACT_draw_two_graphic   = 36, //6+15*2
	LEN_INTERACT_draw_five_graphic  = 81, //6+15*5
	LEN_INTERACT_draw_seven_graphic = 111,//6+15*7
	LEN_INTERACT_draw_char_graphic  = 51, //6+15+30（字符串内容）
};
/****************************雷达的数据段内容****************************/


typedef __packed struct
{
	float target_position_x;
	float target_position_y;
	float target_position_z;
	uint8_t commd_keyboard;
	uint16_t target_robot_ID;
} ext_robot_command_t;


//

typedef __packed struct
{
	uint16_t target_robot_ID;
	float target_position_x;
	float target_position_y;
	float toward_Angle;
} ext_client_map_command_t;

typedef __packed struct
{
	std_frame_header_t txFrameHeader;			
	uint16_t  CmdID;										
	ext_client_map_command_t clientData;		
	uint16_t	FrameTail;								
}radar_command_t;


/****************************绘图的数据段内容****************************/

/*图层删除操作 data_ID: 0x0100  Byte:  2	*/
typedef enum
{
	NONE_delete    = 0,
	GRAPHIC_delete = 1,
	ALL_delete     = 2
}delete_Graphic_Operate;//ext_client_custom_graphic_delete_t：uint8_t operate_type

/* data_ID: 0x0100  Byte:  2	    客户端删除图形*/
typedef __packed struct
{
	delete_Graphic_Operate operate_type; 
	uint8_t 							 layer;//图层数：0~9
}ext_client_custom_graphic_delete_t;

//绘制图形
typedef __packed struct
{                          
	uint8_t graphic_name[3]; 
	
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; //直线   矩形  正圆  椭圆  		 圆弧  	浮点数  整型数  字符
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t details_a:9;    //空     空    空    空 		 起始角度  大小   大小   字体大小
	uint32_t details_b:9;    //空     空    空    空 		 终止角度  无作用  空    字符长度
	
	uint32_t width:10;       
	uint32_t start_x:11;     //起点/圆心 x 坐标
	uint32_t start_y:11;     //起点/圆心 y	坐标
	
	uint32_t details_c:10;   //空     空    半径   空       空     	数     数     空
	uint32_t details_d:11;   //终点x 对顶x   空   x半轴长  x半轴长 		数     数     空
	uint32_t details_e:11;   //终点y 对顶y   空   y半轴长  y半轴长 		数     数     空
} Graphic_data_struct_t;

//显示浮点数
typedef __packed struct
{                          
	uint8_t graphic_name[3]; 
	
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t details_a:9;  
	uint32_t details_b:9;    
	
	uint32_t width:10;       
	uint32_t start_x:11;    
	uint32_t start_y:11;     
	
  int32_t number;       		//该值除以 1000 即实际显示值
} Float_data_struct_t;

//显示整型数
typedef __packed struct
{                          
	uint8_t graphic_name[3]; 
	
	uint32_t operate_tpye:3; 
	uint32_t graphic_tpye:3; 
	uint32_t layer:4;        
	uint32_t color:4;        
	uint32_t details_a:9;  
	uint32_t details_b:9;    
	
	uint32_t width:10;       
	uint32_t start_x:11;    
	uint32_t start_y:11;     
	
  int32_t number;       
} Int_data_struct_t;

/*图层操作*/
//graphic_conf 1   bit 0-2
typedef enum
{
	NONE   = 0,/*空操作*/
	ADD    = 1,/*增加图层*/
	MODIFY = 2,/*修改图层*/
	DELETE = 3,/*删除图层*/
}Graphic_Operate;//graphic_data_struct_t：uint32_t operate_tpye

/*图层类型*/
//graphic_conf 1  bit3-5
typedef enum
{
	LINE      = 0,//直线
	RECTANGLE = 1,//矩形
	CIRCLE    = 2,//正圆
	OVAL      = 3,//椭圆
	ARC       = 4,//圆弧
	FLOAT     = 5,//浮点数
	INT       = 6,//整型数FLOAT
	CHAR      = 7 //字符
}Graphic_Type;

/*图层数*/
//graphic_conf 1  bit 6-9图层数 最大为9，最小0

/*图形/字体颜色*/
//graphic_conf 1  bit 10-13颜色
typedef enum
{
	RED_BLUE  = 0,//红蓝主色	
	YELLOW    = 1,
	GREEN     = 2,
	ORANGE    = 3,
	FUCHSIA   = 4,	/*紫红色*/
	PINK      = 5,
	CYAN_BLUE = 6,	/*青色*/
	BLACK     = 7,
	WHITE     = 8
}Graphic_Color;

/*图层颜色类型*/
//graphic_conf 1  bit 14-31 角度 [0,360]
/**
	*	角度值含义为：0°指 12 点钟方向，顺时针绘制
	*	屏幕位置：（0,0）为屏幕左下角（1920，1080）为屏幕右上角
	**/


/**********************************客户端绘图************************************************/
//删除图层
typedef __packed struct
{
	std_frame_header_t				 					txFrameHeader;			
	uint16_t									 					CmdID;										
	ext_client_data_header_t   					dataFrameHeader;
	ext_client_custom_graphic_delete_t 	clientData;		
	uint16_t														FrameTail;								
}ext_deleteLayer_data_t;

//绘字符串
typedef __packed struct
{
	Graphic_data_struct_t grapic_data_struct;
	uint8_t 							data[30];
} ext_client_string_t;

typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			//帧头
	uint16_t  								CmdID;							//命令码
	ext_client_data_header_t  dataFrameHeader;		//数据段头结构
	ext_client_string_t 			clientData;					//数据段
	uint16_t									FrameTail;					//帧尾
}ext_charstring_data_t;

/**
  * @note  绘制图形
  */
//绘制一个图形
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			//帧头
	uint16_t  								CmdID;							//命令码
	ext_client_data_header_t  dataFrameHeader;		//数据段头结构
	Graphic_data_struct_t 		clientData;					//数据段
	uint16_t									FrameTail;					//帧尾
}ext_graphic_one_data_t;

//绘制两个图形
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Graphic_data_struct_t 		clientData[2];		
	uint16_t									FrameTail;								
}ext_graphic_two_data_t;

//绘制五个图形
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Graphic_data_struct_t 		clientData[5];		
	uint16_t									FrameTail;								
}ext_graphic_five_data_t;

//绘制七个图形
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Graphic_data_struct_t 		clientData[7];		
	uint16_t									FrameTail;								
}ext_graphic_seven_data_t;


/**
  * @note  绘制浮点型
  */
//绘制一个浮点型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Float_data_struct_t 			clientData;		
	uint16_t									FrameTail;								
}ext_float_one_data_t;

//绘制两个浮点型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Float_data_struct_t 			clientData[2];		
	uint16_t									FrameTail;								
}ext_float_two_data_t;

//绘制七个浮点型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Float_data_struct_t 			clientData[7];		
	uint16_t									FrameTail;								
}ext_float_seven_data_t;


/**
  * @note  绘制整型
  */
//绘制一个整型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Int_data_struct_t 				clientData;		
	uint16_t									FrameTail;								
}ext_int_one_data_t;

//绘制两个整型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Int_data_struct_t 				clientData[2];		
	uint16_t									FrameTail;								
}ext_int_two_data_t;

//绘制七个整型
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	Int_data_struct_t 				clientData[7];		
	uint16_t									FrameTail;								
}ext_int_seven_data_t;


//车间通信
typedef __packed struct
{
	std_frame_header_t 				txFrameHeader;			
	uint16_t  								CmdID;										
	ext_client_data_header_t  dataFrameHeader;
	float 										angle_Array[3];
	char   										See_Info_Array[6];
	uint16_t									FrameTail;								
}ext_car_data_t;

#endif
