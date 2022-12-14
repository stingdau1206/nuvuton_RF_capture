#include "IR.h"
#include "board.h"
#include "millis.h"
#include "eeprom.h"

#define IR_IN_VAL P15

#define NUM_BIT 24

#define START_MIN 1400
#define START_MAX 1500
#define PRIMARY_MIN 580
#define PRIMARY_MAX 680
#define SECOND_MIN 150
#define SECOND_MAX 250

static void tim2_init(void);
static void cap_init(void);
extern uint8_t pair_flag;

#define STATE_IDLE 0
#define STATE_RECEIVE 1
#define STATE_CHECK_BIT_0 2
#define STATE_CHECK_BIT_1 3
#define STATE_DECODE_SUCC 4

typedef void (*pfunc)(void);

uint32_t button_state[4] = {0};
uint8_t index = 0;

void idle_state(void);
void receive_state(void);
void check_bit_0_state(void);
void check_bit_1_state(void);
void decode_succ_state(void);

static pfunc funcSet[] = {
	idle_state,
	receive_state,
	check_bit_0_state,
	check_bit_1_state,
	decode_succ_state};

static uint8_t u8State = STATE_IDLE;
static uint8_t u8Count = 0;
static uint32_t u32Result = 0;

void idle_state(void)
{
	uint16_t u16Tmp;
	if (IR_IN_VAL)
	{
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp > START_MIN && u16Tmp < START_MAX)
		{
			u8Count = 0;
			u32Result = 0;
			u8State = STATE_RECEIVE;
		}
		else
		{
			u8State = STATE_IDLE;
		}
	}
}

void receive_state(void)
{
	uint16_t u16Tmp;
	if (!IR_IN_VAL)
	{
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp > SECOND_MIN && u16Tmp < SECOND_MAX)
		{
			u32Result <<= 1;
			++u8Count;
			u8State = STATE_CHECK_BIT_0;
		}
		else if (u16Tmp > PRIMARY_MIN && u16Tmp < PRIMARY_MAX)
		{
			u32Result <<= 1;
			u32Result |= 1;
			++u8Count;
			u8State = STATE_CHECK_BIT_1;
		}
		else
		{
			u8State = STATE_IDLE;
		}
	}
}

void check_bit_0_state(void)
{
	uint16_t u16Tmp;
	if (IR_IN_VAL)
	{
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp > PRIMARY_MIN && u16Tmp < PRIMARY_MAX)
		{
			u8State = STATE_RECEIVE;
		}
		else
		{
			u8State = STATE_IDLE;
		}
	}
}

void check_bit_1_state(void)
{
	uint16_t u16Tmp;
	if (IR_IN_VAL)
	{
		u16Tmp = C0H * 256 + C0L;
		if (u16Tmp > SECOND_MIN && u16Tmp < SECOND_MAX)
		{
			u8State = STATE_RECEIVE;
		}
		else
		{
			u8State = STATE_IDLE;
		}
	}
}

void decode_succ_state(void)
{
	int i;
	if (button_state[index] != u32Result)
	{
		if (index < 1)
		{
			button_state[index] = u32Result;
			index++;
		}
		else if (button_state[index - 1] != u32Result)
		{
			button_state[index] = u32Result;
			index++;
		}
		for(i = 0; i < index*2; i++)
		{
			PAIR_OUT_PIN = !PAIR_OUT_PIN;
			delay_ms(200);
		}
		if(index == 3)
		{
			index = 0;
			for(i = 0; i < 3; i++)
			{
				Write_DATAFLASH_ARRAY(ADDRESS_APROM + 4*i, (uint8_t *)&button_state[i], 4);
			}
		}
	}
}

static void tim2_init(void)
{
	TIMER2_CAP0_Capture_Mode;

	TIMER2_DIV_16;
	set_T2CON_TR2;
}

static void cap_init(void)
{
	IC7_P15_CAP0_BOTHEDGE_CAPTURE;
}

void IR_Init(void)
{
	int i;
	u8State = STATE_IDLE;
	u32Result = 0;
	u8Count = 0;
	cap_init();
	tim2_init();
	for(i = 0; i < 3; i++)
	{
		Read_DATAFLASH_ARRAY(ADDRESS_APROM + 4*i, (uint8_t *)&button_state[i], 4);
	}
}

uint8_t IR_Check(uint32_t *pu32Cmd, uint8_t pair_mode)
{
	PAIR_OUT_PIN = !pair_mode;
	if (TF2)
	{
		TF2 = 0;
		u8State = STATE_IDLE;
	}
	if (CAPCON0 & (1 << 0))
	{
		funcSet[u8State]();  
		CAPCON0 &= ~(1 << 0);
		if (u8Count == NUM_BIT)
		{
			u8Count = 0;
			if(!pair_mode)
			{
				decode_succ_state();
			}
			else
			{
				*pu32Cmd = u32Result;
			}
			u8State = STATE_IDLE;
			return 1;
		}
	}
	return 0;
}