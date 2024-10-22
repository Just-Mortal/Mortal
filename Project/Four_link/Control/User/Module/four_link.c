#include "four_link.h"

#include "arm_math.h"

// pravite function
//static float Phi0_Cal(Link_t* Link_Var);
static float Phi2_Cal(Link_t* Link_Var);
static float Phi3_Cal(Link_t* Link_Var);
//static float Leg_Length_Cal(Link_t* Link_Var);
//static void Link_Coordinate_Cal(Link_t* Link_Var);
static void Five_Transform_Straight(Link_t* Link_Var);

// external function
static void four_link_Init(Link_t* Link_Var);
static void four_link_Cal(Link_t* Link_Var);
static void Leg_Length_Ctrl(Link_t* Link_Var);

// 直腿模型逆解五连杆关节力矩
static float Virt_Joint_Torque_Cal(Link_t* Link_Var, float Fl, float Tp);
static float Real_Joint_Torque_Cal(Link_t* Link_Var, float Fl, float Tp);

// 四连杆解算
static void Link_Coordinate_Cal(Link_t* Link_Var);
static void Link_End_Cal(Link_t* Link_Var);

Link_Angle_t   four_link_angle;
Link_Coord_t   four_link_coord;
Link_Force_t   four_link_force;
Link_Length_t  four_link_length = 
{
	.l0_target = 18.f,
	.l0_measure = 0.f,
	
	.phi0_target = 0.f,
	
	.pid = 
	{
			.kp = 300.f,
			.ki = 2.f,
			.kd = 30000.f,
			.d_lpf = 0.6f,
			.integral_max = 200.f,
			.out_max = 2000.f,
	},
	
	.pid2 = 
	{
			.kp = 10000.f,
			.ki = 2.f,
			.kd = 90000.f,
			.d_lpf = 0.6f,
			.integral_max = 200.f,
			.out_max = 1000.f,
	},
};

Link_Centroid_t four_link_centroid;

Link_t four_link = 
{
	.init = four_link_Init,

	.angle = &four_link_angle,
	.coord = &four_link_coord,
	.force = &four_link_force,
	.length = &four_link_length,
	.centriod = &four_link_centroid,
   
};

// 五连杆初始化
static void four_link_Init(Link_t* Link_Var)
{
    Link_Var->update = four_link_Cal;

//    Link_Var->phi0_cal = Phi0_Cal;
    Link_Var->phi2_cal = Phi2_Cal;
    Link_Var->phi3_cal = Phi3_Cal;
//		Link_Var->length_cal = Leg_Length_Cal;
//    Link_Var->coord_cal = Link_Coordinate_Cal;
    Link_Var->five2straight = Five_Transform_Straight;
    Link_Var->leg_ctrl = Leg_Length_Ctrl;

    Link_Var->virt_torque_cal = Virt_Joint_Torque_Cal;
    Link_Var->real_torque_cal = Real_Joint_Torque_Cal;
}

// 五连杆总计算函数
static void four_link_Cal(Link_t* Link_Var)
{
    //  phi1 和 phi4 外部更新
		
		// 计算 B D 两点坐标
		Link_Coordinate_Cal(Link_Var);
		
		// 由 B D 两点坐标计算 phi2 和 phi3
    Link_Var->angle->phi2 = Link_Var->phi2_cal(Link_Var);
    Link_Var->angle->phi3 = Link_Var->phi3_cal(Link_Var);

		// 计算 C 点坐标，L0 和 phi0
		Link_End_Cal(Link_Var);

//    Link_Var->five2straight(Link_Var);

    // 用于测试角度
    Link_Var->angle->phi0_angle = Link_Var->angle->phi0 * 180.f / PI;
    Link_Var->angle->phi1_angle = Link_Var->angle->phi1 * 180.f / PI;
    Link_Var->angle->phi2_angle = Link_Var->angle->phi2 * 180.f / PI;
    Link_Var->angle->phi3_angle = Link_Var->angle->phi3 * 180.f / PI;
    Link_Var->angle->phi4_angle = Link_Var->angle->phi4 * 180.f / PI;
    Link_Var->angle->vir_phi0_angle = Link_Var->angle->vir_phi0 * 180.f / PI;
		
		// 计算腿长控制力
		Link_Var->leg_ctrl(Link_Var);
		
		Link_Var->real_set = Link_Var->real_torque_cal(Link_Var, Link_Var->force->F_leg, Link_Var->force->T_combine);
		Link_Var->virt_set = Link_Var->virt_torque_cal(Link_Var, Link_Var->force->F_leg, Link_Var->force->T_combine);
}

