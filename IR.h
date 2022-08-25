#ifndef MY_IR_H_
#define MY_IR_H_
// A7D1A2/4/8
#define FIND_LOCK 			0x0A7D1A8
#define OPEN_LOCK 			0x0A7D1A4
#define CLOSE_LOCK 			0x0A7D1A2
#define ADDRESS_APROM  	0x3FF0
#include "main.h"

void IR_Init(void);

uint8_t IR_Check(uint32_t *pu32Cmd, uint8_t pair_mode);

#endif