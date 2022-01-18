#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x_usart.h"
#include "string.h"
#include "stdio.h"

void uart_Init(void);
void uart_SendChar(char _chr);
void uart_SendStr(char *str);

#endif
