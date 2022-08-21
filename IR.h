#ifndef MY_IR_H_
#define MY_IR_H_
// A7D1A2/4/8
#define BUTTON1 0x0A7D1A2
#define BUTTON2 0x0A7D1A4
#define BUTTON3 0x0A7D1A8

#include "main.h"

void IR_Init(void);
uint8_t IR_Check(uint32_t *pu32Cmd);

#endif