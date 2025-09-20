#include "uart_drv.h"

#ifndef	PI
#define	PI	3.14159265359
#endif

// �����״����ݽṹ�壨����N10_P�״�108�ֽ�Э�飩
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
	} point[32];             // 32����������
	
	uint8_t reserved[2];     // Ԥ���ֽ�
	uint8_t end_angle_h;
	uint8_t end_angle_l;
	uint8_t crc;
} N10P_Packet_t;

// �����״����ݰ�
typedef struct Lidar_Struct_t
{
	float speed;          // �״�ת��
	float start_angle;    // ��ʼ�Ƕ�
	float end_angle;      // �����Ƕ�
	
	struct
	{
		float angle;      // ���ƽǶ�
		float distances; // ���ƾ���
		uint8_t strong;    // ����ǿ��
	} points[32];        // 32����������
	
} Lidar_t;

//#pragma pack(pop)

