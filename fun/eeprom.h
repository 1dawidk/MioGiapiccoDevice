#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "I2c/I2CDrv.h"

void EEInit(uint8_t deviceSLA);

uint8_t EERead_Byte(uint16_t adr);
char* EERead_String(uint16_t defAdr);

void EEWrite_Byte(uint16_t adr, uint8_t data);
void EEWrite_String(uint16_t defAdr, char* str);

#endif /* __EEPROM_H */
