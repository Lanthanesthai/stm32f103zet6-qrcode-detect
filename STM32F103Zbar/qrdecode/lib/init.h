#ifndef _INIT_H
#define _INIT_H

#include "stm32f10x.h"
#include "stdio.h"
#define LED_PIN GPIO_Pin_0|GPIO_Pin_1

void GPIO_Config();
void Systick_Config(u8 coreFre );
void delay_us(u32 nus);
void delay_ms(u32 nms);
void USART_Config();
void USART2_Init(void);
void Serial_SendByte2(uint8_t Byte);
void Serial_SendArray2(uint8_t *Array, uint16_t Length);
void Serial_SendString2(char *String);
void UART4_Init(void);
void Serial_SendByte4(uint8_t Byte);
void Serial_SendArray4(uint8_t *Array, uint16_t Length);
void Serial_SendString4(char *String);
#endif
