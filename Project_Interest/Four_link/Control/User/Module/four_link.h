#ifndef __FOUR_LINK_H
#define __FOUR_LINK_H

#include "rp_math.h"

// ��λ cm
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
		
    float xp;  float yp;    //  ��������

}Link_Coord_t;

typedef struct Link_Angle_struct_t
{
    /*������*/
    float phi1;
    float phi2;
    float phi3;
    float phi4;
    float phi0;

    float vir_phi0;
  
    /*�Ƕ���*/  //  ������
    float phi1_angle;
    float phi2_angle;
    float phi3_angle;
    float phi4_angle;
    float phi0_angle;

    float vir_phi0_angle;
    //  ������

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
    // �����˹ؽڵ����ȡ����
    float T_f_true;
    float T_b_true;

    // ����������ֱ��ģ��
    float F_leg_true;   // ֱ��ģ��ʵ����
    float T_leg_true;   // ֱ��ģ��ʵ������

    // ֱ��ģ�͸����ַ���
    float F_leg;        // �����ȳ���
    float F_roll;       // roll�Ჹ����
    float F_gravity;    // ����������
    float F_inertial;   // ������Բ�����
    
    float F_combine;    // ����
    float T_combine;		// ������   test

    // ֱ��ģ�����ؽ�����
    float T_f_target;   // ǰ�ȹؽ�����
    float T_b_target;   // ���ȹؽ�����

}Link_Force_t;

typedef struct Link_Var_struct_t
{
		// ʵ�ʿ��Ƶ�����Ť��
		float real_set;
		float virt_set;
	
    Link_Angle_t*       angle;      // ���˸��Ƕ�
    Link_Coord_t*       coord;      // ���˸�������
    Link_Force_t*       force;      // ���˸�������
    Link_Length_t*      length;     // ���˵�Чǰ���ȳ�
    Link_Centroid_t*    centriod;   // ���˸�������������
	
    // ���������˼��β���
    float (*phi0_cal)(struct Link_Var_struct_t *link_var);
    float (*phi2_cal)(struct Link_Var_struct_t *link_var);
    float (*phi3_cal)(struct Link_Var_struct_t *link_var);
    float (*length_cal)(struct Link_Var_struct_t *link_var);
    void  (*coord_cal)(struct Link_Var_struct_t *link_var);
    // ������������������
    void (*centroid_cal)(struct Link_Var_struct_t *link_var);
    // �����˵�ֱ��ģ�Ͷ���ѧ����
    void (*five2straight)(struct Link_Var_struct_t *link_var);
    
    // �����˳�ʼ��
    void (*init)(struct Link_Var_struct_t *link_var);
    // �������ܼ��㺯��
    void (*update)(struct Link_Var_struct_t *link_var);
    void (*leg_ctrl)(struct Link_Var_struct_t *link_var);

    // ֱ��ģ�����ؽ�����
    float (*real_torque_cal)(struct Link_Var_struct_t *link_var, float Fl, float Tp);
    float (*virt_torque_cal)(struct Link_Var_struct_t *link_var, float Fl, float Tp);
  
}Link_t;

extern Link_t four_link;

#endif
