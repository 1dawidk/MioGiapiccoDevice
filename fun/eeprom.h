#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "I2c/I2CDrv.h"
#include "stdlib.h"
#include "string.h"
#include "coreInit.h"
#include "stdio.h"

#define EEPROM_SIZE_BYTES					8192
#define EEPROM_BYTE_ADR_START			0
#define EEPROM_STRING_ADR_START		4096
#define EEPROM_STRING_MAX_LEN			256
#define EEPROM_STRING_MAX_CNT			16

void EEInit(uint8_t deviceSLA);

void EEClear(void);

uint8_t EERead_Byte(uint16_t adr);
char* EERead_String(uint16_t adr);

void EEWrite_Byte(uint16_t adr, uint8_t data);
void EEWrite_String(uint16_t adr, char* str);

#endif /* __EEPROM_H */
