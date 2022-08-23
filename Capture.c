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

static void find_lock_handle();
static void open_lock_handle();
static void close_lock_handle();

uint32_t u32Cmd, current_time;
uint8_t count_open = 0;
/*********************************************   ***************************************************************/
/*    Main function                                                                                         */
/************************************************************************************************************/
void main(void)
{
	millis_init();
	IR_Init();
	board_io_init();
	board_pwm_init();

	while (1)
	{
		LED_OUT_PIN = 0;
		current_time = millis();
		while (millis() - current_time < 20000)
		{
			if (IR_Check(&u32Cmd, 1))
			{
				switch (u32Cmd)
				{
				case FIND_LOCK:
				    LED_OUT_PIN = 1;
					current_time = millis();
					find_lock_handle();
					break;
				case OPEN_LOCK:
					count_open++;
					if(count_open >= 90)
						open_lock_handle();
					break;
				case CLOSE_LOCK:
					close_lock_handle();
					break;
				}
			}
		}

	}
}

static void find_lock_handle()
{
	int i;
	for (i = 0; i < 3; i++)
	{
		PWMRUN = 1;
		XINHAN_OUT_PIN = 1;
		delay_ms(1000);
		XINHAN_OUT_PIN = 0;
		PWMRUN = 0;
		delay_ms(500);
	}
}

static void open_lock_handle()
{
	PWMRUN = 1;
	XINHAN_OUT_PIN = 1;
	delay_ms(1000);
	XINHAN_OUT_PIN = 0;
	delay_ms(2000);
	PWMRUN = 0;
	LOCK_OUT_PIN = 0;
	count_open = 0;
}

static void close_lock_handle()
{
	int i;
	for(i = 0; i < 2; i++)
	{
		PWMRUN = 1;
		XINHAN_OUT_PIN = 1;
		delay_ms(1000);
		XINHAN_OUT_PIN = 0;
		PWMRUN = 0;
		delay_ms(500);
	}
	LOCK_OUT_PIN = 0;
}