#ifndef __EEPROM_C
#define __EEPROM_C

#include "eeprom.h"

uint8_t sla;

void EEInit(uint8_t deviceSLA)
{
	sla= deviceSLA;
	I2C2_init();
}

uint8_t EERead_Byte(uint16_t adr)
{
	uint8_t data;
	
	I2C2_read16_buf(sla | EEPROM_BLOCK1, adr, 1, &data);
	
	return data;
}

void EEWrite_Byte(uint16_t adr, uint8_t data)
{
	I2C2_write16_buf(sla  | EEPROM_BLOCK1, adr, 1, &data);
}



char* EERead_String(uint16_t adr)
{
	char *readResult;
	uint8_t buff;

	adr*=EEPROM_STRING_SIZE;
	
	I2C2_read16_buf(sla | EEPROM_BLOCK0, adr, 1, &buff);
	readResult= (char *)malloc(sizeof(char)*buff);
	I2C2_read16_buf(sla | EEPROM_BLOCK0, adr+1, buff, readResult);
	
	return readResult;
}

void EEWrite_String(uint16_t adr, char* str)
{
	uint8_t i;
	uint8_t len= strlen(str)+1;
	
	adr*=EEPROM_STRING_SIZE;
	
	if(len<EEPROM_STRING_SIZE)
	{	
		I2C2_write16_buf(sla | EEPROM_BLOCK0, adr, 1, &len);
		for(i=0; i<len; i++)
		{
			_delay_ms(10);
			I2C2_write16_buf(sla | EEPROM_WRITE | EEPROM_BLOCK0, adr+1+i, 1, &(str[i]));
		}
	}
}

#endif /* __EEPROM_C */
