#ifndef I2C_H
#define I2C_H

#include "stm32f10x.h"

void I2C_GPIO_Init(void);

void I2C_WriteReg(uint8_t slaveAddr, uint8_t reg, uint8_t data);

uint8_t I2C_ReadReg(uint8_t slaveAddr, uint8_t reg, uint8_t* buff, uint8_t len);

#endif // !I2C_H