#include "board.h"

void board_io_init(void)
{
    P10_PUSHPULL_MODE;
    P11_PUSHPULL_MODE;
    P06_PUSHPULL_MODE;
    P07_PUSHPULL_MODE;

    SPEAKER_OUT_PIN = 1;
    LED_OUT_PIN = 0;
    LOCK_OUT_PIN = 0;
    XINHAN_OUT_PIN = 0;
}

void board_pwm_init(void)
{
	CLRPWM = 1;
	CKCON &= ~(1 << 6);
	PWMCON1 &= 0x07;
	PWMCON1 |= 0x00;
	PWMPH = 0x12;
	PWMPL = 0xef;
	PWM3H = 0x9;
	PWM3L = 0x78;
	P0M1 &= ~(1 << 0);
	P0M2 |= (1 << 0);
	PIOCON0 |= (1 << 3);
	// CLRPWM = 1;
	// CKCON &= ~(1 << 6);
	// PWMCON1 &= 0x07;
	// PWMCON1 |= 0x00;
	// PWMPH = 0x3e;
	// PWMPL = 0x7f;
	// PWM3H = 0x1f;
	// PWM3L = 0x40;
	// P0M1 &= ~(1 << 0);
	// P0M2 |= (1 << 0);
	// PIOCON0 |= (1 << 3);
	PWMRUN = 0;
}