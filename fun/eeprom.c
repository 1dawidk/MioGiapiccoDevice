#ifndef __EEPROM_C
#define __EEPROM_C

#include "eeprom.h"

uint8_t sla;

void EEInit(uint8_t deviceSLA)
{
	sla= deviceSLA;
	I2C2_init();
	_delay_ms(2000);
}

void EE_Repoint()
{
	uint8_t zero=0;
	_delay_ms(20);
	I2C2_write16_buf(sla , 0, 0, &zero);
}

void EEClear(void)
{
	uint8_t zeros[32];
	uint16_t i;
	
	for(i=0; i<32; i++)
	{
		zeros[i]=0;
	}
	
	for(i=0; i<EEPROM_SIZE_BYTES/32; i++)
	{
		I2C2_write16_buf(sla , i*32, 32, zeros);
		_delay_ms(10);
	}
}

uint8_t EERead_Byte(uint16_t adr)
{
	uint8_t data;
	
	_delay_ms(5);
	I2C2_read16_buf(sla , adr+EEPROM_BYTE_ADR_START, 1, &data);
	
	return data;
}

void EEWrite_Byte(uint16_t adr, uint8_t data)
{
	_delay_ms(20);
	I2C2_write16_buf(sla , adr+EEPROM_BYTE_ADR_START, 1, &data);
	EE_Repoint();
}



char* EERead_String(uint16_t adr)
{
	char *readResult;
	uint8_t buff;

	if(adr<EEPROM_STRING_MAX_CNT)
	{
		adr= (adr*EEPROM_STRING_MAX_LEN) + EEPROM_STRING_ADR_START;
		
		I2C2_read16_buf(sla, adr, 1, &buff);
		_delay_ms(20);
		readResult= malloc(sizeof(char)*buff);
		I2C2_read16_buf(sla, adr+1, buff, (uint8_t *)readResult);
		
		EE_Repoint();
		
		return readResult;
	}
	
	return (char*)0;
}

void EEWrite_String(uint16_t adr, char* str)
{
	uint8_t i;
	uint8_t len= strlen(str)+1;
	
	if(adr<EEPROM_STRING_MAX_CNT)
	{
		adr= (adr*EEPROM_STRING_MAX_LEN) + EEPROM_STRING_ADR_START;
	
		if(len<EEPROM_STRING_MAX_LEN)
		{	
			I2C2_write16_buf(sla, adr, 1, &len);
			for(i=0; i<len; i++)
			{
				_delay_ms(20);
				I2C2_write16_buf(sla, adr+1+i, 1, (uint8_t *)&(str[i]));
			}
			
			EE_Repoint();
		}
	}
}

#endif /* __EEPROM_C */
