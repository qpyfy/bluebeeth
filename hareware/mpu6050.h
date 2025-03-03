#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f10x.h"
#include "i2c.h"
#include "bluetooth.h"
#define MPU6050_SLAVE_ADDRESS  (0x68<<1)      //MPU6050器件读地址

#define MPU6050_WHO_AM_I        0x75
#define MPU6050_SMPLRT_DIV      0  //8000Hz
#define MPU6050_DLPF_CFG        0
#define MPU6050_GYRO_OUT        0x43     //MPU6050陀螺仪数据寄存器地址
#define MPU6050_ACC_OUT         0x3B     //MPU6050加速度数据寄存器地址1

#endif // !MPU6050_H