#ifndef __CONTROL_H
#define __CONTROL_H

int8_t Vertical_Control(float Angle_real ,float Angle_aim ,int16_t GyroY_real);
float Velocity_Control(int16_t SpeedA_Real ,int16_t SpeedB_Real);
int8_t Turn_Control(int16_t GyroZ_Real);



#endif
