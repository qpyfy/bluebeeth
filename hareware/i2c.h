#ifndef __I2C_H
#define __I2C_H

#include "stm32f10x.h"
#include "delay.h"

void I2C_GPIO_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_SendByte(uint8_t Byte);
uint8_t I2C_ReceiveByte(void);
void I2C_SendAck(uint8_t AckBit);
uint8_t I2C_ReceiveAck(void);

#endif
