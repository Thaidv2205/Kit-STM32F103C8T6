#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

#define LED_DONVI GPIO_Pin_15
#define LED_CHUC 	GPIO_Pin_14
#define LED_TRAM	GPIO_Pin_13
#define LED_NGHIN GPIO_Pin_12

unsigned int NUM[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void Delay_ms(uint16_t _time);
void led_Init(void);
void ledcontrol_Init(void);
void led_Counter1(void);
int main(){
	led_Init();
	ledcontrol_Init();
	while(1){
		led_Counter1();
	}
}

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	led_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
											GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&led_init);
	
	GPIOA->ODR = 0XFF;
}

void ledcontrol_Init(void){
	GPIO_InitTypeDef button_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	
	button_init.GPIO_Mode = GPIO_Mode_Out_PP;
	button_init.GPIO_Pin = LED_NGHIN|LED_TRAM|LED_CHUC|LED_DONVI;
	button_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&button_init);
	GPIOB->ODR = 0xffff;
}

void led_Counter1(void){
	uint16_t i,j;
	for(i=0; i<10000; i++){
		for(j=0; j<20; j++){
			/*quet led hang don vi*/
			GPIO_ResetBits(GPIOB, LED_DONVI); 
			GPIOA->ODR = NUM[i%10];
			Delay_ms(10);
			GPIO_SetBits(GPIOB, LED_DONVI);
			//GPIOB->ODR = 0xffff;
			
		/*quet led hang chuc vi*/
			GPIO_ResetBits(GPIOB, LED_CHUC); 
			GPIOA->ODR = NUM[(i/10)%10];
			Delay_ms(10);
			GPIO_SetBits(GPIOB, LED_CHUC);	
			//GPIOB->ODR = 0xffff;
			/*quet led hang tram*/
			GPIO_ResetBits(GPIOB, LED_TRAM); 
			GPIOA->ODR = NUM[(i/100)%10];
			Delay_ms(10);
			GPIO_SetBits(GPIOB, LED_TRAM);
			
			/*quet led hang nghin*/
			GPIO_ResetBits(GPIOB, LED_NGHIN); 
			GPIOA->ODR = NUM[i/1000];
			Delay_ms(10);
			GPIO_SetBits(GPIOB, LED_NGHIN);
		}
	}
}
