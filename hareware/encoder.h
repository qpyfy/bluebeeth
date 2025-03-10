#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(void);

int Encoder_GetValue(int encoder);
#endif // !EN