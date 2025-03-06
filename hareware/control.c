#include "stm32f10x.h"  
#include "mpu6050.h"
#include "oled.h"
#include  "motor.h"

//

void EXTI9_5_IRQHandler(void){
	static int16_t ax, ay, az, gx, gy, gz;
	if(EXTI_GetITStatus(EXTI_Line5) != RESET){
		EXTI_ClearITPendingBit(EXTI_Line5);
		MPU6050_GetData(&ax,&ay, &az, &gx, &gy, &gz); 
        //TODO 控制电机
		OLED_ShowString(1, 1, "ax:");
		OLED_ShowSignedNum(1, 4, ax, 5);
		OLED_ShowString(1, 9, "ay:");
		OLED_ShowSignedNum(1, 14, ay, 5);
		OLED_ShowString(2, 1, "az:");
		OLED_ShowSignedNum(2, 4, az, 5);
		OLED_ShowString(2, 9, "gx:");
		OLED_ShowSignedNum(2, 14, gx, 5);
		OLED_ShowString(3, 1, "gy:");
		OLED_ShowSignedNum(3, 4, gy, 5);
		OLED_ShowString(3, 9, "gz:");
		OLED_ShowSignedNum(3, 14, gz, 5);

	}


}