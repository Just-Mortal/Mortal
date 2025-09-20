#ifndef __LOCATOR_H
#define __LOCATOR_H

#include "imu.h"
#include "motor.h"
#include "pid_cal.h"
#include "lidar_protocol.h"

// λ����Ϣ
typedef struct Locator_pos_info_t
{
    float x_pos;
    float y_pos;
    float a_yaw;
		
}Locator_pos_t;

// ��λ����Ϣ
typedef struct Locator_Info_Struct_t
{
    //  ���㵽���̵��ٶ�
    float front;
    float right;
    float cycle;
		
		float yaw_bias;
		
    // 
    Locator_pos_t measure;
    Locator_pos_t	target;
	
    // PID ������
    pid_info_t* pid_x;
    pid_info_t* pid_y;
    pid_info_t* pid_yaw;
		
		
}Locator_info_t;

typedef struct Locator_struct_t
{
    Locator_info_t* info;

    void (*Init)(struct Locator_struct_t* locator);
    void (*Update)(struct Locator_struct_t* locator);
    void (*CalculatePID)(struct Locator_struct_t* locator);

}Locator_t;

extern Locator_t locator ;

#endif