// 计算phi2
static float Phi2_Cal(Link_t* Link_Var)
{
    float phi2 = 0.f, son = 0.f, mother = 0.f;
		float A, B, C, BD, xB, yB, xD, yD;

		xB = Link_Var->coord->xb; yB = Link_Var->coord->yb;
		xD = Link_Var->coord->xd; yD = Link_Var->coord->yd;
    
		A = 2.f * L2 * (xD - xB);
		B = 2.f * L2 * (yD - yB);
		BD = sqrt(pow((xD - xB), 2.f) + pow((yD - yB), 2.f));
		C = L2 * L2 + BD * BD - L3 * L3;
		
		son = B + sqrt(A*A + B*B - C*C);
		mother = A + C;

    phi2 = 2 * atan2f(son, mother);

    return phi2;
}

static float Phi3_Cal(Link_t* Link_Var)
{
    float phi1 = 0.f, phi2 = 0.f, phi3 = 0.f, phi4 = 0.f;

    phi1 = Link_Var->angle->phi1;
		phi2 = Link_Var->angle->phi2;
    phi4 = Link_Var->angle->phi4;
		
		phi3 = phi1 + phi4 - phi2;

    return phi3;
}


void Five_Transform_Straight(Link_t* Link_Var)
{
    float phi0 = 0.f, phi1 = 0.f, phi2 = 0.f, phi3 = 0.f, phi4 = 0.f, l0 = 0.f;
    float T1 = 0.f, T2 = 0.f, F = 0.f, T = 0.f;
	
		phi0    = Link_Var->angle->phi0;
    phi1    = Link_Var->angle->phi1;
    phi2    = Link_Var->angle->phi2;
    phi3    = Link_Var->angle->phi3;
		phi4    = Link_Var->angle->phi4;
    T1      = Link_Var->force->T_f_true;
    T2      = Link_Var->force->T_b_true;
    l0      = Link_Var->length->l0_measure;

    F = -(T2*L1*sinf(phi0 + phi1 - phi2 - phi3) - T2*L1*sinf(phi0 - phi1 + phi2 - phi3) - T1*L4*sinf(phi0 - phi2 + phi3 - phi4) + \
    T1*L4*sinf(phi0 - phi2 - phi3 + phi4))/(L1*L4*(cosf(phi1 - phi2 + phi3 - phi4) - cosf(phi1 - phi2 - phi3 + phi4)));
	
    T = -(l0*(T2*L1*cosf(phi0 + phi1 - phi2 - phi3) - T2*L1*cosf(phi0 - phi1 + phi2 - phi3) - T1*L4*cosf(phi0 - phi2 + phi3 - phi4) + \
    T1*L4*cosf(phi0 - phi2 - phi3 + phi4)))/(L1*L4*(cosf(phi1 - phi2 + phi3 - phi4) - cosf(phi1 - phi2 - phi3 + phi4)));

    Link_Var->force->F_leg_true = F;
    Link_Var->force->T_leg_true = T;
}

