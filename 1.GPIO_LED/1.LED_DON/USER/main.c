#include "stm32f10x.h"            // Device header
#include "stm32f10x_gpio.h"       // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"        // Keil::Device:StdPeriph Drivers:RCC

int i,j,k;
int STT_LED[]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};

void Delay_ms(uint16_t _time);
void led_Init(void);
void led_Blink(int n);
void led_UpDown(int n);
void led_Droplets(int n);

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	led_Init();
	while(1){
		led_Blink(5);
//		led_UpDown(2);
//		led_Droplets(2);
//		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
//		Delay_ms(500);
//		GPIO_SetBits(GPIOB,GPIO_Pin_7);
//		Delay_ms(500);
	}
}

void Delay_ms(uint16_t _time){
	volatile uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 601; j++);
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

void led_Blink(int n){
	for( i = 0; i < n; i++){
		GPIO_ResetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
												GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		Delay_ms(500);
		GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
												GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
		Delay_ms(500);
	}
}

void led_UpDown(int n){
	for( k = 0; k < n; k++){
		for(i = 0; i <= 8; i++){
			GPIOB->ODR = 0xFF<<i;
			Delay_ms(100);
		}
		for(i = 8; i >=0; i--){
			GPIOB->ODR = 0xFF>>i;
			Delay_ms(100);
		}
	}
}

void led_Droplets(int n){
	for(k = 0; k < n; k++){
		uint8_t STT_New = 0xfe;
		uint8_t STT_Pre = 0xff;

		for(i = 7; i >= 0 ; i--){
			for(j = 0; j <= i; j++){
				STT_New = STT_LED[j]&STT_Pre;
				GPIOB->ODR = STT_New; // STT_New = 0xfe;
				Delay_ms(100);
			}		
			STT_Pre = STT_New;// cap nhap lai bien STT_pre 
			GPIOB->ODR = STT_Pre; // xuat gia tri Pre ra protA;
		}
	}
}
