//uart.h

#ifndef __uart_h__
#define __uart_h__

#include "stm8s_uart2.h"
#include "stm8s_clk.h"

void uart_config(void);
void uart_print(char *str);
extern uint32_t num;
extern uint8_t status;
#endif