#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f10x.h"

void Encoder_Init(void);

u8 Encoder_GetValue(uint8_t encoder);
#endif // !EN