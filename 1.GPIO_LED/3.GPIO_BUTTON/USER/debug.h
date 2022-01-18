#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "stm32f10x_usart.h"
#include "stdio.h"

void USARTx_Init(void);
void UARTx_Sendc(USART_TypeDef* USARTx, uint8_t _ch);

#endif