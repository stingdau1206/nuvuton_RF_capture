#ifndef MY_IR_H_
#define MY_IR_H_
// A7D1A2/4/8
#define FIND_BUTTON 		0x0A7D1A4
#define KEY_BUTTON 	    0x0A7D1A2
#define SPEAKER_BUTTON 	0x0A7D1A8

#define STATE_IDLE 0
#define STATE_RECEIVE 1
#define STATE_CHECK_BIT_0 2
#define STATE_CHECK_BIT_1 3
#define STATE_ENCODE_SUCC 4
#define STATE_END 5

#include "main.h"

void IR_Init(void);

uint8_t IR_Check(uint32_t *pu32Cmd, uint8_t encode);

#endif