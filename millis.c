#include "millis.h"
uint32_t tick_ms = 0;

#define TH0_INIT        0xC1
#define TL0_INIT        0xE0

void Timer0_ISR (void) interrupt 1  //interrupt address is 0x000B
{
    _push_(SFRS);

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;
    tick_ms++;

    _pop_(SFRS);
}

void millis_init(void)
{
    ENABLE_TIMER0_MODE1;
    TIMER0_FSYS;

    TH0 = TH0_INIT;
    TL0 = TL0_INIT;

    ENABLE_TIMER0_INTERRUPT;                       //enable Timer0 interrupt
    ENABLE_GLOBAL_INTERRUPT;                       //enable interrupts

    set_TCON_TR0;                                  //Timer0 run
}

uint32_t millis(void)
{
    return tick_ms;
}

void delay_ms(uint32_t ms)
{
    int i = 0;
    for(i = 0; i < ms; i++)
    {
        _delay_();
    }
}