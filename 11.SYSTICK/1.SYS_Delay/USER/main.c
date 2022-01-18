#include "stm32f10x.h"            // Device header
#include "stm32f10x_gpio.h"       // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"        // Keil::Device:StdPeriph Drivers:RCC

unsigned char vruc_SYSTICK_Count = 0;

void led_Init(void);
void Fn_GPIO_ToggleLED(void);
void Sys_Delay_1ms(void);
void Sys_Delay_ms(uint16_t _time);

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	led_Init();
	while(1){
		Fn_GPIO_ToggleLED();
	}
}

//Ham dao trang thai LED
void Fn_GPIO_ToggleLED(void){
	GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_SET);
	Sys_Delay_ms(500);
	GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_RESET);
	Sys_Delay_ms(500);
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

void Sys_Delay_1ms(void){
	SysTick	-> LOAD 	= 72000 - 1;
	SysTick	-> VAL 		= 0;
	SysTick -> CTRL	  = 0x05; /*ENABLE the timer and choose system clock as the clock source*/
	while(!(SysTick->CTRL & (0x10000))){} /*wait until the count flag is set*/
	SysTick -> CTRL 	= 0x00;/*stop the timer Enable = 0*/
}

void Sys_Delay_ms(uint16_t _time){
	while(_time--){
		Sys_Delay_1ms();
	}
}
