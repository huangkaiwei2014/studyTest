#include "stm32f10x.h"
#include "USART.h"


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 

//__use_no_semihosting was requested, but _ttywrch was 
_ttywrch(int ch) 
{ 
ch = ch; 
}
//�ض���fputc���� 
int fputc(int Data, FILE *f)
{   
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus���õ�����״̬λ
														  //USART_FLAG_TXE:���ͼĴ���Ϊ�� 1��Ϊ�գ�0��æ״̬
	USART_SendData(USART1,Data);						  //����һ���ַ�
	   
	return Data;										  //����һ��ֵ
}
#endif 
/** ����USART1 **/
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
   /**USART ����**/
  USART_InitStruture.USART_BaudRate = 9600;
  USART_InitStruture.USART_WordLength = USART_WordLength_8b;
  USART_InitStruture.USART_StopBits= USART_StopBits_1;
  USART_InitStruture.USART_Parity=USART_Parity_No;
  USART_InitStruture.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_InitStruture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Init(USART1,&USART_InitStruture);
  USART_Cmd(USART1, ENABLE);
}

/**����һ������**/
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
** ��������: USART1_Send_Byte
** ��������: ���ڷ���һ���ַ���
** ����������Data Ҫ���͵�����
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void USART1_Send_Byte(uint16_t Data)
{ 
	while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));	  //USART_GetFlagStatus���õ�����״̬λ
														  //USART_FLAG_TXE:���ͼĴ���Ϊ�� 1��Ϊ�գ�0��æ״̬
	USART_SendData(USART1,Data);						  //����һ���ַ�
}

uint8_t USART1_Receive_Byte(void)
{ 
   	while(!(USART_GetFlagStatus(USART1,USART_FLAG_RXNE))); //USART_GetFlagStatus���õ�����״̬λ
														   //USART_FLAG_RXNE:�������ݼĴ����ǿձ�־λ 
														   //1��æ״̬  0������(û�յ����ݣ��ȴ�������)
	return USART_ReceiveData(USART1);					   //����һ���ַ�
}

/*** USART����16Bit����,���������յ�char����ת��ΪInt ***/
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







