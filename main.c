#include "stm32f10x.h"
#include <stdio.h>
#include "stm32f10x_conf.h"
#include "USART.h"
#include "R61505.h"
#include "spiflash.h"
#include "sdio.h"
#include "ff.h"
#include "diskio.h"
//#include "bmpa.h"
#include "gps_config.h"



void NVIC_Configuration(void);
void GPS_UART_Config(void);
extern void nmea_decode_test(void);

/***===== 变量 结够体参数定义 ====**/
//    FATFS fs;            // Work area (file system object) for logical drive
////    FIL fsrc, fdst,fap;      // file objects
//    BYTE buffer[14]=""; // file copy buffer
//    FRESULT res;         // FatFs function common result code
//    FIL file;
//	  UINT br, bw;         // File R/W count
//    DIR dirs;
__IO	uint32_t TimingDelay;
//extern SD_Error Status;
//extern SD_CardInfo SDCardInfo;
//unsigned char dat[720]="";

/**==== 初始化SYSTICK时钟 2013年4月6日21:51:23 =====	**/
void Init_SysTick(void)
{
 if(SysTick_Config(SystemCoreClock/1000) )	/** 1/1000(s)=0.001s=1ms **/
 {
  while(1);
 }
}

/**==== 精确延时1ms函数 2013年4月6日21:57:07 =====	**/
void Delayms(__IO uint32_t nTime)
{
 TimingDelay = nTime;
 while(TimingDelay !=0);
}


/**==== 主函数入口 ====**/
int main(void)
{
 char str[68]="";
//WORD deviceID=0x0000;

	
Init_SysTick();
USART_Config();

//NVIC_Configuration();	
//R61505_Initializtion();
GPS_UART_Config();
GPS_Config();
GPIO_SetBits(GPIOA,GPIO_Pin_1);
R61505_Clear(Green);
	while(1)
	{	
		printf("%s", gps_rbuff);
//		printf("%s",NEMAstr);
//		while( Flag_GPS_OK==1)
//		{
//			sprintf(str,"%s",NEMAstr);
//			GUI_Text(0,0, (unsigned char)str,strlen(str), 0x0000,0x00ff);
//		} 	
//		nmea_decode_test();
	}
return 0;
}

/********************************************************************************/
void GPS_UART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStruture;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//UART23  APB1
  /** PA2 TXD **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  /** RXD PA3 **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU ;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
   /**USART 配置**/
  USART_InitStruture.USART_BaudRate = 9600;
  USART_InitStruture.USART_WordLength = USART_WordLength_8b;
  USART_InitStruture.USART_StopBits= USART_StopBits_1;
  USART_InitStruture.USART_Parity=USART_Parity_No;
  USART_InitStruture.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_InitStruture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Init(USART2,&USART_InitStruture);
	USART_Cmd(USART2, ENABLE);

}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/** End of This File **/
