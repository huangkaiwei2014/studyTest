#include "stm32f10x.h"
#include "USART.h"


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch) 
{ 
ch = ch; 
}
//重定义fputc函数 
int fputc(int Data, FILE *f)
{   
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
	   
	return Data;										  //返回一个值
}
#endif 
/** 配置USART1 **/
void USART_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStruture;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO,ENABLE);
  /** PA9 TXD **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
  /** RXD PA10 **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU ;
  GPIO_Init(GPIOA,&GPIO_InitStructure);
   /**USART 配置**/
  USART_InitStruture.USART_BaudRate = 9600;
  USART_InitStruture.USART_WordLength = USART_WordLength_8b;
  USART_InitStruture.USART_StopBits= USART_StopBits_1;
  USART_InitStruture.USART_Parity=USART_Parity_No;
  USART_InitStruture.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_InitStruture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Init(USART1,&USART_InitStruture);
  USART_Cmd(USART1, ENABLE);
}

/**发送一串数据**/
void USART_SendString(uint8_t *string,uint16_t len)
{
 uint8_t i;
 for(i=0;i<len;i++)
	 {
	 USART_SendData(USART1, *string++);
	 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);
	 }
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: USART1_Send_Byte
** 功能描述: 串口发送一个字符串
** 参数描述：Data 要发送的数据
** 作  　者: Dream
** 日　  期: 2011年6月20日
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_Byte(uint16_t Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus：得到发送状态位
														  //USART_FLAG_TXE:发送寄存器为空 1：为空；0：忙状态
	USART_SendData(USART1,Data);						  //发送一个字符
}

uint8_t USART1_Receive_Byte(void)
{ 
   	while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))); //USART_GetFlagStatus：得到接收状态位
														   //USART_FLAG_RXNE:接收数据寄存器非空标志位 
														   //1：忙状态  0：空闲(没收到数据，等待。。。)
	return USART_ReceiveData(USART1);					   //接收一个字符
}

/*** USART接收16Bit数据,将两个接收到char类型转换为Int ***/
uint16_t USART_Rec16bit(void)
{
 uint8_t a=0;
 uint8_t temp[2] ;
 uint16_t x,y;
 for(a=0;a<2;a++)
 {
  while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	 {
	  USART_ClearFlag(USART1,USART_FLAG_RXNE);
	  temp[a]=(uint8_t)USART_ReceiveData(USART1);
	  break;
	 } 
 }
x = 0xffff&(temp[0]<<8);
y=x|0Xffff&temp[1];
return(y);
}

//int GetKey (void)  {

//  while (!(USART1->SR & USART_FLAG_RXNE));

//  return ((int)(USART1->DR & 0x1FF));

//}







