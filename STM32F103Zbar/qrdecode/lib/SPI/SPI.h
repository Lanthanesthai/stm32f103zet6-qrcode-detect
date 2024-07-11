#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_

//�����Գ���ʹ�õ���Ӳ��SPI�ӿ�����
//����SPIʱ���ź��Լ�SPI����д�ź����Ų��ɸ��ģ��������Ŷ����Ը���
//SPI��ʱ�����Ŷ���̶�ΪPB13
//SPI�Ķ��������Ŷ���̶�ΪPB14
//SPI��д�������Ŷ���̶�ΪPB15
 
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte);
void SPI2_Init(void);
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState);
void SPI_SetSpeed(SPI_TypeDef* SPIx,u8 SpeedSet);
#endif
