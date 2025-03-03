

#include "usart3.h"
#include "oled.h"
#include "motor.h"
#include "bluetooth.h"
extern u8 rxbuff;
extern u8 rxflag;


//72MHZ 1ms i=72000
void delay_ms(uint32_t x){
    while(x--){
        for (uint8_t i = 0; i < 72; i++)
        {
            for (uint8_t j = 0; j < 1000; j++)
            {
                
            }
            
        }
        
    }
}

int main()
{
    OLED_Init();
    Bluetooth_Init();
    Motor_Init();
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
        //电机测试
        Motor_Load(100, 100);
        OLED_ShowString(0, 0, "Motor Forward");
        delay_ms(50);OLED_Clear();
        Motor_Load(-100, -100);
        OLED_ShowString(0, 0, "Motor Backward");
        delay_ms(50);
        Motor_Load(0, 0);OLED_Clear();
        OLED_ShowString(0, 0, "Motor Stop");
        delay_ms(50);OLED_Clear();
    }
    
}
