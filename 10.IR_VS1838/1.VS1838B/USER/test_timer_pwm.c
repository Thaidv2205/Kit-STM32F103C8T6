#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "uart.h"


volatile char vrvc_SttBlink = 0, Flag = 0;
uint16_t IC1Value = 0,DutyCycle = 0, Frequency = 0;
void led_Init(void);
void pwm_In(void);
void pwm_Out(void);
void pwm_Init(void);
void Delay_ms(uint16_t _time);

int main(){
	uart_Init();
	led_Init();
	pwm_Init();
	pwm_Out();
	pwm_In();
	while(1){
//		if(Flag == 1){
			Flag = 0;
			printf("DutyCycle: %d\n",DutyCycle);
			printf("Frequency: %d\n",Frequency);
	//	}
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
TIM_TimeBaseInitTypeDef tim_timebasestruct;
TIM_ICInitTypeDef tim_icinitstructure;
TIM_OCInitTypeDef tim_ocinitstructure;

void pwm_Init(void){
	GPIO_InitTypeDef pwm_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	// PWM output TIM1-CH2
	pwm_init.GPIO_Mode = GPIO_Mode_AF_PP;
	pwm_init.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA,&pwm_init);	
	// PWM input TIM3-CH1
	pwm_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	pwm_init.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA,&pwm_init);	
}

void pwm_Out(void){
//	TIM_TimeBaseInitTypeDef pwm_out;
//	TIM_OCInitTypeDef pwm_oc;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
//	pwm_out.TIM_Period = 999;
//	pwm_out.TIM_Prescaler = 35;
//	pwm_out.TIM_ClockDivision = 0; 
//	pwm_out.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM2, &pwm_out);
//	
//	// cau hinh timer 2 CH2 phat xung PWM voi duty = 70%
//	pwm_oc.TIM_OCMode = TIM_OCMode_PWM1;
//	pwm_oc.TIM_OutputState = TIM_OutputState_Enable;
//	pwm_oc.TIM_Pulse = 700;
//	pwm_oc.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC3Init(TIM2, &pwm_oc);
//	TIM_CtrlPWMOutputs(TIM2,ENABLE);
//	TIM_Cmd(TIM2,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	TIM_TimeBaseInitTypeDef tim_timebasestruct;
//	TIM_OCInitTypeDef tim_ocinitstructure;
	tim_timebasestruct.TIM_Period=999;//Count Initial value
	tim_timebasestruct.TIM_Prescaler=35;//divide factor
	tim_timebasestruct.TIM_ClockDivision=0;//Clock Segmentation
	tim_timebasestruct.TIM_CounterMode=TIM_CounterMode_Up;//Up counting mode
	TIM_TimeBaseInit (TIM3,&tim_timebasestruct);
	//Tim3_ch1 as PWM output
	tim_ocinitstructure.TIM_OCMode=TIM_OCMode_PWM1;
	tim_ocinitstructure.TIM_OutputState=TIM_OutputState_Enable;
	tim_ocinitstructure.TIM_Pulse=36000;
	tim_ocinitstructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OC1Init(TIM3,&tim_ocinitstructure);
	TIM_CtrlPWMOutputs(TIM3,ENABLE);
	TIM_OC1PreloadConfig (TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig (TIM3,ENABLE);
	TIM_Cmd (TIM3,ENABLE);

}

void pwm_In(void){
//	TIM_TimeBaseInitTypeDef pwm_in;
//	TIM_ICInitTypeDef pwm_ic;
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
//	pwm_in.TIM_Period = 0xFFFF;
//	pwm_in.TIM_Prescaler = 0;
//	pwm_in.TIM_ClockDivision = 0; 
//	pwm_in.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIM3, &pwm_in);	
//	
//	pwm_ic.TIM_Channel = TIM_Channel_1;
//	pwm_ic.TIM_ICFilter = 15;
//	pwm_ic.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	pwm_ic.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	pwm_ic.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM_PWMIConfig(TIM3,&pwm_ic);
//	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
//	TIM_SelectInputTrigger(TIM3,TIM_TS_TI2FP2);
//	TIM_SelectMasterSlaveMode(TIM3,TIM_MasterSlaveMode_Enable);
//	TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Reset);
//	
//	NVIC_EnableIRQ(TIM3_IRQn);
//	
//	TIM_Cmd(TIM3, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	

	tim_icinitstructure.TIM_Channel=TIM_Channel_2;//Select TIM2_CH2, select Input IC2 map to TI2 
	tim_icinitstructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//Rising edge capture 
	tim_icinitstructure.TIM_ICSelection=TIM_ICSelection_DirectTI;// Map to TI2 on 
	tim_icinitstructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//to perform a capture each time an edge is detected on the capture input 
	tim_icinitstructure.TIM_ICFilter=0;//filter setting, through a few periodic jump to determine the waveform stability. (Sampling high, only the continuous acquisition of n levels is high level is considered valid, otherwise less than n is considered invalid, n take 0x0-0xf) 
	TIM_PWMIConfig(TIM2,&tim_icinitstructure);// The above is the input capture configuration 
	TIM_SelectInputTrigger (TIM2,TIM_TS_TI2FP2);//Select the filtered TI2FP2 input as the trigger source, triggering the reset of the following program 
	TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_Reset);///From the mode controller is set to reset mode-the selected trigger signal rising edge reinitialize counter and produces an update signal (rising edge one to, tim2->cnt is cleared, each rising edge comes, CNT will be cleared 0) 
	TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);//start timer passive trigger 
	TIM_ITConfig (TIM2,TIM_IT_CC2,ENABLE);//capture Interrupt Open 
	TIM_ClearFlag (TIM2,TIM_IT_CC2);//clear Flag bit 
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM_Cmd (TIM2,ENABLE);//Enable Timer 2 
	
	
}

void TIM2_IRQHandler(void){
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)){
		DutyCycle = 0;
		Frequency = 0;
		TIM_ClearITPendingBit(TIM2,TIM_IT_CC2);
		IC1Value = TIM_GetCapture2(TIM2);
		
		DutyCycle = TIM_GetCapture1(TIM2)*100/IC1Value;
		Frequency = 72000000/IC1Value;
		//Flag = 1;
		
	}
}

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}