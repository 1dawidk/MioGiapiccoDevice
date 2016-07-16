#ifndef _COREINIT_H
#define	_COREINIT_H

#include "stm32f10x.h"
#include "def\def.h"
#include "def\pin_descr.h"

typedef struct
{
	uint32_t us;
	uint32_t s;
} SystemTimestamp;

void RCC_config(void);
void GPIO_config(void);
void NVIC_config(void);
uint32_t SysTick_Init(uint32_t ticks);
void RTC_Init(void);
void _delay_us(uint32_t val);
void _delay_ms(uint32_t ms);
void _delay_s(uint8_t s);

SystemTimestamp getTimeStamp(void);
uint32_t getDuration(SystemTimestamp from);

void CoreInit(void);

#endif
