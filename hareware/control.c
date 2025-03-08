#include "stm32f10x.h"  
#include "mpu6050.h"
#include "oled.h"
#include  "motor.h"
#include "encoder.h"

#define LPF_COEFFICIENT 0.7 //低通滤波器的比例系数
#define VELOCITY_KP 0.5
#define VELOCITY_KI 0.5


int16_t ax, ay, az, gx, gy, gz;

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line5);
		MPU6050_GetData(&ax,&ay, &az, &gx, &gy, &gz); 
        //TODO 控制电机

	}
 
}
// 速度环PI
u16 VelocityControl(u16 target){
	static u16 left_speed = 0, right_speed = 0;
	static u16 err_last = 0, err_speed = 0, err_out = 0, err_sum = 0;
	left_speed = Encoder_GetValue(1);right_speed = Encoder_GetValue(2);

	err_speed  = (left_speed + right_speed) - target; // 速度误差

	err_out = (1 - LPF_COEFFICIENT) * err_speed + LPF_COEFFICIENT * err_last; // 低通滤波

	err_sum += err_out; // 误差积分
	if(err_sum > 1000) err_sum = 1000;
	else if (err_sum < -1000) err_sum = -1000; // 限幅

	err_last = err_out; // 保存误差

	return (u16)(VELOCITY_KP * err_out + VELOCITY_KI * err_sum); // PI控制
}

// 直立环PD
u16 BalanceControl(u16 target){


}