#include "bluetooth.h"

void Bluetooth_Init(void){
    Usart3_Init();
    Bluetooth_SendString("Bluetooth Init Success!\r\n");
}

void Bluetooth_SendByte(uint8_t byte){
    Usart3_SendByte(byte);
}

void Bluetooth_SendString(char* str){
    Usart3_SendString(str);
}

uint8_t Bluetooth_ReceiveByte(void){
    return Usart3_ReceiveByte();
}
