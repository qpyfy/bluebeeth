#include "stm32f10x.h"  
#include "mpu6050.h"
#include "oled.h"
#include  "motor.h"
#include "encoder.h"
#include "bluetooth.h"

#include "eMPL/inv_mpu.h" //mpu6050库
#include "eMPL/inv_mpu_dmp_motion_driver.h" //mpu6050 dmp库

#define LPF_COEFFICIENT 0.7 //低通滤波器的比例系数
#define MED_ANGLE -0.04
#define VELOCITY_KP 250
#define VELOCITY_KI 0.6

#define BALANCE_KP 0.65
#define BALANCE_KD 0.00325

#define TURN_KP 0.6
#define TURN_KD 20

int ax, ay, az, gx, gy, gz;
float yaw, pitch, roll;
int speed = 0, turn = 0;
extern u8 direction;

int VelocityControl(int target);
int BalanceControl(int target);
int TurnControl(int target);


void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line5);
		MPU6050_GetData(&ax,&ay, &az, &gx, &gy, &gz); 
		mpu_dmp_get_data(&roll, &pitch, &yaw);
        //控制电机

		OLED_ShowSignedNum(1, 1, pitch,3);
		OLED_ShowSignedNum(2, 1, roll,3);
		OLED_ShowSignedNum(3, 1, yaw,3);
		if(direction == STOP){
			speed = 0;
			turn = 0;
		}
		else if (direction == FORWARD)
		{
			speed++;
		}
		else if (direction == BACKWARD)
		{
			speed++;
		}
		else if (direction == RIGHT)
		{
			speed = 0;
			turn = 30;
		}
		else if (direction == LEFT)
		{
			speed = 0;
			turn = -30;
		}
		
		int velocity = VelocityControl(speed);
		int pwm_out = BalanceControl(velocity+MED_ANGLE);
		int turn_out = TurnControl(turn);

		Motor_Load(pwm_out + turn_out, pwm_out - turn_out);
	}
 
}
// 速度环PI
int VelocityControl(int target){
	static int left_speed = 0, right_speed = 0;
	static int err_last = 0, err_speed = 0, err_out = 0, err_sum = 0;

	//低通滤波
	left_speed = Encoder_GetValue(1);right_speed = -Encoder_GetValue(2);
	err_speed  = (left_speed + right_speed) - target; // 速度误差
	err_out = (1 - LPF_COEFFICIENT) * err_speed + LPF_COEFFICIENT * err_last; // 低通滤波
	err_sum += err_out; // 误差积分
	if(err_sum > 10000) err_sum = 10000;
	else if (err_sum < -10000) err_sum = -10000; // 限幅
	err_last = err_out; // 保存误差

	//PI
	return (int)(VELOCITY_KP * err_out + VELOCITY_KI * err_sum); // PI控制
}

// 直立环PD
/*
@target: 目标角度
@note: yaw为当前角度, gy为当前角速度
@note: 目标角度大于当前角度时, 返回值为正, 电机正转
@note: 目标角度小于当前角度时, 返回值为负, 电机反转
*/
int BalanceControl(int target){
	//PD
	return (int)(BALANCE_KP * (pitch - target) + BALANCE_KD * (gy - 0));
}


//转向环PD
int TurnControl(int target){
	//PD
	return (int)(BALANCE_KP * (target - 0) + BALANCE_KD * (gz - 0));
}