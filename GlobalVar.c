#ifndef GLOBALVAR_C_
#define GLOBALVAR_C_

#include "stm32f10x.h"

volatile uint32_t seconds;
volatile uint8_t runMode;
volatile char rxLocker[512];

#endif
