#include "motor.h"
#include "bluetooth.h"
#include "mpu6050.h"
#include "oled.h"
#include "delay.h"
#include "eMPL/inv_mpu.h"
#include "eMPL/inv_mpu_dmp_motion_driver.h"

//蓝牙接收缓冲区
extern u8 rxbuff;
extern u8 rxflag;
extern u8 rxlen;
//MPU6050数据
extern int16_t ax, ay, az, gx, gy, gz;
extern float yaw, pitch, roll;
//72MHZ 1ms i=72000

void FloatToString(float f, char *s)
{
    
}

int main()
{  
    OLED_Init();
    Bluetooth_Init();
    Motor_Init();
    MPU6050_Init();
    mpu_dmp_init();
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

        // OLED_ShowSignedNum(1, 1, yaw, 3);
        // OLED_ShowSignedNum(2, 1, pitch, 3);
        // OLED_ShowSignedNum(3, 1, roll, 3);

        OLED_ShowSignedNum(1,1,ax,3);
        OLED_ShowSignedNum(2,1,ay,3);
        OLED_ShowSignedNum(3,1,az,3);


    }
    
}
