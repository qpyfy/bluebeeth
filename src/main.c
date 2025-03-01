

#include "usart3.h"
#include "oled.h"
#include "motor.h"

extern u8 rxdata;
extern u8 rxflag;


//72MHZ 1ms i=72000
void delay_ms(uint8_t x){
    while(x--){
        for (size_t i = 0; i < 72; i++)
        {
            for (size_t j = 0; j < 1000; j++)
            {
                
            }
            
        }
        
    }
}

int main()
{
    OLED_Init();
    Usart3_Init();
    Motor_Init();
    Usart3_SendString("Hello World!\n");

    OLED_Clear();
    while (1)
    {
        //蓝牙测试
       if(rxflag)
       {
        OLED_Clear();
        OLED_ShowString(0, 0, "RX:");
        OLED_ShowHexNum(0, 2, rxdata, sizeof(rxdata));
        rxflag = 0;
       }
        //电机测试
        // Motor_Load(100, 100);
        // delay(1000);
        // Motor_Load(-100, -100);
        // delay(1000);
        // Motor_Load(0, 0);
        // delay(1000);
    }
    
}
