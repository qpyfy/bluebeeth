#include "usart3.h"



void Usart3_Init(void){
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitTypeDef USART_InitStructure;
    USART_StructInit(&USART_InitStructure);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No; 
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_EnableIRQ(USART3_IRQn);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //响应优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    USART_Cmd(USART3, ENABLE);

}

void Usart3_SendByte(uint8_t byte){
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3, byte);
}

void Usart3_SendString(char* str){
    while(*str){
        Usart3_SendByte(*str++);
    }
}

uint8_t Usart3_ReceiveByte(void){
     return rxdata[0];
}

#define BUFF_SIZE 128

u8 rxdata[BUFF_SIZE];
u8 rxbuff[BUFF_SIZE];
u8 rxlen = 0;



void ClearBuff(void){
    for (uint8_t i = 0; i < rxlen; i++)
    {
        rxbuff[i] = 0;
    }
    rxlen = 0;
    
}
/*
@note 用于处理蓝牙接收到的数据
*/
void USART3_IRQHandler(void){
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
        u8 data = USART_ReceiveData(USART3);
        if(data != '\n'){
            rxbuff[rxlen++] = data;
        }
        else {
            for (uint8_t i = 0; i < rxlen; i++)
            {
                rxdata[i] = rxbuff[i];
            }
            rxlen = 0;
            ClearBuff();
        }
        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}

