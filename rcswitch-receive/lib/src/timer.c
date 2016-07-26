//timer.c

#include "timer.h"


void clk_config(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

void timer_init()
{
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_1, 1600);
  //TIM2_PrescalerConfig(TIM2_PRESCALER_32768, TIM2_PSCRELOADMODE_IMMEDIATE);
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  
  ITC_SetSoftwarePriority(ITC_IRQ_TIM2_OVF, ITC_PRIORITYLEVEL_1);  
  TIM2_Cmd(ENABLE);
}
