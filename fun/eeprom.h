#ifndef __EEPROM_H
#define __EEPROM_H

#include "stm32f10x.h"
#include "I2c/I2CDrv.h"
#include "stdlib.h"
#include "string.h"
#include "coreInit.h"
#include "stdio.h"

#define EE_PAGES_NO								2

#define FLASH_PAGE_SIZE						2048
#define EEPROM_DATA_MAX_LEN				256

#define EE_PAGE_CLEAR							12
#define EE_PAGE_NEXT							11
#define EE_PAGE_CURRENT						10

void EEInit(void);

uint8_t EERead_Byte(uint16_t idx);
char* EERead_String(uint16_t idx);

void EEWrite_Byte(uint16_t idx, uint8_t data);
void EEWrite_String(uint16_t idx, char* str);


#endif /* __EEPROM_H */
