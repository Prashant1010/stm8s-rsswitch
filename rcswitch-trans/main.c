//main.c

#include "stm8s.h"
#include "delay.h"
#include "rcswitch_trans.h"
#include "uart.h"




int main( void )
{
  timer_init();
  uart_config();
  
  
  GPIO_DeInit(GPIOA);
  GPIO_DeInit(GPIOD);
  GPIO_Init(GPIOA, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST);
  

  RCSwitch();
  enableTransmit(10);
  
  
  while(1)
  {
      if(status == 1){
        send(num, 24);
        status = 0;
      }
    delay_ms(2000);
  }
}
