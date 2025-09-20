#include "uart_drv.h"

#ifndef	PI
#define	PI	3.14159265359
#endif

// 定义雷达数据结构体（适配N10_P雷达108字节协议）
//#pragma pack(push, 1)

typedef struct
{
	uint8_t header_0;        // 0xA5
	uint8_t header_1;        // 0x5A
	uint8_t len;             // 0x6C (108)
	uint8_t speed_h;
	uint8_t speed_l;
	uint8_t start_angle_h;
	uint8_t start_angle_l;
	
	struct
	{
		uint8_t dist_h;
		uint8_t dist_l;
		uint8_t strong;
	} point[32];             // 32个点云数据
	
	uint8_t reserved[2];     // 预留字节
	uint8_t end_angle_h;
	uint8_t end_angle_l;
	uint8_t crc;
} N10P_Packet_t;

// 定义雷达数据包
typedef struct Lidar_Struct_t
{
	float speed;          // 雷达转速
	float start_angle;    // 起始角度
	float end_angle;      // 结束角度
	
	struct
	{
		float angle;      // 点云角度
		float distances; // 点云距离
		uint8_t strong;    // 点云强度
	} points[32];        // 32个点云数据
	
} Lidar_t;

//#pragma pack(pop)

