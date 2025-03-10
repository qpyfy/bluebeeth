#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"
#include "pwm.h"

void Motor_Init(void);
void Motor_Load(int motor1, int motor2);

#endif // !MOTOR_H