#include "i2c.h"

#define SDA_OUT(x) x ? GPIO_SetBits(GPIOB, GPIO_Pin_4) : GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define SCL_OUT(x) x ? GPIO_SetBits(GPIOB, GPIO_Pin_3) : GPIO_ResetBits(GPIOB, GPIO_Pin_3)
#define SDA_IN() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define SCL_IN() GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)

//72M 1us = 72
void I2C_Delay(void){
    for (size_t i = 0; i < 72; i++)
    {
        ;
    }
    
}

void I2C_GPIO_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void I2C_Start(void){
    SDA_OUT(1);
    SCL_OUT(1);
    SDA_OUT(0); //起始信号
    SCL_OUT(0); //拉下SCL，准备发送数据
}

void I2C_Stop(void){
    SCL_OUT(0); //拉低SCL,修改SDA
    SDA_OUT(0);
    SCL_OUT(1);
    SDA_OUT(1); //停止信号
}
// SCL高电平期间，SDA要保持稳定
void I2C_Ack(void){
    SCL_OUT(0); //拉低SCL,修改SDA
    SDA_OUT(0);
    SCL_OUT(1);
    I2C_Delay();//等待从机获取应答
    SDA_OUT(1);
}

void I2C_NoAck(void){
    SCL_OUT(0); //拉低SCL,修改SDA
    SDA_OUT(1);
    SCL_OUT(1);
    I2C_Delay(); //等待从机获取应答
    SDA_OUT(1);
}

uint8_t I2C_WaitAck(void){
    SDA_OUT(1);
    SCL_OUT(1);
    I2C_Delay();
    int err = 0;
    while (SDA_IN())
    {
        err++;
        if(err > 250){
            I2C_Stop();
            return 1;
        }
    }
    SCL_OUT(0); //拉低SCL,准备发送数据
    return 0;
}

void I2C_SendByte(uint8_t byte){
    for (size_t i = 0; i < 8; i++)
    {
        SCL_OUT(0); //拉低SCL,准备发送数据
        if(byte & 0x80){
            SDA_OUT(1); //发送数据
        }
        else {
            SDA_OUT(0);
        }
        SCL_OUT(1); //拉高SCL,从机读取数据
        byte <<= 1;
    }
}

uint8_t I2C_ReceiveByte(void){
    SDA_OUT(1);
    uint8_t byte = 0;
    for (size_t i = 0; i < 8; i++)
    {
        byte <<= 1;
        SCL_OUT(0);
        I2C_Delay();
        SCL_OUT(1);
        if(SDA_IN()){
            byte |= 0x01;
        }
    }
    return byte;
}

void I2C_WriteReg(uint8_t slaveAddr, uint8_t reg, uint8_t data){
    I2C_Start();
    I2C_SendByte(slaveAddr);
    I2C_WaitAck();
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
}

uint8_t I2C_ReadReg(uint8_t slaveAddr, uint8_t reg, uint8_t* buff, uint8_t len){
    uint8_t data;
    I2C_Start();
    I2C_SendByte(slaveAddr);
    I2C_WaitAck();
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(slaveAddr | 0x01);
    I2C_WaitAck();
    for (size_t i = 0; i < len; i++)
    {
        data = I2C_ReceiveByte();
        if(i == len - 1){
            I2C_NoAck();
        }
        else {
            I2C_Ack();
        }
        buff[i] = data;
    }
    I2C_Stop();
    return 1;
}

