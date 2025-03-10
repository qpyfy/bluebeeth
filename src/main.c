#include "motor.h"
#include "bluetooth.h"
#include "mpu6050.h"
#include "encoder.h"
#include "mpu6050_exit.h"
#include "mpu6050_reg.h"
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
    OLED_ShowString(1, 1, "OLED Test");
    Bluetooth_Init();
    OLED_ShowString(1, 1, "Bluetooth Test");
    Motor_Init();
    OLED_ShowString(1, 1, "Motor Test");
    MPU6050_Init();
    OLED_ShowString(1, 1, "MPU6050 Test");
    mpu_dmp_init();
    OLED_ShowString(1, 1, "DMP Test");
    delay_ms(10);
    Encoder_Init();
    OLED_Clear();    
    MPU6050_Exit_Init();
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
        // //MPU6050测试
        // MPU6050_GetData(&ax, &ay, &az, &gx, &gy, &gz);
        // OLED_ShowSignedNum(1,1,ax,4);
        // OLED_ShowSignedNum(2,1,ay,4);
        // OLED_ShowSignedNum(3,1,az,4);

        // MPU_Get_Gyroscope(&gx, &gy, &gz);
        // OLED_ShowSignedNum(1, 1, gx, 4);
        // OLED_ShowSignedNum(2, 1, gy, 4);
        // OLED_ShowSignedNum(3, 1, gz, 4);

        // mpu_dmp_get_data(&pitch, &roll, &yaw);
        // OLED_ShowSignedNum(1, 1, pitch, 3);
        // OLED_ShowSignedNum(2, 1, roll, 3);
        // OLED_ShowSignedNum(3, 1, yaw, 3);

   
        // u8 id = MPU6050_ReadReg(MPU_DEVICE_ID_REG);
        // u8 stat = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
        // OLED_ShowNum(1, 1, res, 4);


    }
    
}
