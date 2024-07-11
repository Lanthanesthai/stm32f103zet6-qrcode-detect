#include "init.h"

static u8 fac_us= 0;
static u16 fac_ms= 0;
void Systick_Config(u8 coreFre){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = coreFre/8;
	fac_ms = (u16)fac_us* 1000;
}
void delay_us(u32 nus){
  u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}
void delay_ms(u32 nms){
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	
}


void GPIO_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_initStructrue;
	GPIO_initStructrue.GPIO_Pin=LED_PIN;
	GPIO_initStructrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_initStructrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_initStructrue);
	GPIO_SetBits(GPIOC,LED_PIN);
}
int fputc(int ch , FILE * f){//重写这个函数,使得能够使用printf函数
	USART1->DR = (u8)ch;
	while((USART1->SR&0X40)==0);
	return ch;
}

//串口
void USART_Config(){
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);  //PA2 TX PA3 RX
	
  USART_InitTypeDef UI;//声明 串口初始化结构体
	UI.USART_BaudRate = 115200;//波特率
	UI.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;
	UI.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	UI.USART_Parity = USART_Parity_No;
	UI.USART_StopBits = USART_StopBits_1;
	UI.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&UI);

	GPIO_InitTypeDef GPIO_InitStructure;//声明 GPIO初始化结构体
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
//	
//	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//打开中断
	USART_Cmd(USART1,ENABLE);//使能串口2
}


//esp32
void USART2_Init(void) {
    // 1. Enable the clock for USART2 and GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2. Configure PA2 as USART2 TX (Alternate Function Push Pull)
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configure PA3 as USART2 RX (Input, Floating)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 4. Configure USART2 settings
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART2, &USART_InitStructure);

    // 5. Enable USART2
    USART_Cmd(USART2, ENABLE);
}


void Serial_SendByte2(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray2(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte2(Array[i]);
	}
}

void Serial_SendString2(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte2(String[i]);
	}
}



//蓝牙
void UART4_Init(void) {
    // 1. Enable the clock for USART2 and GPIOA
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 2. Configure PA2 as USART2 TX (Alternate Function Push Pull)
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 3. Configure PA3 as USART4 RX (Input, Floating)
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // 4. Configure USART2 settings
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART4, &USART_InitStructure);

    // 5. Enable USART2
    USART_Cmd(UART4, ENABLE);
}


void Serial_SendByte4(uint8_t Byte)
{
	USART_SendData(UART4, Byte);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TXE) == RESET);
}

void Serial_SendArray4(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)
	{
		Serial_SendByte4(Array[i]);
	}
}

void Serial_SendString4(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)
	{
		Serial_SendByte4(String[i]);
	}
}

