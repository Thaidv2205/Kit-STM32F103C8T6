#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_exti.h"             // Keil::Device:StdPeriph Drivers:EXTI
#include "uart.h"

#define REMOTE_CH_SUB 		0xA2
#define REMOTE_CH					0x62
#define REMOTE_CH_PLUS		0xE2
#define REMOTE_PREV				0x22
#define REMOTE_NEXT				0x02
#define REMOTE_PAUSE			0xC2
#define REMOTE_VOL_SUB		0xE0
#define REMOTE_VOL_PLUS		0xA8
#define REMOTE_EQ					0x90
#define REMOTE_0					0x68
#define REMOTE_1					0x30
#define REMOTE_2					0x18
#define REMOTE_3					0x7A
#define REMOTE_4					0x10
#define REMOTE_5					0x38
#define REMOTE_6 					0x5A
#define REMOTE_7					0x42
#define REMOTE_8					0x4A
#define REMOTE_9					0x52
#define REMOTE_100				0x98
#define REMOTE_200				0xB0

volatile char vrvc_SttBlink = 0, Flag = 0;
uint16_t ucTim2Flag = 0,repeatEnable = 0, Frequency = 0;
char remote_code[100];
void led_Init(void);
void IR_Pin_init(void);
void Tim2_UPCount_Init(u16 Prescaler,u16 Period);
uint8_t Ir_Server(void);
void Delay_ms(uint16_t _time);
void led_Init(void);
void ir_scanfCode(void);
void ir_test(void);
void Control_Buzz(void);
uint16_t Value_IR = 0;

int main(){
	uart_Init();
  IR_Pin_init();
	led_Init();
  Tim2_UPCount_Init(2-1,3600-1); // 0.1ms

  while(1){
//		ir_test();
    ir_scanfCode();
//		Control_Buzz();
  }

}

void IR_Pin_init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_Init(GPIOA,&GPIO_InitStructure);

  EXTI_ClearITPendingBit(EXTI_Line8);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource8); 
  EXTI_InitStructure.EXTI_Line=EXTI_Line8;
  EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure); 

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn; 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
}

void Tim2_UPCount_Init(u16 Prescaler,u16 Period){
  TIM_TimeBaseInitTypeDef TIM_StructInit;
  NVIC_InitTypeDef NVIC_StructInit;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_StructInit.TIM_Period=Period;
  TIM_StructInit.TIM_Prescaler=Prescaler;
  TIM_StructInit.TIM_ClockDivision=TIM_CKD_DIV1;
  TIM_StructInit.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_StructInit.TIM_RepetitionCounter=0;
  TIM_TimeBaseInit(TIM2, &TIM_StructInit);

  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, ENABLE);    
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);

  NVIC_StructInit.NVIC_IRQChannel=TIM2_IRQn;
  NVIC_StructInit.NVIC_IRQChannelCmd=ENABLE;
  NVIC_StructInit.NVIC_IRQChannelPreemptionPriority=0;
  NVIC_StructInit.NVIC_IRQChannelSubPriority=1;
  NVIC_Init(&NVIC_StructInit);
}


void TIM2_IRQHandler(void){
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  ucTim2Flag++;
}

uint8_t irdata[33];   //Used to record the time between two falling edges
char receiveComplete; //Receive Complete Flag Bits
uint8_t idx;          //Number received for indexing
char startflag;       //Indicates start of reception

void EXTI9_5_IRQHandler(void){
  uint16_t ir_time;
  if(startflag){
		ir_time = ucTim2Flag; 
		if(ucTim2Flag < 150 && ucTim2Flag >= 50 ){ // Receive Sync Header
			idx=0;  // Array subscript zeroing
		}

		irdata[idx] = ucTim2Flag;  // Get Count Time
		ucTim2Flag = 0;            // Zero count time for next count
		idx++;                     // Received a data, index plus 1

		if(idx==33){       // If you receive 33 bits of data, including 32 bits and with a sync header
			idx=0;
			ucTim2Flag = 0;
			receiveComplete = 1;
		}
  }
  else{   // Drop Edge First Trigger
		idx = 0;
		ucTim2Flag = 0;
		startflag = 1;
  }
	EXTI_ClearITPendingBit(EXTI_Line8);  // Clear interrupt flag
}

