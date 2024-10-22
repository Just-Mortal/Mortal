#ifndef __FOUR_LINK_H
#define __FOUR_LINK_H

#include "rp_math.h"

// 单位 cm
#define L1 14.f			
#define L2 17.667f
#define L3 17.667f
#define L4 14.f

typedef struct Link_Coord_struct_t
{
    float xa;  float ya;
    float xb;  float yb;
    float xc;  float yc;
    float xd;  float yd;
		
    float xp;  float yp;    //  质心坐标

}Link_Coord_t;

typedef struct Link_Angle_struct_t
{
    /*弧度制*/
    float phi1;
    float phi2;
    float phi3;
    float phi4;
    float phi0;

    float vir_phi0;
  
    /*角度制*/  //  测试用
    float phi1_angle;
    float phi2_angle;
    float phi3_angle;
    float phi4_angle;
    float phi0_angle;

    float vir_phi0_angle;
    //  测试用

}Link_Angle_t;

typedef struct Link_Leg_Length_struct_t
{
    float l0_measure;
    float l0_target;
		
		float phi0_target;

    pid_t pid;
		pid_t	pid2;

}Link_Length_t;

typedef struct Link_Centroid_struct_t
{
    float mx_l1;  float my_l1;
    float mx_l2;  float my_l2;
    float mx_l3;  float my_l3;
    float mx_l4;  float my_l4;

    float centriod_coefficient;

}Link_Centroid_t;

typedef struct Link_Force_struct_t
{
    // 五连杆关节电机读取力矩
    float T_f_true;
    float T_b_true;

    // 五连杆正解直腿模型
    float F_leg_true;   // 直腿模型实际力
    float T_leg_true;   // 直腿模型实际力矩

    // 直腿模型各部分分力
    float F_leg;        // 保持腿长力
    float F_roll;       // roll轴补偿力
    float F_gravity;    // 重力补偿力
    float F_inertial;   // 侧向惯性补偿力
    
    float F_combine;    // 合力
    float T_combine;		// 合力矩   test

    // 直腿模型逆解关节力矩
    float T_f_target;   // 前腿关节力矩
    float T_b_target;   // 后腿关节力矩

}Link_Force_t;

typedef struct Link_Var_struct_t
{
		// 实际控制电机输出扭矩
		float real_set;
		float virt_set;
	
    Link_Angle_t*       angle;      // 连杆各角度
    Link_Coord_t*       coord;      // 连杆各点坐标
    Link_Force_t*       force;      // 连杆各个分力
    Link_Length_t*      length;     // 连杆等效前后腿长
    Link_Centroid_t*    centriod;   // 连杆各部分质心坐标
	
    // 计算五连杆几何参数
    float (*phi0_cal)(struct Link_Var_struct_t *link_var);
    float (*phi2_cal)(struct Link_Var_struct_t *link_var);
    float (*phi3_cal)(struct Link_Var_struct_t *link_var);
    float (*length_cal)(struct Link_Var_struct_t *link_var);
    void  (*coord_cal)(struct Link_Var_struct_t *link_var);
    // 计算五连杆质心坐标
    void (*centroid_cal)(struct Link_Var_struct_t *link_var);
    // 五连杆到直腿模型动力学解算
    void (*five2straight)(struct Link_Var_struct_t *link_var);
    
    // 五连杆初始化
    void (*init)(struct Link_Var_struct_t *link_var);
    // 五连杆总计算函数
    void (*update)(struct Link_Var_struct_t *link_var);
    void (*leg_ctrl)(struct Link_Var_struct_t *link_var);

    // 直腿模型逆解关节力矩
    float (*real_torque_cal)(struct Link_Var_struct_t *link_var, float Fl, float Tp);
    float (*virt_torque_cal)(struct Link_Var_struct_t *link_var, float Fl, float Tp);
  
}Link_t;

extern Link_t four_link;

#endif
