//OV7670摄像头模块接线说明：

//D0-D7：PF0-PF7
//WEN：PB6
//RCLK：PB7
//VSYNC：PA7
//SDA：PC6
//SCL：PC4
//RRST：PC2
//OE：PC3
//WRST：PE6
//GND：GND
//3.3V：3.3V

#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"

 
#define OV7670_VSYNC  	PAin(7)			//同步信号检测IO
#define OV7670_WRST		PEout(6)		//写指针复位
#define OV7670_WREN		PBout(6)		//写入FIFO使能
#define OV7670_RCK_H	GPIO_SetBits(GPIOB,GPIO_Pin_7)//GPIOB->BSRRH=1<<7////设置读数据时钟高电平
#define OV7670_RCK_L	GPIO_ResetBits(GPIOB,GPIO_Pin_7)//GPIOB->BSRRL=1<<7//	//设置读数据时钟低电平
#define OV7670_RRST		PCout(2)  		//读指针复位
#define OV7670_CS		PCout(3)  		//片选信号(OE)

#define OV7670_DATA   GPIOF->IDR&0x00FF  					//数据输入端口
/////////////////////////////////////////									
	    				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);


#endif





















