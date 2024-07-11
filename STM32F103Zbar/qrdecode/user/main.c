#include "main.h"
#include "stm32f10x.h"
#include "my_stm32f10x_exti.h"
#include "zbar.h"
#include "malloc.h"
#include <string.h>
#include "stdint.h"
#include "sram.h"
#include "sys.h"
#include "lcd.h"
#include "ov7670.h"
#include "time.h"
#include "exti.h"
#include "buzzle.h"
#include "LED.h"

extern u8 ov_sta;	//在exit.c里面定义
uint8_t colorQR;

uint8_t data_buf[240*240] __attribute__((at(0x68106f80)));


void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//选择GPIO管脚用作外部中断线路
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//选择GPIO管脚用作外部中断线路
	
	
	//EXTI0 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	//EXTI2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	//EXTI3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	//EXTI4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line2|EXTI_Line3|EXTI_Line4; 
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
}


u8 ov_sta;	//帧中断标记
void EXTI7_Init(void)
{												  
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
 	   	 
	EXTI_InitStructure.EXTI_Line=EXTI_Line7;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占优先级0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//子优先级0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器   
}

void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line7)==SET)	//是8线的中断
	{      
		OV7670_WRST=0;	//复位写指针		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//允许写入FIFO 	 
		ov_sta++;		//帧中断加1 
	}
	EXTI_ClearITPendingBit(EXTI_Line7);  //清除EXTI8线路挂起位						  
} 


void delay(int x){
	uint32_t i = 1 *x;
	while(i--);
}


void camera_refresh(void)
{
	u32 j;
	u16 i;
 	u16 color;

	if(ov_sta)//有帧中断更新？
	{
		
		//LCD_Set_Window((tftlcd_data.width-320)/2,(tftlcd_data.height-240)/2,320,240-1);//将显示区域设置到屏幕中央
//		LCD_Set_Window(0,(tftlcd_data.height-240)/2,320-1,240-1);//将显示区域设置到屏幕中央
		LCD_direction(1);
		LCD_SetWindows(0,0,320-1,240-1);
		
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK_H;
//		for(i=0;i<240;i++)   //此种方式可以兼容任何彩屏,但是速度很慢
//		{
//			for(j=0;j<320;j++)
//			{
//				OV7670_RCK_L;
//				color=GPIOF->IDR&0XFF;	//读数据
//				OV7670_RCK_H; 
//				color<<=8;  
//				OV7670_RCK_L;
//				color|=GPIOF->IDR&0XFF;	//读数据
//				OV7670_RCK_H; 
//				LCD_DrawFRONT_COLOR(i,j,color);
//			}
//		}
			    u32 a = 0;
					
		for(j=0;j<76800;j++)   //此种方式需清楚TFT内部显示方向控制寄存器值 速度较快
		{
			OV7670_RCK_L;
			color=GPIOF->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOF->IDR&0XFF;	//读数据
			OV7670_RCK_H;
			
			//二值化
			if(color > 0x4500)
			{
				color = 0xffff;
				colorQR = 0xff;
			}
			else
			{
				color = 0x0000;
			  colorQR = 0x00;
			}


        // 检查是否在范围内
        if ((j <= 239) || (j >= 320 && ((j - 320) % 320 <= 239))) {
            // 在范围内，执行你的操作
           data_buf[a] = colorQR;
				   a++;
					Lcd_WriteData_16Bit(color);
					//Lcd_WriteData_16Bit(0x0000);
        }
				else {Lcd_WriteData_16Bit(0xffff);}//白色

			 
			//printf("%x  ",colorQR);
			//if(j%20==0)printf("\r\n");
			//delay_us(50);
		}  

	} 
}


int Zbar_Test(int width, int height);



int main(void){
  GPIO_Config();//LED GPIO 初始化
	USART_Config();

	Systick_Config(72);
  Buzzle_Init();

	uint16_t width=240;
	uint8_t height = 240;
	
		int i=0;
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	LCD_Init();	   //液晶屏初始化
	LCD_Clear(0xffff);
	LCD_Clear(BLUE);
	i=OV7670_Init();
	delay_ms(1500);	
	 OV7670_Light_Mode(0);
	 OV7670_Color_Saturation(2);
	 OV7670_Brightness(2);
	 OV7670_Contrast(2);
 	 OV7670_Special_Effects(0);
	 	EXTI7_Init();		
		  	OV7670_CS=0;	
	LCD_Clear(BLACK);
	delay_ms(1500);	
	UART4_Init();
		USART2_Init();
	UART4_Init();
	  LED_Init();
		FSMC_SRAM_Init();//初始化FSMC
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化内部内存池 内部内存池不够用
int p = 0;


	while(1){
		//Serial_SendString2("SSS");
						LED0_ON();

		        //Buzzle_ON();
						//delay_ms(2000);
						//Buzzle_OFF();
						//LED0_OFF();
		

//		for(u32 j=0;j<76800;j++) 
//		{
//			   if ((j <= 239) || (j >= 320 && ((j - 320) % 320 <= 239))) {
//            // 在范围内，执行你的操作
//            //data_buf[a] = colorQR;
//					//data_buf[a] = data_buff[a];
//				   // a++;
//        }
//				else {Lcd_WriteData_16Bit(0xffff);}
//		}
			//u32 i = LCD_ReadPoint(100,100);
		//printf("%x",i);//白色
		//Serial_SendString2("hhh");
		delay_ms(200);
		LED4_OFF();
		LED5_OFF();
		buzzle6_OFF();
		//for(uint32_t a=0;a<320*240;a++){data_buf[a] = data_buf[a];}
		Zbar_Test(width,height);

		EXTI_ClearITPendingBit(EXTI_Line7);//清零帧中断标记
 		ov_sta=0;					//下一次扫描
		
		delay_ms(100);
	  //GPIO_SetBits(GPIOC,LED_PIN);
		camera_refresh();//更新显示
	}
  
}

int Zbar_Test( int width, int height)
{
	  int a;
	  void *raw = (void* )data_buf;

	  zbar_image_scanner_t *scanner = NULL;
    /* create a reader */
    scanner = zbar_image_scanner_create();

    /* configure the reader */
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);


    /* wrap image data */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int*)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, raw, width * height, zbar_image_free_data);

    /* scan the image for barcodes */
    int n = zbar_scan_image(scanner, image);
		if(n==0){LED4_ON();}
		else{LED5_ON();}
    printf("识别的二维码个数为 = %d\r\n", n);
    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        printf("识别为 %s 结果是 \"%s\"\r\n",
               zbar_get_symbol_name(typ), data);
			if (strcmp((char *)data, "sos") == 0){
			buzzle6_ON();
				//buzzle6_OFF();
			}
			Serial_SendString2((char* )data);
			Serial_SendString4((char* )data);
    }

    /* clean up */
    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return(0);
} 	