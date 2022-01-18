#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "uart.h"

int str[]={9,7,4,5,9,7,4,5,6,4,2,1,3,0,6,4,2,1,3,0,9,7,4,5,9,7,4,5,6,4,2,1,3,0,6,4,2,1,3,0};	

volatile char vrvc_SttBlink = 0;

void led_Init(void);
void timer_Init(void);
void Delay_ms(unsigned int time);
void swap(int *x, int *y);
void xuly(int str[]);
	
int main(){
	uint16_t timer = 0;
	led_Init();
	uart_Init();
	timer_Init();
	while(1){
		TIM_Cmd(TIM2,ENABLE);
		TIM_SetCounter(TIM2,0);
		xuly(str);
		timer = TIM_GetCounter(TIM2);
		TIM_Cmd(TIM2, DISABLE);
		printf("time: %0.2f(us)\n",(float)timer/36);
		printf("time: %d\n",timer);
		Delay_ms(100);
	}
}

void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	led_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
											GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&led_init);
	
}

///*
//function: 1ms
//timer: 1000 = 1ms
//*/
//void timer_Init(void){
//	TIM_TimeBaseInitTypeDef timer_init;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
//	timer_init.TIM_Period = 65535;
//	timer_init.TIM_Prescaler = 71;
//	timer_init.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM2,&timer_init);
//}

/*
function: 1us 
timer: 36 = 1us
*/
void timer_Init(void){
	TIM_TimeBaseInitTypeDef timer_init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 65535;
	timer_init.TIM_Prescaler = 1;
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&timer_init);
}


void Delay_ms(unsigned int time){
	uint16_t i,j;
	for(i = 0; i < time; i++){
		for(j = 0; j < 0x2aff; j++){}
	}
}

void swap(int *x, int *y){
  int temp = *x;
  *x=*y;
  *y = temp;
}

void xuly(int str[]){
  int i,j; 

  for(i=0;i<40-1;i++){
    for(j=i+1;j<40;j++){
      if(str[i]>str[j]){
        swap(&str[i],&str[j]);
      }
    }
  }
}