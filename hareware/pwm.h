#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"


void PWM_Init(void);

void PWM_SetDutyCycle(uint16_t dutyCycle);
#endif // !PWM_H