#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

int i,j,k;

void Delay_ms(uint16_t _time);
void led_Init(void);
void button_Init(void);
void button_Press(void);
void button_PressHold(void);
void button_Scan(void);
int main(){
	led_Init();
	button_Init();
	while(1){
		button_Press();
		//button_PressHold();
		//button_Scan();
	}
}

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
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
	
	GPIOB->ODR = 0XFF;
}

void button_Init(void){
	GPIO_InitTypeDef button_init;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	button_init.GPIO_Mode = GPIO_Mode_IPU;
	button_init.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
	button_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&button_init);
}

void button_Press(void){
	if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)){
		GPIOB->ODR = 0x00;
	}
	else{
		GPIOB->ODR = 0xff;
	}
}
uint16_t counter = 0;
uint8_t sttOld = 1, sttNew = 1;
void button_PressHold(void){
	sttOld = sttNew;
	sttNew = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12);
	if(sttOld == 1 && sttNew == 0){
		counter++;
	}
	if(counter%2==0){
		GPIOB->ODR = 0xff;
	}
	else{
		GPIOB->ODR = 0x00;
	}
	Delay_ms(1);
}

u8 check_Flag = 0;
u16 mode_counter = 0;
void button_Scan(void){
	sttOld = sttNew;
	sttNew = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_14);
	if(sttOld == 1 && sttNew == 0){		
		check_Flag++;		
	}
	if(check_Flag <= 8){
		mode_counter++;
		if(mode_counter <= 100){
			GPIOB->ODR = 0xFF<<check_Flag;
		}
		else{
			GPIOB->ODR = 0xFF;
			if(mode_counter >=200){
				mode_counter = 0;
			}
		}
	}
	else check_Flag = 0;
	Delay_ms(1);
}

