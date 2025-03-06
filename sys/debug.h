#ifndef DEBUG_H
#define DEBUG_H

#include "stm32f10x.h"
void Debug_Init(void);
void print(const char* str);
#endif // !DEBUG_H