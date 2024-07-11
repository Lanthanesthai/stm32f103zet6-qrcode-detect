#include "stm32f10x.h"                  // Device header
#include "LED.h"
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_0 |  GPIO_Pin_5 | GPIO_Pin_4| GPIO_Pin_6);
}

//PA0ÂÌµÆ£¬PA4»ÆµÆ£¬PA5À¶µÆ£¬PA6À®°È
//ledÒ»±ß½Ó3v3£¬Ò»±ß½Óio¿Ú
void LED5_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}

void LED5_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}

void LED5_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_5) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}

void LED4_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void LED4_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void LED4_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	}
}

void buzzle6_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

void buzzle6_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_6);
}

void LED0_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}

void LED0_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
}
