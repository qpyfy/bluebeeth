#include "motor.h"
/*
pin_12 13 控制电机A
pin_14 15 控制电机B
*/
#define MOTOR_AIN1 GPIO_Pin_12
#define MOTOR_AIN2 GPIO_Pin_13

#define MOTOR_BIN1 GPIO_Pin_14
#define MOTOR_BIN2 GPIO_Pin_15

void Motor_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    PWM_Init();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = MOTOR_AIN1 | MOTOR_AIN2 | MOTOR_BIN1 | MOTOR_BIN2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}

#define MOTOR_SPEED 100

void Limit_Speed(uint8_t *motor1, uint8_t *motor2){
    if(*motor1 > MOTOR_SPEED){
        *motor1 = MOTOR_SPEED;
    }
    if(*motor2 > MOTOR_SPEED){
        *motor2 = MOTOR_SPEED;
    }
}
/*
@param motor1 电机A速度 motor2 电机B速度
@note 电机速度范围-100~100
@note 电机速度为正数时电机正转，为负数时电机反转
*/
void Motor_Load(uint8_t motor1, uint8_t motor2){
    Limit_Speed(&motor1, &motor2);
    if(motor1 > 0){
        GPIO_SetBits(GPIOB, MOTOR_AIN1);
        GPIO_ResetBits(GPIOB, MOTOR_AIN2);
        PWM_SetDutyCycle(motor1);
    }
    else {
        GPIO_ResetBits(GPIOB, MOTOR_AIN1);
        GPIO_SetBits(GPIOB, MOTOR_AIN2);
        PWM_SetDutyCycle(-motor1);
    }

    if (motor2 > 0){
        GPIO_SetBits(GPIOB, MOTOR_BIN1);
        GPIO_ResetBits(GPIOB, MOTOR_BIN2);
        PWM_SetDutyCycle(motor2);
    }
    else {
        GPIO_ResetBits(GPIOB, MOTOR_BIN1);
        GPIO_SetBits(GPIOB, MOTOR_BIN2);
        PWM_SetDutyCycle(-motor2);
    }
}