#include "mpu6050.h"
//MPU6050地址 0x68
//MPU6050的I2C地址为0x68，读写时最后一位（bit0）为0表示写，为1表示读。
//写：0xD0 读：0xD1
//MPU6050内部寄存器地址


void MPU6050_Init(void){
    I2C_GPIO_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //外部中断
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); //启用GPIOB_PIN5外部中断
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    MPU6050_WakeUp();
}
/*
测试MPU6050是否连接成功
*/
uint8_t MPU6050_LinkTest(void){
    uint8_t WHO_AM_I = 0x00;
    I2C_ReadReg(MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I, &WHO_AM_I, 1);
    return WHO_AM_I;
}

void MPU6050_WriteReg(uint8_t reg, uint8_t data){
    I2C_WriteReg(MPU6050_SLAVE_ADDRESS, reg, data);
}
/*
@note 复位MPU6050
*/
void MPU6050_Reset(void){
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x80);
}
/*
@note 唤醒MPU6050
*/
void MPU6050_WakeUp(void){
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x00);
}
/*
@param config 分辨率 0~3
@note 0: ±250°/s   1: ±500°/s
@note 2: ±1000°/s  3: ±2000°/s
*/
void MPU6050_SetGyroConfig(uint8_t config){
    config = config > 3 ? 3 : (config < 0 ? 0 : config); //限制config范围0-3
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, config);
}

void MPU6050_SetAccelConfig(uint8_t config){
    config = config > 3 ? 3 : (config < 0 ? 0 : config); //限制config范围0-3
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, config);
}

void MPU6050_ReadReg(uint8_t reg, uint8_t* buff, uint8_t len){
    I2C_ReadReg(MPU6050_SLAVE_ADDRESS, reg, buff, len);
}



void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line5) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line5);
        //TODO 处理陀螺仪数据
    }
}