#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

/*
function: duty 50%;
*/

#define DUTY 10000 // 10000/20000 * 100(%) = 50%

void led_Init(void);
void pwm_Init(void);

/*

TIM3: CH1 - PA6
			CH2 - PA7
			CH3 - PB0
			CH4 - PB1
			
TIM4: CH1 - PB6 - OC1
			CH2 - PB7 - OC2
			CH3 - PB8 - OC3
			CH4 - PB9 - OC4

0 : sang max;
20000: off;
*/

int main(){
	uint16_t vrui_pwmval=200;
	uint8_t vrui_key=1;	
	
	led_Init();
	pwm_Init();	
	while(1){
	}
}


void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_AF_PP;
	led_init.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&led_init);
	
}

void pwm_Init(void){
	
	TIM_TimeBaseInitTypeDef timer_init;
	TIM_OCInitTypeDef				pwm_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	timer_init.TIM_CounterMode = TIM_CounterMode_Up;
	timer_init.TIM_Period = 19999;// 2s
	timer_init.TIM_Prescaler = 7199;
	TIM_TimeBaseInit(TIM4,&timer_init);
	
	
	/*chon che do hoat dong cho chuc nang Ouput Compare*/
	pwm_init.TIM_OCMode = TIM_OCMode_PWM1;
//	/*lenh cau hinh muc tich cuc cua Output Compare doi voi cac chan CHx*/
	pwm_init.TIM_OCPolarity = TIM_OCPolarity_High;
//	/*lenh cho phep hoac khong cho phep Output Compare xuat tin hieu ra chan CHx*/
	pwm_init.TIM_OutputState = TIM_OutputState_Enable;
//	/*lenh cau hinh gia tri so sanh cho Output Compare*/
	pwm_init.TIM_Pulse = DUTY;
//	
	TIM_OC1Init(TIM4,&pwm_init);
	TIM_OC2Init(TIM4,&pwm_init);
	/*cho phep xuat tin hieu o tim4.*/
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}


