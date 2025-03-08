#include "stm32f10x.h"                  // Device header
#include "i2c.h"
#include "mpu6050_reg.h"
#include "oled.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址


void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	I2C_Start();						//I2C起始
	I2C_SendByte(MPU6050_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	I2C_ReceiveAck();					//接收应答
	I2C_SendByte(RegAddress);			//发送寄存器地址
	I2C_ReceiveAck();					//接收应答
	I2C_SendByte(Data);				//发送要写入寄存器的数据
	I2C_ReceiveAck();					//接收应答
	I2C_Stop();						//I2C终止
	return 0;
}

u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf){
	I2C_Start();						//I2C起始
	I2C_SendByte(addr);	//发送从机地址，读写位为0，表示即将写入
	I2C_ReceiveAck();					//接收应答
	I2C_SendByte(reg);			//发送寄存器地址
	I2C_ReceiveAck();					//接收应答
	for(u8 i = 0;i < len;i++){
		I2C_SendByte(buf[i]);
		if(I2C_ReceiveAck())
		{
			I2C_Stop();
			return 1;
		}
	}
	I2C_Stop();						//I2C终止
	return 0;
}

u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf){
	I2C_Start();
	I2C_SendByte(addr);
	I2C_ReceiveAck();
	I2C_SendByte(reg);
	I2C_ReceiveAck();
	for(u8 i = 0;i < len;i++){
		buf[i] = I2C_ReceiveByte();
		I2C_SendAck(i == len - 1 ? 0 : 1);
	}
	I2C_Stop();
	return 0;
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	I2C_Start();						//I2C起始
	I2C_SendByte(MPU6050_ADDRESS);	//发送从机地址，读写位为0，表示即将写入
	if(I2C_ReceiveAck()){
		return 66;
	}	
	I2C_SendByte(RegAddress);			//发送寄存器地址
	if(I2C_ReceiveAck()){
		return 77;
	}					//接收应答
	
	I2C_Start();						//I2C重复起始
	I2C_SendByte(MPU6050_ADDRESS | 0x01);	//发送从机地址，读写位为1，表示即将读取
	if(I2C_ReceiveAck()){
		return 88;
	}						//接收应答
	Data = I2C_ReceiveByte();			//接收指定寄存器的数据
	I2C_SendAck(1);					//发送应答，给从机非应答，终止从机的数据输出
	I2C_Stop();						//I2C终止
	
	return Data;
}


void MPU6050_Init(void)
{
	I2C_GPIO_Init();									//先初始化底层的I2C
	// 复位MPU6050
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x80);
	/*MPU6050寄存器初始化，需要对照MPU6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		//采样率分频寄存器，配置采样率
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);			//配置寄存器，配置DLPF
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	//加速度计配置寄存器，选择满量程为±16g
	MPU6050_WriteReg(MPU6050_INT_ENABLE, 0X01);     //中断使能寄存器，使能INT_PIN_INTERRUPT
	
    //TODO 配置中断引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);

	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);


	NVIC_EnableIRQ(EXTI9_5_IRQn);

	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//返回WHO_AM_I寄存器的值
}

uint8_t MPU6050_Test(void){
	return MPU6050_ReadReg(MPU6050_CONFIG);
}


void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);		//读取加速度计X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);		//读取加速度计X轴的低8位数据
	*AccX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);		//读取加速度计Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);		//读取加速度计Y轴的低8位数据
	*AccY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);		//读取加速度计Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);		//读取加速度计Z轴的低8位数据
	*AccZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);		//读取陀螺仪X轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);		//读取陀螺仪X轴的低8位数据
	*GyroX = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);		//读取陀螺仪Y轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);		//读取陀螺仪Y轴的低8位数据
	*GyroY = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);		//读取陀螺仪Z轴的高8位数据
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);		//读取陀螺仪Z轴的低8位数据
	*GyroZ = (DataH << 8) | DataL;						//数据拼接，通过输出参数返回
}

