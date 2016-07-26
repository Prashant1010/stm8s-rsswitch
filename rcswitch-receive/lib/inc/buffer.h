//buffer.h

#ifndef __buffer_h__
#define __buffer_h__

#include "stm8s.h"

void entry_buffer(uint32_t val);
uint32_t entry_next(uint16_t index);
void empty_buffer(void);


uint32_t buffer[64];
uint16_t cur_pointer = 0;
uint16_t last_pointer = 0;
uint32_t last_val = 0;
uint8_t dataStatus = 0;
int buffer_full = 0;

#endif