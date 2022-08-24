/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2020 nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

/***********************************************************************************************************/
/* Website: http://www.nuvoton.com                                                                         */
/*  E-Mail : MicroC-8bit@nuvoton.com                                                                       */
/*  Date   : Jan/21/2020                                                                                   */
/***********************************************************************************************************/

/************************************************************************************************************/
/*  File Function: MS51 Timer 2 capture function demo                                                       */
//***********************************************************************************************************/
#include "IR.h"
#include "millis.h"
#include "board.h"

#define PERIOD_OPEN 700
#define PERIOD_CLOSE 300

static void speaker_handle(uint8_t state);
static void open_lock_handle();
static void close_lock_handle();

uint32_t u32Cmd, current_time, open_time;
uint8_t count_key = 0;
uint8_t count_speaker = 0;
uint8_t count_find = 0;
uint8_t key_flag = 0;
uint8_t speaker_flag = 0;
uint8_t led_flag = 0;
uint8_t find_flag = 0;
uint8_t speaker_state = 0;
uint8_t key_state = 0;
/*********************************************   ***************************************************************/
/*    Main function                                                                                         */
/************************************************************************************************************/
void main(void)
{
	millis_init();
	IR_Init();
	board_io_init();
	board_pwm_init();

	set_WDCON_WDCLR; // Clear WDT timer

	while (1)
	{
		set_WDCON_WDCLR;

		current_time = millis();
		if (find_flag)
		{
			LED_OUT_PIN = !LED_OUT_PIN;
			if (count_find < 6)
			{
				PWMRUN = !PWMRUN;
				XINHAN_OUT_PIN = !XINHAN_OUT_PIN;
			}
			else
			{
				PWMRUN = 0;
				XINHAN_OUT_PIN = 0;
			}
			count_find++;
			if (count_find >= 40)
			{
				PWMRUN = 0;
				XINHAN_OUT_PIN = 0;
				LED_OUT_PIN = 0;
				count_find = 0;
				find_flag = 0;
			}
		}
		while (millis() - current_time < 500)
		{
			set_WDCON_WDCLR;
			if (IR_Check(&u32Cmd, 1))
			{
				switch (u32Cmd)
				{
				case FIND_BUTTON:
					find_flag = 1;
					count_find = 0;
					break;
				case KEY_BUTTON:
					count_key++;
					if (key_state == 0)
					{
						if (count_key >= 70)
						{
							open_lock_handle();
							LED_OUT_PIN = 1;
							open_time = millis();
							led_flag = 1;
						}
					}
					else
					{
						close_lock_handle();
					}
					break;
				case SPEAKER_BUTTON:
					count_speaker++;
					if (count_speaker >= 40)
					{
						count_speaker = 0;
						speaker_state = !speaker_state;
						speaker_handle(speaker_state);
					}
					break;
				}
			}
		}
		if(key_state == 1 && (millis() - open_time) > 10000 && led_flag == 1)
		{
			led_flag = 0;
			LED_OUT_PIN = 0;
			PWMRUN = 1;
			delay_ms(PERIOD_OPEN);
			PWMRUN = 0;
			delay_ms(PERIOD_CLOSE);
			set_WDCON_WDCLR;
			PWMRUN = 1;
			delay_ms(PERIOD_OPEN);
			PWMRUN = 0;
		}
	}
}

static void open_lock_handle()
{
	set_WDCON_WDCLR;
	LED_OUT_PIN = 1;
	PWMRUN = 1;
	XINHAN_OUT_PIN = 1;
	delay_ms(PERIOD_OPEN);
	XINHAN_OUT_PIN = 0;
	set_WDCON_WDCLR;
	delay_ms(PERIOD_OPEN);
	set_WDCON_WDCLR;
	delay_ms(PERIOD_OPEN);
	PWMRUN = 0;
	LOCK_OUT_PIN = 1;
	count_key = 0;
	key_state = 1;
	LED_OUT_PIN = 0;
}

static void close_lock_handle()
{
	int i;
	for (i = 0; i < 2; i++)
	{
		set_WDCON_WDCLR;
		PWMRUN = 1;
		XINHAN_OUT_PIN = 1;
		delay_ms(PERIOD_OPEN);
		XINHAN_OUT_PIN = 0;
		PWMRUN = 0;
		delay_ms(PERIOD_CLOSE);
	}
	count_find = 40;
	LED_OUT_PIN = 0;
	key_state = 0;
	LOCK_OUT_PIN = 0;
}

static void speaker_handle(uint8_t state)
{
	if (state)
	{
		PWMRUN = 1;
		XINHAN_OUT_PIN = 1;
		LED_OUT_PIN = 1;
		delay_ms(PERIOD_OPEN);
		PWMRUN = 0;
		XINHAN_OUT_PIN = 0;
		LED_OUT_PIN = 0;
		delay_ms(PERIOD_CLOSE);
		set_WDCON_WDCLR;
		XINHAN_OUT_PIN = 1;
		LED_OUT_PIN = 1;
		delay_ms(PERIOD_OPEN);
		XINHAN_OUT_PIN = 0;
		LED_OUT_PIN = 0;
		delay_ms(PERIOD_CLOSE);
		set_WDCON_WDCLR;
		XINHAN_OUT_PIN = 1;
		LED_OUT_PIN = 1;
		delay_ms(PERIOD_OPEN);
		XINHAN_OUT_PIN = 0;
		LED_OUT_PIN = 0;
		set_WDCON_WDCLR;
	}
	else
	{
		PWMRUN = 1;
		delay_ms(PERIOD_OPEN);
		set_WDCON_WDCLR;
		delay_ms(PERIOD_OPEN);
		set_WDCON_WDCLR;
		delay_ms(PERIOD_OPEN);
		PWMRUN = 0;
		delay_ms(PERIOD_CLOSE);
		set_WDCON_WDCLR;
		PWMRUN = 1;
		XINHAN_OUT_PIN = 1;
		delay_ms(PERIOD_OPEN);
		PWMRUN = 0;
		XINHAN_OUT_PIN = 0;
		set_WDCON_WDCLR;
	}
}