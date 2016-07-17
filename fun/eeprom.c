#ifndef __EEPROM_C
#define __EEPROM_C

#include "eeprom.h"

uint8_t sla;

void EEInit(uint8_t deviceSLA)
{
	sla= deviceSLA;
}

uint8_t EERead_Byte(uint16_t adr)
{
	uint8_t data;
	
	I2C1_read16_buf(sla, adr, 1, &data);
	
	return data;
}
char* EERead_String(uint16_t defAdr)
{
	uint8_t buff[3];
	I2C1_read16_buf(sla, defAdr, 3, buff);
}

void EEWrite_Byte(uint16_t adr, uint8_t data);
void EEWrite_String(uint16_t defAdr, char* str);

#endif /* __EEPROM_C */
