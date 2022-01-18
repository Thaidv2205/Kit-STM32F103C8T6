#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI

int i,j,k;

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
}


void exti_Init(void){
	EXTI_InitTypeDef exti_init;
	exti_init.EXTI_Line=EXTI_Line11;
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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_ST.NVIC_IRQChannel=EXTI15_10_IRQn;
	NVIC_ST.NVIC_IRQChannelSubPriority=0;
	NVIC_ST.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_ST);
	
}
void EXTI15_10_IRQHandler()
{
		
	int i;
	
	if(EXTI_GetITStatus(EXTI_IMR_MR11)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_IMR_MR11);
			for(i=0; i<5; i++)
			{
				GPIOB->ODR = 0x00;		
				Delay_ms(400);
				GPIOB->ODR = 0xFF;
				Delay_ms(400);
				
			}	
	}	
}


