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

    
}
/*
测试MPU6050是否连接成功
*/
void MPU6050_LinkTest(void){
    uint8_t WHO_AM_I;
    I2C_ReadReg(MPU6050_SLAVE_ADDRESS, MPU6050_WHO_AM_I, &WHO_AM_I, 1);
    if(WHO_AM_I == 0x68){
        Bluetooth_SendString("MPU6050连接成功\r\n");
    }
    else {
        Bluetooth_SendString("MPU6050连接失败\r\n");
    }
}



void EXTI9_5_IRQHandler(void){
    if(EXTI_GetITStatus(EXTI_Line5) != RESET){
        EXTI_ClearITPendingBit(EXTI_Line5);
        //TODO 处理中断
    }
}