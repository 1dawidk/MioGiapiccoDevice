#ifndef _STATIC_MEMORY_DRV_C
#define _STATIC_MEMORY_DRV_C

#include "StaticMemoryDrv.h"

void StaticMemory_Format(void)
{
	FLASH_Unlock();
	
	FLASH_ErasePage(STATICMEMORY_START_ADR);
	
	FLASH_Lock();
}

void StaticMemory_Write(uint16_t vAdr, uint16_t data)
{
	uint16_t offset= vAdr*2;
	FLASH_Unlock();
	
	if(vAdr<=256)
	{
			FLASH_ProgramHalfWord(STATICMEMORY_START_ADR, 0);
	}
	
	FLASH_Lock();
}

uint16_t StaticMemory_Read(uint16_t vAdr)
{
	uint16_t offset= vAdr*2;
	return *((uint16_t *)STATICMEMORY_START_ADR+offset);
}

#endif
