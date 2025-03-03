#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"


void PWM_Init(void);

void PWMA_SetDutyCycle(uint8_t dutyCycle);

void PWMB_SetDutyCycle(uint8_t dutyCycle);
#endif // !PWM_H