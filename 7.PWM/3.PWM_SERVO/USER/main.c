#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void Delay_ms(uint16_t _time);
void led_Init(void);
void pwm_Init(void);
void pin_pwm_Init(void);
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
uint16_t analogWrite(uint16_t _Duty);
/*
TIM1: CH1 - PA8
			CH2 - PA9
			CH3 - PA10
			CH4 - PA11

TIM2: CH1 - PA0
			CH2 - PA1
			CH3 - PA2
			CH4 - PA3

TIM3: CH1 - PA6
			CH2 - PA7
			CH3 - PB0
			CH4 - PB1
			
TIM4: CH1 - PB6
			CH2 - PB7
			CH3 - PB8
			CH4 - PB9
*/

int main(){

	led_Init();
	pwm_Init();	
	while(1){

//		TIM2->CCR4 = 500; 	// 0.5ms: 0 do
//		Delay_ms(1000);				
//		TIM2->CCR4 = 1000; 	// 1ms	: 45 do
//		Delay_ms(1000);
//		TIM2->CCR4 = 1500;	// 1.5ms: 90 do
//		Delay_ms(1000);
//		TIM2->CCR4 = 2000;	// 2ms	: 135 do
//		Delay_ms(1000);
//		TIM2->CCR4 = 2500;	// 2.5ms: 180 do
//		Delay_ms(1000);
		analogWrite(25);
		Delay_ms(1000);
		analogWrite(100);
		Delay_ms(1000);

	}
}


void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}

uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t analogWrite(uint16_t _Duty){
	return TIM2->CCR4 = map(_Duty,0,180,500,2500);
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

void pin_pwm_Init(void){
	GPIO_InitTypeDef pin_pwm;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
	/* TIM2 - Channel_4 - PA3 */
	pin_pwm.GPIO_Mode = GPIO_Mode_AF_PP;
	pin_pwm.GPIO_Pin = GPIO_Pin_3;
	pin_pwm.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&pin_pwm);	
}

void pwm_Init(void){
	
	TIM_TimeBaseInitTypeDef timer_init;
	TIM_OCInitTypeDef				pwm_init;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	pin_pwm_Init();
	
	timer_init.TIM_Period				 		= 19999;
	timer_init.TIM_Prescaler 		 		= 71;
	timer_init.TIM_CounterMode 	 		= TIM_CounterMode_Up;
	timer_init.TIM_ClockDivision 		= 0;
	timer_init.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&timer_init);
	
	
	/*chon che do hoat dong cho chuc nang Ouput Compare*/
	pwm_init.TIM_OCMode = TIM_OCMode_PWM1;
	/*lenh cau hinh muc tich cuc cua Output Compare doi voi cac chan CHx*/
	pwm_init.TIM_OCPolarity = TIM_OCPolarity_High;
	/*lenh cho phep hoac khong cho phep Output Compare xuat tin hieu ra chan CHx*/
	pwm_init.TIM_OutputState = TIM_OutputState_Enable;
	/*lenh cau hinh gia tri so sanh cho Output Compare*/
	pwm_init.TIM_Pulse = 0;

	TIM_OC4Init(TIM2,&pwm_init);
	/*cho phep xuat tin hieu o tim4.*/
	TIM_CtrlPWMOutputs(TIM2,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}


