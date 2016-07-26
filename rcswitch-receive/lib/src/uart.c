//uart.c

#include "uart.h"


void uart_config()
{
  UART2_DeInit();
  UART2_Init(9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE, ENABLE);
  ITC_SetSoftwarePriority(ITC_IRQ_UART2_RX, ITC_PRIORITYLEVEL_3); 
  //CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
  UART2_Cmd(ENABLE);
}

uint32_t pow(int exp, int pow)
{
  int i=0;
  uint32_t num = 1;
  if(pow == 0){
    return 1;
  }
  else if(pow == 1){
    return exp;
  }
  for(i=0; i<pow; i++)
  {
    num = num * exp;
  }
  return num;
}

void u32_to_str(uint32_t num)
{
  int i = 0, val;
  char ch;
  uint32_t no = 0 , num_ = num;
  while(num != 0)
  {
    num = num /10;
    i++;
  }
  i -= 1;
  while(i >= 0){
    no = pow(10,i);
    val = num_/no;
    num_ = num_%no;
    ch = val + '0';
    UART2_SendData8(ch);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
    i--;
  }
}
    