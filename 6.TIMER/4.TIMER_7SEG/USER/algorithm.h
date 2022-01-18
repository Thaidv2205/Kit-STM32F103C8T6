#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include "stm32f10x.h"

#define LED_DONVI GPIO_Pin_15
#define LED_CHUC 	GPIO_Pin_14
#define LED_TRAM	GPIO_Pin_13
#define LED_NGHIN GPIO_Pin_12


void Delay_ms(unsigned int time);

/*bat cac led don vi, chuc, tram, nghin*/
void OFF_LED7(void);

/*lay cac chu so cua cac hang*/
void LED7_Receive(uint16_t vrui_Number);

/*xuat du lieu ra cac chan a->g*/
void LED7_DataOut(uint16_t vrui_Data);

/*bat cac led don vi, chuc, tram, nghin*/
void ON_LED7(uint16_t vrui_Swit);

/*hien thi tren phan cung*/
void Display(uint16_t vrui_Value);

/**/
void TIM4_IRQHandler(void);


#endif
