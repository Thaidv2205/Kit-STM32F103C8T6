#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "debug.h"

#define BTN_CLK	 RCC_APB2Periph_GPIOA
#define BTN_PORT GPIOA
#define BTN_KEY1 GPIO_Pin_3
#define BTN_KEY2 GPIO_Pin_4
#define BTN_KEY3 GPIO_Pin_5
#define BTN_KEY4 GPIO_Pin_6

typedef struct{
	unsigned char	  vruc_DataOld;
	unsigned int 	  vruc_CountAccess;
	unsigned char 	vruc_FlagChange;
  unsigned char   vruc_hold;
}	TS_TypeInput;

GPIO_InitTypeDef GPIO_INITS;

TS_TypeInput BTN_K1, BTN_K2, BTN_K3, BTN_K4;
char str[16];

void Delay_ms(uint16_t _time);
void led_Init(void);
void button_Init(void);

void Fn_INPUT_ReadInput (unsigned char _vruc_Input, TS_TypeInput *_vrts_DataInput);
void BUTTON_Run(void);

uint16_t counter = 0, time_out = 0;

int main(){
	led_Init();
	button_Init();
	USARTx_Init();
	while(1){
		BUTTON_Run();
		if(BTN_K1.vruc_FlagChange){ // trang thai nhan nha
			BTN_K1.vruc_FlagChange = 0;
			counter++;
			if(counter%2==0){
				GPIO_SetBits(GPIOA,GPIO_Pin_0);
			}
			else{
				GPIO_ResetBits(GPIOA,GPIO_Pin_0);
			}
		}

		if(BTN_K1.vruc_hold){ // trang thai nhan giu
			BTN_K1.vruc_hold = 0;
			GPIO_ResetBits(GPIOA,GPIO_Pin_1);
		}
		else{
			GPIO_SetBits(GPIOA,GPIO_Pin_1);
		}
		Delay_ms(1);
	}
}



void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	led_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&led_init);
	GPIOA->ODR = 0xff;
}

void button_Init(void){
	GPIO_InitTypeDef button_init;
	RCC_APB2PeriphClockCmd(BTN_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	button_init.GPIO_Mode = GPIO_Mode_IPU;
	button_init.GPIO_Pin = BTN_KEY1|BTN_KEY2|BTN_KEY3|BTN_KEY4;
	button_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(BTN_PORT,&button_init);
	
	BTN_K1.vruc_FlagChange = 0;
	BTN_K2.vruc_FlagChange = 0;   
	BTN_K3.vruc_FlagChange = 0;      
	BTN_K4.vruc_FlagChange = 0;
	
}

void Fn_INPUT_ReadInput (unsigned char _vruc_Input, TS_TypeInput *_vrts_DataInput){
  _vrts_DataInput->vruc_DataOld = _vruc_Input;
  if(!_vruc_Input){
    if(_vrts_DataInput->vruc_DataOld == _vruc_Input){
      if(_vrts_DataInput->vruc_CountAccess <= 500){ // thoi gian nhan nut < 500ms -> nhan nha; nguoc lai la nhan giu
        _vrts_DataInput->vruc_CountAccess++;
        if(_vrts_DataInput->vruc_CountAccess == 50){ // nhan nha
          _vrts_DataInput->vruc_FlagChange = 1;
          _vrts_DataInput->vruc_hold = 0;
        }
      }
      else// nhan giu
      {
				_vrts_DataInput->vruc_FlagChange = 0;
        _vrts_DataInput->vruc_hold = 1;
      }
    }
    else{
      _vrts_DataInput->vruc_CountAccess = 0;
    }
  }
  else{
    _vrts_DataInput->vruc_CountAccess = 0;
  }
}

void BUTTON_Run(void)
{
  Fn_INPUT_ReadInput (GPIO_ReadInputDataBit(GPIOA,BTN_KEY1), z	(&BTN_K1));
//  Fn_INPUT_ReadInput (GPIO_ReadInputDataBit(GPIOB,BTN_KEY2), (TS_TypeInput*)(&BTN_K2));
//  Fn_INPUT_ReadInput (GPIO_ReadInputDataBit(GPIOB,BTN_KEY3), (TS_TypeInput*)(&BTN_K3));
//  Fn_INPUT_ReadInput (GPIO_ReadInputDataBit(GPIOB,BTN_KEY4), (TS_TypeInput*)(&BTN_K4));

}

