//buffer.c


#include "buffer.h"
#include "uart.h"

void entry_buffer(uint32_t val)
{
  //dataAvailable(); 
  if(val != last_val){
  buffer[cur_pointer++] = val;
  }
  
  last_val = val;
  
  if(cur_pointer == 64){
    cur_pointer = 0;
    buffer_full = 1;
  }
}


uint32_t entry_next(uint16_t index)
{
  return buffer[index];
}


void delimeter()
{
  UART2_SendData8('|');
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}


void empty_buffer()
{
  uint16_t index = 0;
  uint32_t data;
  if(buffer_full == 1){
    last_pointer = 64;
  }
  else{
    last_pointer = cur_pointer;
  }
  
  for(index=0; index<last_pointer; index++){
    data = entry_next(index);
    u32_to_str(data);
    delimeter();
  }
  
  cur_pointer = 0;
  last_pointer = 0;
  buffer_full = 0;
  last_val = 0;
}



INTERRUPT_HANDLER(UART2_RX_IRQHandler, 21)
{
    if(UART2_GetFlagStatus(UART2_FLAG_TXE) == SET){
      if(UART2_ReceiveData8() == 'R'){
        empty_buffer();
      }
    }
 }