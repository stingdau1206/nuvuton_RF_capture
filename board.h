#ifndef board_h
#define board_h

#include "main.h"

#define SPEAKER_OUT_PIN P10
#define XINHAN_OUT_PIN  P11
#define LED_OUT_PIN     P06
#define LOCK_OUT_PIN    P07
#define PAIR_OUT_PIN    P30

void board_io_init(void);

void board_pwm_init(void);

#endif