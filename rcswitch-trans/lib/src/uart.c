//uart.c

#include "uart.h"
#include "delay.h"

char uartstr[10];
uint32_t num = 100;
uint8_t status = 0;

void uart_config(void)
{
  UART2_DeInit();
  UART2_Init((u32)9600, UART2_WORDLENGTH_8D,UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE  ,UART2_MODE_TXRX_ENABLE);
  UART2_ITConfig(UART2_IT_RXNE, ENABLE);
  __enable_interrupt();
  UART2_Cmd(ENABLE);
}


void uart_print(char *str)
{
  while( (*str) )
  {
    UART2_SendData8(*str++);
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);    
  }
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


uint32_t str_to_u32(void)
{
  int i = 0, j = 0;
  uint32_t number = 0;
  while(uartstr[i++] != '\0');
  i = i - 2;
  while(i >= 0)
  {
    number += ((int)(uartstr[j++] - '0')) * pow(10,i);
    i--;
  }
  return number;
}

  
INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
  uint8_t i = 0;
  if(UART2_GetITStatus(UART2_IT_RXNE)!=RESET)
  {
    UART2_ClearFlag(UART2_IT_RXNE);
    uartstr[i] = UART2_ReceiveData8();
    while(uartstr[i++] != 'E')
    {
      while(UART2_GetITStatus(UART2_IT_RXNE)==RESET);{
        uartstr[i] = UART2_ReceiveData8();
      }
    }
    uartstr[i-1] = '\0';
  }
  num = str_to_u32();
  status = 1;
}