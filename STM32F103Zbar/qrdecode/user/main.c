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

extern u8 ov_sta;	//��exit.c���涨��
uint8_t colorQR;

uint8_t data_buf[240*240] __attribute__((at(0x68106f80)));


void My_EXTI_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);//ѡ��GPIO�ܽ������ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��GPIO�ܽ������ⲿ�ж���·
	
	
	//EXTI0 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//EXTI0�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//EXTI2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	//EXTI3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//EXTI3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	//EXTI4 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//EXTI4�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
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


u8 ov_sta;	//֡�жϱ��
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
	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;					//�����ȼ�0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���   
}

void EXTI9_5_IRQHandler(void)
{		 		
	if(EXTI_GetITStatus(EXTI_Line7)==SET)	//��8�ߵ��ж�
	{      
		OV7670_WRST=0;	//��λдָ��		  		 
		OV7670_WRST=1;	
		OV7670_WREN=1;	//����д��FIFO 	 
		ov_sta++;		//֡�жϼ�1 
	}
	EXTI_ClearITPendingBit(EXTI_Line7);  //���EXTI8��·����λ						  
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

	if(ov_sta)//��֡�жϸ��£�
	{
		
		//LCD_Set_Window((tftlcd_data.width-320)/2,(tftlcd_data.height-240)/2,320,240-1);//����ʾ�������õ���Ļ����
//		LCD_Set_Window(0,(tftlcd_data.height-240)/2,320-1,240-1);//����ʾ�������õ���Ļ����
		LCD_direction(1);
		LCD_SetWindows(0,0,320-1,240-1);
		
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
//		for(i=0;i<240;i++)   //���ַ�ʽ���Լ����κβ���,�����ٶȺ���
//		{
//			for(j=0;j<320;j++)
//			{
//				OV7670_RCK_L;
//				color=GPIOF->IDR&0XFF;	//������
//				OV7670_RCK_H; 
//				color<<=8;  
//				OV7670_RCK_L;
//				color|=GPIOF->IDR&0XFF;	//������
//				OV7670_RCK_H; 
//				LCD_DrawFRONT_COLOR(i,j,color);
//			}
//		}
			    u32 a = 0;
					
		for(j=0;j<76800;j++)   //���ַ�ʽ�����TFT�ڲ���ʾ������ƼĴ���ֵ �ٶȽϿ�
		{
			OV7670_RCK_L;
			color=GPIOF->IDR&0XFF;	//������
			OV7670_RCK_H; 
			color<<=8;  
			OV7670_RCK_L;
			color|=GPIOF->IDR&0XFF;	//������
			OV7670_RCK_H;
			
			//��ֵ��
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


        // ����Ƿ��ڷ�Χ��
        if ((j <= 239) || (j >= 320 && ((j - 320) % 320 <= 239))) {
            // �ڷ�Χ�ڣ�ִ����Ĳ���
           data_buf[a] = colorQR;
				   a++;
					Lcd_WriteData_16Bit(color);
					//Lcd_WriteData_16Bit(0x0000);
        }
				else {Lcd_WriteData_16Bit(0xffff);}//��ɫ

			 
			//printf("%x  ",colorQR);
			//if(j%20==0)printf("\r\n");
			//delay_us(50);
		}  

	} 
}


int Zbar_Test(int width, int height);



int main(void){
  GPIO_Config();//LED GPIO ��ʼ��
	USART_Config();

	Systick_Config(72);
  Buzzle_Init();

	uint16_t width=240;
	uint8_t height = 240;
	
		int i=0;
	SystemInit();//��ʼ��RCC ����ϵͳ��ƵΪ72MHZ
	LCD_Init();	   //Һ������ʼ��
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
		FSMC_SRAM_Init();//��ʼ��FSMC
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ڲ��ڴ�� �ڲ��ڴ�ز�����
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
//            // �ڷ�Χ�ڣ�ִ����Ĳ���
//            //data_buf[a] = colorQR;
//					//data_buf[a] = data_buff[a];
//				   // a++;
//        }
//				else {Lcd_WriteData_16Bit(0xffff);}
//		}
			//u32 i = LCD_ReadPoint(100,100);
		//printf("%x",i);//��ɫ
		//Serial_SendString2("hhh");
		delay_ms(200);
		LED4_OFF();
		LED5_OFF();
		buzzle6_OFF();
		//for(uint32_t a=0;a<320*240;a++){data_buf[a] = data_buf[a];}
		Zbar_Test(width,height);

		EXTI_ClearITPendingBit(EXTI_Line7);//����֡�жϱ��
 		ov_sta=0;					//��һ��ɨ��
		
		delay_ms(100);
	  //GPIO_SetBits(GPIOC,LED_PIN);
		camera_refresh();//������ʾ
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
    printf("ʶ��Ķ�ά�����Ϊ = %d\r\n", n);
    /* extract results */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for(; symbol; symbol = zbar_symbol_next(symbol)) {
        /* do something useful with results */
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
        const char *data = zbar_symbol_get_data(symbol);
        printf("ʶ��Ϊ %s ����� \"%s\"\r\n",
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