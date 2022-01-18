#include "delay_sys.h"
#define SYSCLK_MHZ 72
void SysTick_Init()
{
  //SysTick->VAL  = 0;                                          // Load the SysTick Counter Value
  SysTick->LOAD = SysTick_LOAD_RELOAD_Msk;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_TICKINT_Msk   |
                  SysTick_CTRL_ENABLE_Msk;                    // Enable SysTick IRQ and SysTick Timer
}
uint32_t volatile SysTickCounter = 1;

void SysTick_Handler()
{
  ++SysTickCounter;
}
uint64_t SysTick64()
{

  return (((uint64_t)SysTickCounter) << 24) - SysTick->VAL;
}

uint32_t SysTick32()
{
  return (SysTickCounter << 24) - SysTick->VAL;
}

uint32_t SysTick24()
{
  return ~(SysTick->VAL);
}

uint64_t SysTick_Millis()
{
  return SysTick64() / (SYSCLK_MHZ *1000);
}

uint64_t SysTick_Micros()
{
  return SysTick64() / SYSCLK_MHZ;
}

void delay_us(unsigned long us)
{
  uint32_t finish = SysTick32() + (us * SYSCLK_MHZ);
  while (((int32_t)(finish - SysTick32())) > 0);
}

void delay_ms(unsigned long ms)
{
  for (; ms; ms--)  // while (ms--)
  {
    delay_us(1000);
  }
}
