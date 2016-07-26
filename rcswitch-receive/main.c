#include "stm8s.h"
#include "uart.h"
#include "timer.h"
#include "rcswitch.h"

volatile uint32_t micros =0;

void gpio_init()
{
  GPIO_DeInit(GPIOA);
  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_IN_FL_IT);
  
  
  GPIO_Init(GPIOB, GPIO_PIN_6, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_Init(GPIOD, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_SLOW);
  GPIO_WriteHigh(GPIOD, GPIO_PIN_0);
}

void blink()
{
  uint32_t i,j;
  GPIO_WriteLow(GPIOD, GPIO_PIN_0);
  //for(j=0; j<10; j++)
  for(i=0; i<100000; i++);
  GPIO_WriteHigh(GPIOD, GPIO_PIN_0);
}

void main( void )
{
  unsigned int value,  rbitlen, rproto;
  uint32_t rvalue = 0;
  
  clk_config();
  timer_init();
  uart_config();
  gpio_init();
  RCSwitch();
  
  enableReceive(0);
  
  //while(1){
  //  UART2_SendData8('P');
  //  while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
  //}
  
  while(1)
  {  
    if (available())
    {
      value = getReceivedValue();
      if (value != 0)
      {
        rvalue = getReceivedValue();
        rbitlen = getReceivedBitlength();
        rproto = getReceivedProtocol();
        blink();
        entry_buffer(rvalue);
      }
      resetAvailable();
    } 
  }
}



