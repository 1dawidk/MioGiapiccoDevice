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


void EEClear(void)
{
	uint8_t zeros[2];
	uint16_t i;	
	
	zeros[0]=0;
	zeros[1]=0;	
	
	for(i=0; i<EEPROM_SIZE_BYTES/2; i++)
	{
		I2C2_write16_buf(sla , i*2, 2, zeros);
		_delay_ms(5);
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
}



char* EERead_String(uint16_t adr)
{
	char *readResult;
	uint8_t buff;

	if(adr<EEPROM_STRING_MAX_CNT)
	{
		adr= (adr*EEPROM_STRING_MAX_LEN) + EEPROM_STRING_ADR_START;
		
		_delay_ms(20);
		I2C2_read16_buf(sla, adr, 1, &buff);
		if(buff>0)
		{
			_delay_ms(20);
			readResult= malloc(sizeof(char)*buff);
			I2C2_read16_buf(sla, adr+1, buff, (uint8_t *)readResult);
			readResult[buff]=0;
		}
		else
		{
				readResult=malloc(sizeof(char));
				readResult[0]=0;
		}
		
		return readResult;
	}
	
	return (char*)0;
}

uint16_t EEWrite_String(uint16_t adr, char* str)
{
	uint8_t i;
	uint8_t len= strlen(str)+1;
	uint16_t errorCode=0;
	
	if(adr<EEPROM_STRING_MAX_CNT)
	{
		adr= (adr*EEPROM_STRING_MAX_LEN) + EEPROM_STRING_ADR_START;
	
		if(len<EEPROM_STRING_MAX_LEN)
		{	
			errorCode+=I2C2_write16_buf(sla, adr, 1, &len);
			for(i=0; i<len; i++)
			{
				_delay_ms(5);
				errorCode+=I2C2_write16_buf(sla, adr+1+i, 1, str+i);
			}
		}
	}
	
	return errorCode;
}

void EEShow_All(void)
{
	uint16_t i;
	
	UART1_putStr(" ###### EEPROM MEMORY MAP ######\r\n");
	
	for(i=0; i<4096; i++)
	{
		if(!(i%20))
		{
			UART1_putStr("||");
			UART1_putInt(i-(i%20));
			UART1_putStr("\r\n");
		}
	
			uart1_putc(EERead_Byte(i));
		
		UART1_putStr("\t");
	}
	
	UART1_putStr(" ###############################");
}

#endif /* __EEPROM_C */
