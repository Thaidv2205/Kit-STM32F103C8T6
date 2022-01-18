#include "lcd16x2.h"
#define LCD_RS      GPIO_Pin_14  
#define LCD_RW      GPIO_Pin_13
#define LCD_EN      GPIO_Pin_12
#define LCD_D4      GPIO_Pin_4
#define LCD_D5      GPIO_Pin_5
#define LCD_D6      GPIO_Pin_6
#define LCD_D7      GPIO_Pin_7

GPIO_InitTypeDef GPIO_LCD_InitStruction;
void GPIO_LCD_Config(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  
  GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
  GPIO_LCD_InitStruction.GPIO_Pin     = LCD_D4|LCD_D5|LCD_D6|LCD_D7;
  GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_LCD_InitStruction);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_LCD_InitStruction.GPIO_Mode    = GPIO_Mode_Out_PP;
  GPIO_LCD_InitStruction.GPIO_Pin     = LCD_RS|LCD_RW|LCD_EN;
  GPIO_LCD_InitStruction.GPIO_Speed   = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_LCD_InitStruction);
	
}
void LCD_Enable(void)
{
  GPIO_SetBits(GPIOB, LCD_EN);
  delay_us(3);
  GPIO_ResetBits(GPIOB, LCD_EN);
  delay_us(50); 
}

void LCD_Send4Bit(unsigned char Data)
{
  GPIO_WriteBit(GPIOA, LCD_D4, Data & 0x01);
  GPIO_WriteBit(GPIOA, LCD_D5, (Data>>1)&1);
  GPIO_WriteBit(GPIOA, LCD_D6, (Data>>2)&1);
  GPIO_WriteBit(GPIOA, LCD_D7, (Data>>3)&1);
}

void LCD_SendCommand(unsigned char command)
{
  LCD_Send4Bit(command >> 4);
  LCD_Enable();
  LCD_Send4Bit(command);
  LCD_Enable();

}

void LCD_Clear()
{
  LCD_SendCommand(0x01);
  delay_us(10);
}

void LCD_Init()
{
	GPIO_LCD_Config();
  LCD_Send4Bit(0x00);
  GPIOB->BRR = LCD_RS;
	GPIOB->BRR = LCD_RW;
  LCD_Send4Bit(0x03);
  LCD_Enable();
  LCD_Enable();
  LCD_Enable();
  LCD_Send4Bit(0x02);
  LCD_Enable();
  LCD_SendCommand(0x28); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
  LCD_SendCommand(0x0c); // cho phep hien thi man hinh
  LCD_SendCommand(0x06); // tang ID, khong dich khung hinh
  LCD_SendCommand(0x01); // xoa toan bo khung hinh
	delay_ms(10);
}
void LCD_Gotoxy(unsigned char x, unsigned char y)
{
  unsigned char address;
  if(y == 0)address=(0x80+x);
  else if(y == 1) address=(0xc0+x);
  delay_ms(1);
  LCD_SendCommand(address);
  delay_us(50);
}

void LCD_PutChar(unsigned char Data)
{
	GPIO_SetBits(GPIOB, LCD_RS);
  LCD_SendCommand(Data);
  GPIO_ResetBits(GPIOB, LCD_RS);
}
/*tao ky tu dac biet*/
void LCD_Custom(unsigned char count, unsigned char *msg){
	unsigned char i;
	if(count < 8){
	/* Command 0x40 and onwards forces the device to point CGRAM address */
		LCD_SendCommand (0x40 + (count * 8));
		for(i=0;i<8;i++) /* Write 8 byte for generation of 1 character */
			LCD_PutChar(msg[i]);      
		}   
}

void LCD_Puts(char *s)
{
  while (*s)
  {
    LCD_PutChar(*s);
    s++;
  }
}

void TempShow(unsigned char z)
{
   LCD_PutChar((z/100)+48); //Tram
   LCD_PutChar((z%100/10)+48); //Chuc
   LCD_PutChar((z%10)+48); //Don vi
}