float Virt_Joint_Torque_Cal(Link_t* Link_Var, float Fl, float Tp)
{
    float phi0 = 0.f, phi1, phi2 = 0.f, phi3 = 0.f, l0 = 0.f;

    phi0 = Link_Var->angle->phi0;
    phi1 = Link_Var->angle->phi1;
    phi2 = Link_Var->angle->phi2;
    phi3 = Link_Var->angle->phi3;

    l0 = Link_Var->length->l0_measure;

    float T = - (Fl*L1*sinf(phi0 - phi3)*sinf(phi1 - phi2))/sinf(phi2 - phi3) - (Tp*L1*cosf(phi0 - phi3)*sinf(phi1 - phi2))/(l0*sinf(phi2 - phi3));

    return T;
}

float Real_Joint_Torque_Cal(Link_t* Link_Var, float Fl, float Tp)
{
    float phi0 = 0.f, phi2 = 0.f, phi3 = 0.f, phi4 = 0.f, l0 = 0.f;

    phi0 = Link_Var->angle->phi0;
    phi2 = Link_Var->angle->phi2;
    phi3 = Link_Var->angle->phi3;
    phi4 = Link_Var->angle->phi4;
    
    l0 = Link_Var->length->l0_measure;

    float T = - (Fl*L4*sinf(phi0 - phi2)*sinf(phi3 - phi4))/sinf(phi2 - phi3) - (Tp*L4*cosf(phi0 - phi2)*sinf(phi3 - phi4))/(l0*sinf(phi2 - phi3));

    return T;
}

// 腿长控制力
void Leg_Length_Ctrl(Link_t* Link_Var)
{
    // 腿长控制力
    float l0_target = 0.f, l0_measure = 0.f;
    float F = 0.f;

    l0_target = Link_Var->length->l0_target;
    l0_measure = Link_Var->length->l0_measure;

    Link_Var->length->pid.target = l0_target;
    Link_Var->length->pid.measure = l0_measure;

    single_pid_cal(&Link_Var->length->pid);

    F = Link_Var->length->pid.out;

    Link_Var->force->F_leg = F;

    // phi0控制力矩，测试用
    float phi_measure = 0.f, phi_target = 0.f;
    float T = 0.f;

    phi_measure = Link_Var->angle->vir_phi0;
    phi_target = Link_Var->length->phi0_target;

    Link_Var->length->pid2.target = phi_target;
    Link_Var->length->pid2.measure = phi_measure;

    single_pid_cal(&Link_Var->length->pid2);

    T = Link_Var->length->pid2.out;

    Link_Var->force->T_combine = T;
}

void Link_Coordinate_Cal(Link_t* Link_Var)
{
		float phi1 = Link_Var->angle->phi1;
		float phi4 = Link_Var->angle->phi4;

		// A 点坐标
		Link_Var->coord->xa = 0.f;
		Link_Var->coord->ya	= 0.f;

		// B 点坐标
    Link_Var->coord->xb = L1*arm_cos_f32(phi1);
    Link_Var->coord->yb = L1*arm_sin_f32(phi1);
		
		// D 点坐标
    Link_Var->coord->xd = L4*arm_cos_f32(phi4);
    Link_Var->coord->yd = L4*arm_sin_f32(phi4);

}

void Link_End_Cal(Link_t* Link_Var)
{
	float phi2 = Link_Var->angle->phi2;

	// C 点坐标
	Link_Var->coord->xc = Link_Var->coord->xb + L2 * arm_cos_f32(phi2);
	Link_Var->coord->yc = Link_Var->coord->yb + L2 * arm_sin_f32(phi2);
	
	// L0 长度
	float xc = Link_Var->coord->xc, yc = Link_Var->coord->yc, ya = Link_Var->coord->ya;
	Link_Var->length->l0_measure = sqrtf(xc*xc + yc*yc);
	if(yc < ya)
		Link_Var->length->l0_measure = -Link_Var->length->l0_measure;

	// phi0 角度
	Link_Var->angle->phi0 = (Link_Var->angle->phi1 + Link_Var->angle->phi4) / 2.f;
	Link_Var->angle->vir_phi0 = PI / 2.f - Link_Var->angle->phi0;
}
