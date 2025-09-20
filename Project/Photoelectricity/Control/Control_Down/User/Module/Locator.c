#include "Locator.h"

#include "pid_cal.h"

// lidar
extern uint8_t data_process_flag;
extern N10P_Packet_t dma_pack_data;

static void Locator_Init(struct Locator_struct_t* locator);
static void Locator_Update(struct Locator_struct_t* locator);
static void Locator_CalculatePID(struct Locator_struct_t* locator);

// 2.1 0.2

pid_info_t pid_x = {
    .kp = 800.0f,
    .ki = 0.f,
    .kd = 0.01f,
    .integral_max = 0.0f,
    .out_max = 1000.0f
};
pid_info_t pid_y = {
    .kp = 800.0f,
    .ki = 0.f,
    .kd = 0.01f,
    .integral_max = 0.0f,
    .out_max = 1000.0f
};
pid_info_t pid_yaw = {
    .kp = 500.0f,
    .ki = 0.f,
    .kd = 0.01f,
    .integral_max = 0.0f,
    .out_max = 500.0f
};

Locator_info_t  locator_info = {
    .pid_x = &pid_x,
    .pid_y = &pid_y,
    .pid_yaw = &pid_yaw
};

Locator_t locator = {
    .info = &locator_info,
    
    .Init = Locator_Init,
    .Update = Locator_Update,
    .CalculatePID = Locator_CalculatePID
};

static void Locator_Init(struct Locator_struct_t* locator)
{
    // Initialize PID controllers
    pid_init(locator->info->pid_x);
    pid_init(locator->info->pid_y);
    pid_init(locator->info->pid_yaw);

    // Initialize position information
    locator->info->measure.x_pos = 0.0f;
    locator->info->measure.y_pos = 0.0f;
    locator->info->measure.a_yaw = 0.0f;
    locator->info->target.x_pos = 0.0f;
    locator->info->target.y_pos = 0.0f;
    locator->info->target.a_yaw = 0.0f;
    locator->info->front = 0;
    locator->info->right = 0;
    locator->info->cycle = 0;
}

// �������� 1ms�� ���굥λ m
static void Locator_Update(struct Locator_struct_t* locator)
{
    // �����ķ�ֵ����ٶȼ���
    float front_speed = motor[MOTOR_RF].measure_speed + motor[MOTOR_RB].measure_speed -
                        motor[MOTOR_LF].measure_speed - motor[MOTOR_LB].measure_speed;
    float right_speed = motor[MOTOR_LB].measure_speed + motor[MOTOR_RB].measure_speed -
                        motor[MOTOR_LF].measure_speed - motor[MOTOR_RF].measure_speed;
    float cycle_speed = -motor[MOTOR_LF].measure_speed - motor[MOTOR_RF].measure_speed -
                        motor[MOTOR_LB].measure_speed - motor[MOTOR_RB].measure_speed;
												
		front_speed /= 4.f;		right_speed /= 4.f;		cycle_speed /= 4.f;
		

    locator->info->measure.x_pos += front_speed * 0.001f;
    locator->info->measure.y_pos += right_speed * 0.001f;
		
		
    locator->info->measure.a_yaw += cycle_speed * 0.001f;

		//	yaw
//		locator->info->measure.a_yaw = imu_data.yaw - locator->info->yaw_bias;
//		while(locator->info->measure.a_yaw < -180.f)	locator->info->measure.a_yaw += 360.f;
//		while(locator->info->measure.a_yaw > +180.f)	locator->info->measure.a_yaw -= 360.f;
}

static void Locator_CalculatePID(struct Locator_struct_t* locator)
{
		// x 0 - 1.6m ��֤ 1000 ��� 3m / s
		if(abs(locator->info->pid_x->err) > 0.5f)
			locator->info->pid_x->kp = 2000.f;
		else
			locator->info->pid_x->kp = 800.f;

    // ���� pid ������
    locator->info->pid_x->target = locator->info->target.x_pos;
    locator->info->pid_y->target = locator->info->target.y_pos;
    locator->info->pid_yaw->target = locator->info->target.a_yaw;

    locator->info->pid_x->measure = locator->info->measure.x_pos;
    locator->info->pid_y->measure = locator->info->measure.y_pos;
		
    locator->info->pid_yaw->measure = locator->info->measure.a_yaw;

    // ����PID������
    single_pid_cal(locator->info->pid_x);
    single_pid_cal(locator->info->pid_y);
    single_pid_cal(locator->info->pid_yaw);

    // ���µ����ٶ�
    locator->info->front = locator->info->pid_x->out;
    locator->info->right = locator->info->pid_y->out;
    locator->info->cycle = locator->info->pid_yaw->out;
}

// ---------------------- Lidar ---------------------- //
// ���ݴ�������������ѭ���е��ã�
void data_process_dma(void)
{
    if (!data_process_flag) return;
    
    // 1. �����Ƕ���Ϣ
    float start_angle = (dma_pack_data.start_angle_h << 8 | dma_pack_data.start_angle_l) / 100.0f;
    float end_angle = (dma_pack_data.end_angle_h << 8 | dma_pack_data.end_angle_l) / 100.0f;
    
    // 2. ����ǶȻ���
    if (start_angle > end_angle)	end_angle += 360;

    // 3. ����ÿ�����ʵ�ʽǶ�
    float angle_step = (end_angle - start_angle) / 32.0f;
    
    // 4. ����˫�ز�����
    for (int i = 0; i < 32; i++)
		{
        uint16_t distance = (dma_pack_data.point[i].dist_h << 8) | dma_pack_data.point[i].dist_l;
        
        // ��Ч���жϣ�������룩
        if (distance != 0)
				{
            float current_angle = start_angle + angle_step * i;
            
            // ����ת�����ĵ�2.4������ϵ��
            float rad = current_angle * PI / 180.0f;
            float x = distance * cosf(rad);
            float y = distance * sinf(rad);
            
//            add_to_obstacle_queue(x, y, distance);
        }
    }
    
    data_process_flag = 0;  // ��������־
}

// α���룺���ϴ����߼�
//void add_to_obstacle_queue(float x, float y, uint16_t dist)
//{
//    // ֻ��עǰ��100������-50��~+50�㣩
//    if (fabsf(atan2f(y, x)) < 50.0f * PI/180.0f)
//		{
//        // 220mm������ֵ���
//        if (dist < 220 && dist > 0)
//				{
//            // �ϰ��﷽���жϣ���/�ң�
//            bool left_obstacle = (y > 0); 
//            trigger_avoidance_action(left_obstacle);
//        }
//    }
//}
