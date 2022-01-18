#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stdio.h"

unsigned int Counter = 0;
unsigned int min = 0;
unsigned int Led7_Font[10]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};// Ma Anode chung
int Flag = 0;

void SEG_Config(void);// cau hinh chan du lieu: a,b,c,d,...,dp
void SEG_control(void); // cau hinh chan dieu khien
void SEG_Disp(void);
void timer_Init(void);
void Delay_ms(unsigned int _time);


int main(){
	SEG_Config();
	SEG_control();
	timer_Init();
	while(1){
		SEG_Disp();
	}
}

void Delay_ms(unsigned int _time){
	volatile unsigned int i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 601; j++){}
	}
}


/*
7 chan ma hoa
a - PA0
b - PA1
c - PA2
d
e
f
g - PA6

4 chan dk
hang don vi cua giay: PB0
hang chuc cuar giay : PB1

hang don vi cua phut: PB2
hang chuc cua phut  : PB3

*/



// cau hinh chan ma hoa led
void SEG_Config(void){	
	GPIO_InitTypeDef seg_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

	seg_init.GPIO_Mode = GPIO_Mode_Out_PP;
	seg_init.GPIO_Speed = GPIO_Speed_50MHz;
	seg_init.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |GPIO_Pin_2 | GPIO_Pin_3 | 
											GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOA,&seg_init);

	GPIOA->ODR = 0xff;
}

// cau hinh chan dieu khien.
void SEG_control(void){
	GPIO_InitTypeDef seg_control;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	seg_control.GPIO_Mode = GPIO_Mode_Out_PP;
	seg_control.GPIO_Speed = GPIO_Speed_50MHz;
	seg_control.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 |GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOB,&seg_control);
	GPIOB->ODR = 0xffff;
}

void SEG_Disp(void){
	if(Counter==60){
		min++;
		if(min==60) min = 0;
		Counter = 0;
	}
	// hien thi hang don vi cua giay
	GPIO_ResetBits(GPIOB, GPIO_Pin_15);
	GPIO_Write(GPIOA,Led7_Font[Counter%10]);
	Delay_ms(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_15);
	
	// hien thi hang chuc vi cua giay
	GPIO_ResetBits(GPIOB, GPIO_Pin_14);
	GPIO_Write(GPIOA,Led7_Font[Counter/10]);
	Delay_ms(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_14);
	
	// hien thi hang don vi cua phut
	GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	GPIO_Write(GPIOA,Led7_Font[min%10]);
	Delay_ms(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_13);
	
	// hien thi hang chuc vi cua phut
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	GPIO_Write(GPIOA,Led7_Font[min/10]);
	Delay_ms(50);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

/*
function: 1s
*/
void timer_Init(void){
	TIM_TimeBaseInitTypeDef timer_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 10000 - 1;
	timer_init.TIM_Prescaler = 7200 - 1;
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&timer_init);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){		
		Counter++;
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
