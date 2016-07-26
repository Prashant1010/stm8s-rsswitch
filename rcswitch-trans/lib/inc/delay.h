//delay.h

#ifndef __delay_h__
#define __delay_h__

#include "stm8s.h"

void timer_init(void);
void delay_us(uint32_t us);
void delay_ms(uint16_t ms);
#endif