#include "motor.h"
#include "bluetooth.h"
#include "mpu6050.h"
#include "oled.h"
#include "delay.h"

//蓝牙接收缓冲区
extern u8 rxbuff;
extern u8 rxflag;
extern u8 rxlen;
//MPU6050数据

//72MHZ 1ms i=72000

int main()
{  
    OLED_Init();
    Bluetooth_Init();
    Motor_Init();
    MPU6050_Init();
    delay_ms(10);
    OLED_Clear();
    while (1)
    {
        //蓝牙测试
    //    if(rxflag)
    //    {
    //     OLED_Clear();
    //     OLED_ShowString(0, 0, "RX:");
    //     OLED_ShowHexNum(0, 2, rxbuff, sizeof(rxbuff));
    //     rxflag = 0;
    //    }
    //  通过
        //电机测试
        // Motor_Load(50, 50);
        // OLED_ShowString(1, 1, "Motor Forward");
        // delay_ms(50);OLED_Clear();
        // Motor_Load(-50, -50);
        // OLED_ShowString(1, 1, "Motor Backward");
        // delay_ms(50);
        // Motor_Load(0, 0);OLED_Clear();
        // OLED_ShowString(1, 1, "Motor Stop");
        // delay_ms(50);OLED_Clear();
        // 
        //MPU6050测试
        // MPU6050_GetData(&x, &y, &z, &yaw, &pitch, &roll);
        

        // OLED_ShowString(1, 1, "Y:");
        // OLED_ShowNum(1, 3, yaw, 3);
        // OLED_ShowString(1, 7, "P:");
        // OLED_ShowNum(1, 9, pitch, 3);
        // OLED_ShowString(1, 13, "R:");
        // OLED_ShowNum(1, 15, roll, 3);
        // OLED_ShowString(2, 1, "X:");
        // OLED_ShowNum(2, 3, x, 3);
        // OLED_ShowString(2, 7, "Y:");
        // OLED_ShowNum(2, 9, y, 3);
        // OLED_ShowString(2, 13, "Z:");
        // OLED_ShowNum(2, 15, z, 3);
        // OLED_ShowString(3, 1, "T:");
        // OLED_ShowNum(3, 3, temp, 3);



    }
    
}
