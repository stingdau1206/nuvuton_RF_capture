#ifndef button.h
#define button.h

#include "main.h"

typedef enum
{
    REALESED,
    SHORT_PRESSED,
    LONG_PRESSED,
}button_state_t;

button_state_t button_handle(sbit pin);

#endif