#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "stm32f10x.h"
#include "usart3.h"


enum Direction{
    STOP = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



void Bluetooth_Init(void);

void Bluetooth_SendByte(uint8_t byte);

void Bluetooth_SendString(char* str);

uint8_t Bluetooth_ReceiveByte(void);

#endif // !BLUETOOTH_H