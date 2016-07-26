//delay.c

#include "delay.h"

void clk_config(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);  
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
  
}

void timer_init()
{
  clk_config();
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 0);
  //TIM2_Cmd(ENABLE);
}

void delay_ms(uint16_t ms)
{
  uint16_t i = 0;
  ms = ms / 4;
  for(i=0; i<ms; i++){
    delay_us(4000);
  }
}

void delay_us(uint32_t us)
{
  if(us > 4000)
  {
    delay_ms(us/1000);
  }
  us = 16 * us;
  TIM2_SetCounter(0);
  TIM2_ARRPreloadConfig(DISABLE);
  TIM2_SetAutoreload(us);
  
  TIM2_Cmd(ENABLE);
  while(TIM2_GetFlagStatus(TIM2_FLAG_UPDATE) == RESET);
  TIM2_ClearFlag(TIM2_FLAG_UPDATE);
  TIM2_Cmd(DISABLE);
}