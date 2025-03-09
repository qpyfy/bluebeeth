#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Init(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int *AccX, int *AccY, int *AccZ, 
	int *GyroX, int *GyroY, int *GyroZ);
uint8_t MPU6050_Test(void);

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);

u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz);
#endif
