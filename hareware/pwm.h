#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"


void PWM_Init(void);

void PWMA_SetDutyCycle(int dutyCycle);

void PWMB_SetDutyCycle(int dutyCycle);
#endif // !PWM_H