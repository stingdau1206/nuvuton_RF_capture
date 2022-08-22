#ifndef millis_h
#define millis_h

#include "main.h"

void millis_init(void);

uint32_t millis(void);

void delay_ms(uint32_t ms);

#endif