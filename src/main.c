

#include "usart3.h"
#include "oled.h"

extern u8 rxdata;
extern u8 rxflag;

void delay(int x)
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < 1000; j++)
            __NOP();
    }
}

int main()
{
    OLED_Init();
    Usart3_Init();
    Usart3_SendString("Hello World!\n");

    OLED_Clear();
    while (1)
    {
       if(rxflag)
       {
        OLED_ShowString(0, 0, "RX:");
        OLED_ShowHexNum(0, 2, rxdata);
        rxflag = 0;
       }
    }
    
}
