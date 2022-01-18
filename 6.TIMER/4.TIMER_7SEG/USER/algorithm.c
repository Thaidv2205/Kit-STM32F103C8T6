#include "algorithm.h"

#define NUMBER_LED7 4 								/*so led 7 thanh*/
uint16_t Led7_Font[10]= {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};// Ma Anode chung
uint16_t vrui_Count=0; 								/*bien dem so led 7 thanh*/
uint16_t Led7_BufferData[NUMBER_LED7]; /*mang chua so led 7 thanh*/
volatile int  vrvi_i = 0;
extern unsigned int vrui_DisValue;

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void Delay_ms(unsigned int time){
	unsigned int i,j;
	for(i=0; i<time; i++){
		for(j=0; j<0x2aff; j++);
	}
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void OFF_LED7(void){
	GPIOB->ODR = 0xffff;
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void LED7_Receive(uint16_t vrui_Number){
	int16_t vri_i; 
	for(vri_i = 0; vri_i < NUMBER_LED7; vri_i++)
	{     
		Led7_BufferData[vri_i]=Led7_Font[vrui_Number%10];
		vrui_Number=vrui_Number/10;
	} 
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void LED7_DataOut(uint16_t vrui_Data){
	GPIOA->ODR = vrui_Data;
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void ON_LED7(uint16_t vrui_Swit){
	switch(vrui_Swit){
		case 0: GPIO_ResetBits(GPIOB, LED_DONVI); // bat led don vi.
		break;
		case 1: GPIO_ResetBits(GPIOB, LED_CHUC); // bat led chuc.
		break;
		case 2: GPIO_ResetBits(GPIOB, LED_TRAM); // bat led tram.
		break;
		case 3: GPIO_ResetBits(GPIOB, LED_NGHIN); // bat led nghin.
		break;
	}
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
void Display(uint16_t vrui_Value){
	OFF_LED7();
	LED7_Receive(vrui_Value); 
	LED7_DataOut(Led7_BufferData[vrui_Count]);
	ON_LED7(vrui_Count);
	vrui_Count++;
	if(vrui_Count>=NUMBER_LED7){
		vrui_Count = 0;
	}
}
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

