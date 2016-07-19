#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "I2c/I2CDrv.h"
#include "stdlib.h"
#include "string.h"
#include "coreInit.h"
#include "stdio.h"

#define EEPROM_WRITE							0
#define EEPROM_READ								0
#define EEPROM_BLOCK0							0
#define EEPROM_BLOCK1							0x02
#define EEPROM_STRING_SIZE				32
#define EEPROM_SLA								0x

void EEInit(uint8_t deviceSLA);

uint8_t EERead_Byte(uint16_t adr);
char* EERead_String(uint16_t adr);

void EEWrite_Byte(uint16_t adr, uint8_t data);
void EEWrite_String(uint16_t adr, char* str);

#endif /* __EEPROM_H */
