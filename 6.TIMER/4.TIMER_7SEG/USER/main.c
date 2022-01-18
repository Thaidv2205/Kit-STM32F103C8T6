#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "algorithm.h"

unsigned int vrui_DisValue = 0;
void led_Init(void);
void timer_Init(void);
void ledcontrol_Init(void);
	
int main(){
	led_Init();
	ledcontrol_Init();
	timer_Init();
	while(1){
		vrui_DisValue++;
		Delay_ms(1000);
		if(vrui_DisValue>9999) vrui_DisValue=0;

	}
}

void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	led_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
											GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&led_init);
	
	//GPIOA->ODR = 0XFF;
	
}

void ledcontrol_Init(void){
	GPIO_InitTypeDef button_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	button_init.GPIO_Mode = GPIO_Mode_Out_PP;
	button_init.GPIO_Pin = LED_NGHIN|LED_TRAM|LED_CHUC|LED_DONVI;
	button_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&button_init);
}

void timer_Init(void){
	TIM_TimeBaseInitTypeDef timer_init;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 35999;
	timer_init.TIM_Prescaler = 1;
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4,&timer_init);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
			Display(vrui_DisValue);
	}
}
