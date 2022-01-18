#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "string.h"
#include "stdio.h"

#define MAX 100

char 	vrc_Getc; 			// bien kieu char, dung de nhan du lieu tu PC gui xuong;
int		vri_Stt; 			// bien danh dau trang thai. 
int 	vri_Count = 0; 		// bien diem
char	vrc_Res[MAX]; 		//Mang kieu char vrc_Res[MAX]: mang luu tru tam thoi;


void Delay_ms(uint16_t _time);
void uart_Init(void);
void uart_SendChar(char _chr);
void uart_SendStr(char *str);
uint16_t UARTx_Getc(USART_TypeDef* USARTx);

int main(){
	uart_Init();	
	while(1){
		if(vri_Stt){
			if(strstr(vrc_Res,"batden")!=NULL){
				vri_Stt = 0;
				printf("ON-LED\n");		
				Delay_ms(200);
			}
		}
	}
}

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}

struct __FILE {
    int dummy;
};
FILE __stdout;
 
int fputc(int ch, FILE *f) {
 
    uart_SendChar(ch);
  
    return ch;
}

void uart_Init(void){
	GPIO_InitTypeDef gpio_typedef;
	USART_InitTypeDef usart_typedef;
	// enable clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	// congifgure pin Tx - A9;
	gpio_typedef.GPIO_Pin = GPIO_Pin_9;
	gpio_typedef.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_typedef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_typedef);	
	// configure pin Rx - A10;
	gpio_typedef.GPIO_Pin = GPIO_Pin_10;
	gpio_typedef.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_typedef.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_typedef);
	// usart configure
	usart_typedef.USART_BaudRate = 9600;
	usart_typedef.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usart_typedef.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	usart_typedef.USART_Parity = USART_Parity_No;
	usart_typedef.USART_StopBits = USART_StopBits_1;
	usart_typedef.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &usart_typedef);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	 /* Enable USART1 global interrupt */
	NVIC_EnableIRQ(USART1_IRQn);
	
	USART_Cmd(USART1, ENABLE);
}

void uart_SendChar(char _chr){
	USART_SendData(USART1,_chr);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)==RESET);
}

void uart_SendStr(char *str){
	while(*str != NULL){
		uart_SendChar(*str++);		
	}
}

uint16_t UARTx_Getc(USART_TypeDef* USARTx){
	return USART_ReceiveData(USARTx);
}

void USART1_IRQHandler(){
	if(USART_GetITStatus(USART1, USART_IT_RXNE)!=RESET){
		vrc_Getc = UARTx_Getc(USART1);
		if(vrc_Getc == '!'){// 13: enter; bao hieu ket thuc chuoi
			vri_Stt = 1; // dieu kien kiem tra de thuc hien cac chuong trinh theo yeu cau.
			vrc_Res[vri_Count] = NULL;
			vri_Count = 0;
		}
		else{
			vrc_Res[vri_Count] = vrc_Getc;
			//uart_SendChar(vrc_Getc);
			vri_Count++;
		}
	}
}
