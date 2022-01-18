#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "lcd16x2.h"
#include "delay_sys.h"

int main(){
	SysTick_Init();
	LCD_Init();	
	LCD_Gotoxy(0,0);
	LCD_Puts("ELECTRONIC");
	LCD_Gotoxy(0,1);
	LCD_Puts("PTIT-KTDT1");
	while(1){
	
	}
}

