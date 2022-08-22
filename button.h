#ifndef button_h
#define button_h

#include "main.h"

typedef enum
{
    REALESED,
    SHORT_PRESSED,
    LONG_PRESSED,
	NONE
}button_state_t;

button_state_t button_handle();

#endif