uint8_t Ir_Server()
{
  uint8_t i,j,idx=1; //idx starts with 1 to indicate that the synchronization header time is not handled
  uint8_t temp;
  for(i=0; i<4; i++)
  {
      for(j=0; j<8; j++)
      {
          if(irdata[idx] >=8 && irdata[idx] < 15)   //Represents 0
          {
              temp = 0;
          }
          else if(irdata[idx] >=18 && irdata[idx]<25) //Represents 1
          {
              temp = 1;
          }
          remote_code[i] <<= 1;
          remote_code[i] |= temp;
          idx++;
      }
  }

  return remote_code[2];  // The array records control codes, each key is different
}

void ir_scanfCode(void){
//	printf("key_code = %#x\n",Ir_Server());
	printf("key_code = %d\n",Ir_Server());
	Delay_ms(100);
}

void Delay_ms(uint16_t _time){
	u16 i,j;
	for(i = 0; i < _time; i++){
		for(j = 0; j < 0x2aff; j++);
	}
}

void led_Init(void){
	
	GPIO_InitTypeDef led_init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	led_init.GPIO_Mode = GPIO_Mode_Out_PP;
	led_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|
											GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	led_init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&led_init);
	GPIOB->ODR = 0xff;
	
}

void Control_Buzz(void){
	Value_IR = Ir_Server();
//	printf("key_code = %#x\n",Ir_Server());
	if(Value_IR == REMOTE_100){	
		GPIOB->ODR = 0xfeff;
	}	
	else if(Value_IR == REMOTE_200){
		GPIOB->ODR = 0xffff;
	}
	else if(Value_IR == REMOTE_EQ){
		GPIOB->ODR = 0xff;
	}
	else if(Value_IR == REMOTE_1){
		GPIOB->ODR = 0xfe;
	}
	else if(Value_IR == REMOTE_2){
		GPIOB->ODR = 0xfc;
	}
	else if(Value_IR == REMOTE_3){
		GPIOB->ODR = 0xf8;
	}		
	else if(Value_IR == REMOTE_4){
		GPIOB->ODR = 0xf0;
	}	
	else if(Value_IR == REMOTE_5){
		GPIOB->ODR = 0xe0;
	}	
	else if(Value_IR == REMOTE_6){
		GPIOB->ODR = 0xc0;
	}	
	else if(Value_IR == REMOTE_7){
		GPIOB->ODR = 0x80;
	}	
	else if(Value_IR == REMOTE_8){
		GPIOB->ODR = 0x00;
	}	
	else if(Value_IR == REMOTE_9){
		GPIOB->ODR = 0xaa;
	}	
	else if(Value_IR == REMOTE_0){
		GPIOB->ODR = 0x55;
	}	
//	printf("PB8: %d",GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_8));
	Delay_ms(100);
}

void ir_test(void){
	Value_IR = Ir_Server();
	if(Value_IR == REMOTE_EQ){
		GPIOB->ODR = 0xff;
	}
	else if(Value_IR == REMOTE_1){
		GPIOB->ODR = 0xfe;
	}
	else if(Value_IR == REMOTE_2){
		GPIOB->ODR = 0xfc;
	}
	else if(Value_IR == REMOTE_3){
		GPIOB->ODR = 0xf8;
	}		
	else if(Value_IR == REMOTE_4){
		GPIOB->ODR = 0xf0;
	}	
	else if(Value_IR == REMOTE_5){
		GPIOB->ODR = 0xe0;
	}	
	else if(Value_IR == REMOTE_6){
		GPIOB->ODR = 0xc0;
	}	
	else if(Value_IR == REMOTE_7){
		GPIOB->ODR = 0x80;
	}	
	else if(Value_IR == REMOTE_8){
		GPIOB->ODR = 0x00;
	}	
	else if(Value_IR == REMOTE_9){
		GPIOB->ODR = 0xaa;
	}	
	else if(Value_IR == REMOTE_0){
		GPIOB->ODR = 0x55;
	}	
	else if(Value_IR == REMOTE_100){
		GPIOB->ODR = 0xaf;
	}	
	Delay_ms(100);
}