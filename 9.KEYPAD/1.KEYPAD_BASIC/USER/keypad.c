#include "keypad.h"
#include "stm32f10x_gpio.h"
#include "string.h"

#define SCAN_COL_0			0xEF
#define SCAN_COL_1			0xDF
#define SCAN_COL_2			0xBF
#define SCAN_COL_3			0x7F
#define STOP						0xFF

#define PIN_ROW_0				GPIO_Pin_0
#define PIN_ROW_1 			GPIO_Pin_1
#define PIN_ROW_2 			GPIO_Pin_2
#define PIN_ROW_3 			GPIO_Pin_3

#define PIN_COL_0 			GPIO_Pin_4
#define PIN_COL_1 			GPIO_Pin_5
#define PIN_COL_2 			GPIO_Pin_6
#define PIN_COL_3 			GPIO_Pin_7

#define PORT_KEYPAD 		GPIOB

#define RCC_APB2_PROT_KEYPAD RCC_APB2Periph_GPIOB

uint16_t Row[]={PIN_ROW_0,PIN_ROW_1,PIN_ROW_2,PIN_ROW_3};
uint16_t Column[]={PIN_COL_0,PIN_COL_1,PIN_COL_2,PIN_COL_3};

GPIO_InitTypeDef GPIO_KEYPAD;

void Delay_ms(uint16_t _time){
	uint16_t i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2AFF; j++);
	}
}

void keypad_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2_PROT_KEYPAD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	// cau hinh cot
	GPIO_KEYPAD.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_KEYPAD.GPIO_Pin = PIN_COL_0|PIN_COL_1|PIN_COL_2|PIN_COL_3;
	GPIO_KEYPAD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_KEYPAD,&GPIO_KEYPAD);
	// cau hinh hang
	GPIO_KEYPAD.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_KEYPAD.GPIO_Pin = PIN_ROW_0|PIN_ROW_1|PIN_ROW_2|PIN_ROW_3;
	GPIO_KEYPAD.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PORT_KEYPAD,&GPIO_KEYPAD);
}

char keypad_Press(void){
	GPIO_Write(PORT_KEYPAD,SCAN_COL_0);
	if(!GPIO_ReadOutputDataBit(PORT_KEYPAD,PIN_COL_0)){
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_0)){return '1';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_1)){return '4';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_2)){return '7';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_3)){return '*';}
	}
	
	GPIO_Write(PORT_KEYPAD,SCAN_COL_1);
	if(!GPIO_ReadOutputDataBit(PORT_KEYPAD,PIN_COL_1)){
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_0)){return '2';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_1)){return '5';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_2)){return '8';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_3)){return '0';}
	}
	GPIO_Write(PORT_KEYPAD,SCAN_COL_2);
	if(!GPIO_ReadOutputDataBit(PORT_KEYPAD,PIN_COL_2)){
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_0)){return '3';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_1)){return '6';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_2)){return '9';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_3)){return '#';}
	}
	GPIO_Write(PORT_KEYPAD,SCAN_COL_3);
	if(!GPIO_ReadOutputDataBit(PORT_KEYPAD,PIN_COL_3)){
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_0)){return 'A';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_1)){return 'B';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_2)){return 'C';}
		if(!GPIO_ReadInputDataBit(PORT_KEYPAD,PIN_ROW_3)){return 'D';}
	}
	Delay_ms(1);
	return 0xFF;
}

//uint8_t CheckPass(char *Str1, char *Str2){
//	if(strlen(Str1) == strlen(Str2)){
//		if(strstr(Str1,Str2)!=NULL){ 
//			return 1;
//		}
//	}
//	return 0;
//}
