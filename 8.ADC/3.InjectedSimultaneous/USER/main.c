#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "uart.h"

unsigned int vrui_ADC_Value1;
unsigned int vrui_ADC_Value2;

void Delay_ms(uint16_t _time);
void GPIO_Config(void);
void ADC_Config(void);
unsigned int ADCx_Read1 (void);
unsigned int ADCx_Read2 (void);
int Map(int x, int in_min, int in_max, int out_min, int out_max);

int main(){
	GPIO_Config();
	ADC_Config();
	uart_Init();
	while(1){
		vrui_ADC_Value1 = ADCx_Read1();
		vrui_ADC_Value2 = ADCx_Read2();
		printf("ADC1-Injected channel_1: %d\n",vrui_ADC_Value1);
		printf("ADC2-Injected channel_1: %d\n",vrui_ADC_Value2);
		Delay_ms(500);
	}
}

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}


void GPIO_Config(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_Config(void){
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Init(ADC2, &ADC_InitStructure);

	ADC_InjectedSequencerLengthConfig(ADC1, 1);// tong so kenh can doc
	ADC_InjectedSequencerLengthConfig(ADC2, 1);
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
	ADC_InjectedChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
	
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);
	ADC_ExternalTrigInjectedConvConfig(ADC2, ADC_ExternalTrigInjecConv_None);
	ADC_AutoInjectedConvCmd(ADC1, ENABLE);
	ADC_AutoInjectedConvCmd(ADC2, ENABLE);
	ADC_Cmd (ADC1, ENABLE);
	ADC_Cmd (ADC2, ENABLE);

	ADC_ResetCalibration(ADC1);	// Reset hieu Chuan ADC
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);	// Hieu chuan lai
	while(ADC_GetCalibrationStatus(ADC1));
	ADC_SoftwareStartInjectedConvCmd(ADC1 , ENABLE);

	ADC_ResetCalibration(ADC2);	// Reset hieu Chuan ADC
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);	// Hieu chuan lai
	while(ADC_GetCalibrationStatus(ADC2));
	ADC_SoftwareStartInjectedConvCmd(ADC2 , ENABLE);

}


unsigned int ADCx_Read1(void){
	return(ADC_GetInjectedConversionValue(ADC1, ADC_InjectedChannel_1));
}

unsigned int ADCx_Read2(void){
	return(ADC_GetInjectedConversionValue(ADC2, ADC_InjectedChannel_1));
}


int Map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}