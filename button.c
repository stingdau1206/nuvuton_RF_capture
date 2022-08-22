#include "button.h"
#include "millis.h"
#include "board.h"

#define DEBOUNCE_DELAY 15
#define PRESS_HOLD_TIME 2000

uint32_t lastDebounceTime = 0;
uint8_t preState;
uint8_t flag_button = 0;
uint8_t currentState = 1;
uint8_t lastAfterFilterNoiseState = 0;
uint32_t state_time;

void INT0_ISR(void) interrupt 2          // Vector @  0x03
{
    _push_(SFRS);

    clr_TCON_IE1;          //clr int flag wait next falling edge
    preState = 1;
    flag_button = 1;

    _pop_(SFRS);
}

void button_init()
{
		P17_INPUT_MODE;
    INT1_FALLING_EDGE_TRIG;              //setting trig condition level or edge
    set_IE_EX1;                         //INT0_Enable;
    ENABLE_GLOBAL_INTERRUPT;            //Global interrupt enable
}

button_state_t button_handle()
{
    if (flag_button)
    {
        currentState = PAIR_IN_PIN;
        if (currentState != lastAfterFilterNoiseState)
        {
            lastDebounceTime = millis();
            lastAfterFilterNoiseState = currentState;
        }
        if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
        {
            if (preState == 1 && currentState == 0)
            {
                preState = currentState;
                return SHORT_PRESSED;
            }
            else if (preState == 0 && currentState == 0)
            {
                if (millis() - lastDebounceTime > PRESS_HOLD_TIME)
                {
                    flag_button = 0;
                    return LONG_PRESSED;
                }
            }
            else
            {
                flag_button = 0;
                return REALESED;
            }
        }
    }
		return NONE;
}
