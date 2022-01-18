#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

uint16_t _time = 0;
uint16_t flag_out = 0;
volatile uint8_t flag_exti11 = 0;
volatile uint8_t flag_exti12 = 0;
volatile uint8_t flag_exti14 = 0;
volatile uint8_t flag_exti15 = 0;

void Delay_ms(uint16_t _time);
void led_Init(void);
void button_Init(void);
void exti_Init(void);
void nvic_Init(void);

int main(){
	led_Init();
	button_Init();
	exti_Init();
	nvic_Init();
	while(1){
		if(flag_exti11){
			_time++;
			if(_time <= 1000){
				GPIOB->ODR = 0xAA;
			}
			else{
				GPIOB->ODR = 0xFF;
				if(_time >= 2000){
					_time = 0;
					flag_out++;
					if(flag_out >= 5){
						flag_exti11 = 0;
						flag_out = 0;
					}
				}
			}
		}
		if(flag_exti12){
			_time++;
			if(_time <= 500){
				GPIOB->ODR = 0x55;
			}
			else{
				GPIOB->ODR = 0xFF;
				if(_time >= 1000){
					_time = 0;
					flag_out++;
					if(flag_out >= 5){
						flag_exti12 = 0;
						flag_out = 0;
					}
				}
			}
		}
		if(flag_exti14){
			_time++;
			if(_time <= 200){
				GPIOB->ODR = 0xF0;
			}
			else{
				GPIOB->ODR = 0xFF;
				if(_time >= 400){
					_time = 0;
					flag_out++;
					if(flag_out >= 5){
						flag_exti14 = 0;
						flag_out = 0;
					}
				}
			}
		}
		if(flag_exti15){
			_time++;
			if(_time <= 100){
				GPIOB->ODR = 0x0F;
			}
			else{
				GPIOB->ODR = 0xFF;
				if(_time >= 200){
					_time = 0;
					flag_out++;
					if(flag_out >= 25){
						flag_exti15 = 0;
						flag_out = 0;
					}
				}
			}
		}
		Delay_ms(1);
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
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	button_init.GPIO_Mode = GPIO_Mode_IPU;
	button_init.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14|GPIO_Pin_15;
	button_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&button_init);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource14);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource15);
}


void exti_Init(void){
	EXTI_InitTypeDef exti_init;
	
	exti_init.EXTI_Line=EXTI_Line11|EXTI_Line12|EXTI_Line14|EXTI_Line15;
	//lenh chon kenh tin hieu ngat ngoai
	exti_init.EXTI_Mode=EXTI_Mode_Interrupt;
	//lenh chon che do ngat ngoai
	exti_init.EXTI_Trigger=EXTI_Trigger_Falling;
	//lenh chon canh tich cuc
	exti_init.EXTI_LineCmd = ENABLE;
	EXTI_Init(&exti_init);

}
void nvic_Init(void){
	NVIC_InitTypeDef NVIC_ST;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_ST.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_ST.NVIC_IRQChannelSubPriority=0;
	NVIC_ST.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_ST);
}

void EXTI15_10_IRQHandler(){
	
	if(EXTI_GetITStatus(EXTI_Line11)!=RESET){
		EXTI_ClearITPendingBit(EXTI_Line11);
			flag_exti11 = 1;
	}	
	else if(EXTI_GetITStatus(EXTI_Line12)!=RESET){
		EXTI_ClearITPendingBit(EXTI_Line12);
			flag_exti12 = 1;
	}	
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET){
		EXTI_ClearITPendingBit(EXTI_Line14);
			flag_exti14 = 1;
	}
	else if(EXTI_GetITStatus(EXTI_Line15)!=RESET){
		EXTI_ClearITPendingBit(EXTI_Line15);
			flag_exti15 = 1;
	}	
}


