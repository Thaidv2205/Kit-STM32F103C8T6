#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

volatile char vrvc_SttBlink = 0;

void led_Init(void);
void timer_Init(void);

int main(){
	led_Init();
	timer_Init();
	while(1){
		if(vrvc_SttBlink) GPIOB->ODR = 0x00;	
		else GPIOB->ODR = 0xFF;
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

/*
function: 3s
*/
void timer_Init(void){
	TIM_TimeBaseInitTypeDef timer_init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 21599;
	timer_init.TIM_ClockDivision = 0;
	timer_init.TIM_RepetitionCounter = 0;
	timer_init.TIM_Prescaler = 9999;	
	TIM_TimeBaseInit(TIM2,&timer_init);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd(TIM2,ENABLE);
}

void TIM2_IRQHandler(){
	if(TIM_GetITStatus(TIM2, TIM_IT_Update)!=RESET){		
		vrvc_SttBlink = !vrvc_SttBlink;
	}	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}