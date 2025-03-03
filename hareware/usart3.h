#ifndef USART3_H
#define USART3_H

#include "stm32f10x.h" 

void Usart3_Init(void);

void Usart3_SendByte(uint8_t byte);

void Usart3_SendString(char* str);

uint8_t Usart3_ReceiveByte(void);
#endif // !USART3_H