#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f10x.h"
#include "pwm.h"

void Motor_Init(void);
void Motor_Load(uint8_t motor1, uint8_t motor2);

#endif // !MOTOR_H