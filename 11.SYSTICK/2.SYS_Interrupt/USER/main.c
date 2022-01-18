#include "stm32f10x.h"            // Device header
#include "stm32f10x_gpio.h"       // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"        // Keil::Device:StdPeriph Drivers:RCC

unsigned char vruc_SYSTICK_Count = 0;

void led_Init(void);
void Fn_GPIO_ToggleLED(void);

int main(){
	SystemInit();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/10); // (1/10)s
	led_Init();
	while(1){
		if (vruc_SYSTICK_Count==10){
			vruc_SYSTICK_Count = 0;
			Fn_GPIO_ToggleLED();
		}
	}
}

//Ham dao trang thai LED
void Fn_GPIO_ToggleLED(void){
	if (GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7) == Bit_RESET){
		GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_SET);
	}
	else{
		GPIO_WriteBit(GPIOB,GPIO_Pin_7, Bit_RESET);
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

void SysTick_Handler (void){
	vruc_SYSTICK_Count++;
}

