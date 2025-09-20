#include "Chassis.h"
#include "math.h"

// 斜坡规划器，防止转速突变烧坏电机
static void Chassis_SpeedRamp(Chassis_t *Chassis);

Chassis_t chassis = {
    .motor = {
        &motor[MOTOR_LF],
        &motor[MOTOR_RF],
        &motor[MOTOR_LB],
        &motor[MOTOR_RB],
    },

    .Init = Chassis_Init,
};

void Chassis_Init(Chassis_t *Chassis)
{
    // Initialize all motors
    for (int i = 0; i < MOTOR_NUM; i++)
        Chassis->motor[i]->Init(Chassis->motor[i]);

    Chassis->Work = Chassis_Work;
    Chassis->Update = Chassis_Update;
    Chassis->Sleep = Chassis_Sleep;

    Chassis->info.front_speed = 0.0f;
    Chassis->info.right_speed = 0.0f;
    Chassis->info.cycle_speed = 0.0f;
    for (int i = 0; i < MOTOR_NUM; i++)
    {
        Chassis->info.target_speed[i] = 0.0f;
        Chassis->info.output[i] = 0;
    }
}

void Chassis_Update(Chassis_t *Chassis)
{
    // Update motor speed
    for (int i = 0; i < MOTOR_NUM; i++)
        Chassis->motor[i]->UpdateSpeed(Chassis->motor[i]);
//        Chassis->info.measure_speed[i] = Chassis->motor[i]->measure_speed;

    // Calculate the speed for each motor based on the chassis info
    Chassis->info.target_speed[MOTOR_LF] =-Chassis->info.front_speed - Chassis->info.right_speed - Chassis->info.cycle_speed;
    Chassis->info.target_speed[MOTOR_RF] =+Chassis->info.front_speed - Chassis->info.right_speed - Chassis->info.cycle_speed;
    Chassis->info.target_speed[MOTOR_LB] =-Chassis->info.front_speed + Chassis->info.right_speed - Chassis->info.cycle_speed;
    Chassis->info.target_speed[MOTOR_RB] =+Chassis->info.front_speed + Chassis->info.right_speed - Chassis->info.cycle_speed;

    // 限制最大转速为 1000，单个电机超转速成比例均分到另外三个电机，保证运动合方向符合预期
    float max_speed = 1000.0f; // 最大转速
    if(fabs(Chassis->info.target_speed[MOTOR_LF]) > max_speed ||
       fabs(Chassis->info.target_speed[MOTOR_RF]) > max_speed ||
       fabs(Chassis->info.target_speed[MOTOR_LB]) > max_speed ||
       fabs(Chassis->info.target_speed[MOTOR_RB]) > max_speed)
    {  
        float max_tar_speed = fmaxf(fabs(Chassis->info.target_speed[MOTOR_LF]),
                              fmaxf(fabs(Chassis->info.target_speed[MOTOR_RF]),
                              fmaxf(fabs(Chassis->info.target_speed[MOTOR_LB]),
                                    fabs(Chassis->info.target_speed[MOTOR_RB]))));

        float scale = max_speed / max_tar_speed; // 计算缩放比例
    
        for (int i = 0; i < MOTOR_NUM; i++)
            Chassis->info.target_speed[i] *= scale;
    }

    // 斜坡规划器，防止转速突变
    Chassis_SpeedRamp(Chassis);

    // PID 计算
    for (int i = 0; i < MOTOR_NUM; i++)
    {
        Chassis->motor[i]->target_speed = Chassis->info.target_speed[i];
        Chassis->motor[i]->PIDControl(Chassis->motor[i]);
        Chassis->info.output[i] = (int16_t)Chassis->motor[i]->pid_info.out;
    }
}

void Chassis_Work(Chassis_t *Chassis)
{
    for (int i = 0; i < MOTOR_NUM; i++)
        Chassis->motor[i]->SetSpeed(Chassis->motor[i], Chassis->info.output[i]);
}

void Chassis_Sleep(Chassis_t *Chassis)
{
    for (int i = 0; i < MOTOR_NUM; i++)
        Chassis->motor[i]->Sleep(Chassis->motor[i]);
}

/**
 * @brief 斜坡规划器，防止电机转速突变（任务周期1ms）
 * @note 使用静态变量记录状态，实现速度的平滑过渡
 * @param Chassis 底盘结构体指针
 * @author Jia Sujian
 */
float ramp_step = 2.f; // 速度变化步长
static void Chassis_SpeedRamp(Chassis_t *Chassis)
{
    static float last_speed[MOTOR_NUM] = {0.0f, 0.0f, 0.0f, 0.0f};
    static float ramp_speed[MOTOR_NUM] = {0.0f, 0.0f, 0.0f, 0.0f};

    for (int i = 0; i < MOTOR_NUM; i++)
    {
        float speed_diff = Chassis->info.target_speed[i] - last_speed[i];

        if (fabs(speed_diff) > ramp_step)
            ramp_speed[i] = last_speed[i] + (speed_diff > 0 ? ramp_step : -ramp_step);
        else
            ramp_speed[i] = Chassis->info.target_speed[i];

        last_speed[i] = ramp_speed[i];
    }

    for (int i = 0; i < MOTOR_NUM; i++)
        Chassis->info.target_speed[i] = ramp_speed[i];
}

