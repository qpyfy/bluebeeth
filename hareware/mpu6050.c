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


void MPU_Set_Gyro_Fsr(u8 fsr)
{
	MPU6050_WriteReg(MPU_GYRO_CFG_REG,fsr<<3);	//设置陀螺仪满量程范围
}

void MPU_Set_Accel_Fsr(u8 fsr)
{
	MPU6050_WriteReg(MPU_ACCEL_CFG_REG,fsr<<3);	//设置加速度传感器满量程范围
}

void MPU_Set_Rate(u8 rate)
{
	MPU6050_WriteReg(MPU_SAMPLE_RATE_REG,rate);	//设置采样率
}

void MPU6050_Init(void)
{
	I2C_GPIO_Init();									//先初始化底层的I2C
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0X80);	//复位MPU6050
    delay_ms(10);
	MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0X00);	//唤醒MPU6050 
	MPU_Set_Gyro_Fsr(3);					//陀螺仪传感器,±2000dps
	MPU_Set_Accel_Fsr(0);					//加速度传感器,±2g
	MPU_Set_Rate(200);						//设置采样率50Hz
	MPU6050_WriteReg(MPU_INT_EN_REG,0X00);	//关闭所有中断
	MPU6050_WriteReg(MPU_USER_CTRL_REG,0X00);	//I2C主模式关闭
	MPU6050_WriteReg(MPU_FIFO_EN_REG,0X00);	//关闭FIFO
	MPU6050_WriteReg(MPU_INTBP_CFG_REG,0X80);	//INT引脚低电平有效
	u8 res=MPU6050_ReadReg(MPU_DEVICE_ID_REG); 
	if(res==MPU_ADDR)//器件ID正确
	{
		MPU6050_WriteReg(MPU_PWR_MGMT1_REG,0X01);	//设置CLKSEL,PLL X轴为参考
		MPU6050_WriteReg(MPU_PWR_MGMT2_REG,0X00);	//加速度与陀螺仪都工作
		MPU_Set_Rate(100);						//设置采样率为50Hz
 	}
    //TODO 配置中断引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
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
	return MPU6050_ReadReg(MPU_ADDR << 1);		//返回WHO_AM_I寄存器的值
}

uint8_t MPU6050_Test(void){
	return MPU6050_ReadReg(MPU_ADDR << 1);
}


void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;								//定义数据高8位和低8位的变量
	
	DataH = MPU6050_ReadReg(MPU_ACCEL_XOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_XOUTL_REG);
	*AccX = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU_ACCEL_YOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_YOUTL_REG);
	*AccY = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU_ACCEL_ZOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_ACCEL_ZOUTL_REG);
	*AccZ = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU_GYRO_XOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_XOUTL_REG);
	*GyroX = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU_GYRO_YOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_YOUTL_REG);
	*GyroY = (DataH << 8) | DataL;

	DataH = MPU6050_ReadReg(MPU_GYRO_ZOUTH_REG);
	DataL = MPU6050_ReadReg(MPU_GYRO_ZOUTL_REG);
	*GyroZ = (DataH << 8) | DataL;
}

