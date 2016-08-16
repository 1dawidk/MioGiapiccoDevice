#ifndef __EEPROM_C
#define __EEPROM_C

#include "eeprom.h"

uint8_t sla;
uint8_t currentPageNo;
uint32_t pagesAdr[EE_PAGES_NO];
uint16_t offset;

static uint32_t FindVarAddress(uint16_t idx, uint8_t page)
{
	uint32_t varAdr=0;
	uint16_t cnt=1;
	uint16_t varIdx;
	uint16_t varLen;
	
	while(cnt*4<FLASH_PAGE_SIZE && (*(uint32_t *)(pagesAdr[page]+(cnt*4)))!=0xffffffff)
	{
		varIdx= *(uint16_t *)(pagesAdr[page]+(cnt*4));
		varLen= *(uint16_t *)(pagesAdr[page]+(cnt*4)+2);
		
		if(varIdx==idx)
			varAdr= pagesAdr[page]+(cnt*4);
		
		cnt+= varLen+1;
	}
	
	return varAdr;
}

static uint32_t FindMaxLessThan(uint32_t startAdr, uint16_t pageSize, uint32_t lessThan)
{
	uint32_t max=0;
	uint32_t maxAdr=0;
	uint16_t cnt=1;
	uint16_t varLen;
	uint16_t varIdx;
	
	while(cnt*4<pageSize && (*(uint32_t *)(startAdr+(cnt*4)))!=0xffffffff)
	{
		varIdx= *(uint16_t *)(startAdr+(cnt*4));
		varLen= *(uint16_t *)(startAdr+(cnt*4)+2);
		
		if(varIdx>=max && varIdx<lessThan)
		{
			max= varIdx;
			maxAdr= startAdr+(cnt*4);
		}
		
		cnt+= varLen+1;
	}
	
	return maxAdr;
}

static uint16_t FindOffset(uint8_t page, uint16_t pageSize)
{
	uint16_t cnt=1;
	uint16_t varLen;
	
	while(((cnt*4)<pageSize) && ((*(uint32_t *)(pagesAdr[page]+(cnt*4)))!=0xffffffff))
	{
		varLen= *(uint16_t *)(pagesAdr[page]+(cnt*4)+2);
		cnt+= 1+varLen;
	}
	
	return cnt-1;
}

static uint8_t IsEnoughtMemory(uint16_t pageSize, uint16_t varLen)
{
	if(offset+varLen+2<pageSize)
		return 1;
	else
		return 0;
}

static uint8_t ChangePage(uint8_t page)
{
	uint8_t nextPage=page+1;
	uint32_t varAdr=0;
	uint16_t varIdx=0xffff;
	uint16_t varLen=0;
	uint16_t cnt;
	
	if(nextPage==EE_PAGES_NO)
		nextPage=0;
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ErasePage(pagesAdr[nextPage]);
	FLASH_ProgramWord(pagesAdr[nextPage], EE_PAGE_CURRENT);
	offset=0;
	
	//Rewrite variabels
	varAdr= FindMaxLessThan(pagesAdr[page], FLASH_PAGE_SIZE, varIdx);
	while(varAdr!=0)
	{
		varLen= *(uint16_t *)(varAdr+2);
		varIdx= *(uint16_t *)varAdr;
	
		for(cnt=0; cnt<1+varLen; cnt++)
		{
			FLASH_ProgramWord(pagesAdr[nextPage]+((offset+1)*4), *(uint32_t *)(varAdr+(cnt*4)));
		}
		
		offset+= 1+varLen;
		
		varAdr= FindMaxLessThan(pagesAdr[page], FLASH_PAGE_SIZE, varIdx);
	}
	
	FLASH_Lock();
	
	return nextPage;
}

void EEInit(void)
{
	uint8_t cnt;
	currentPageNo=250;
	
	for(cnt=0; cnt<EE_PAGES_NO; cnt++)
	{
		pagesAdr[cnt]= FLASH_EE_START_ADR+ (FLASH_PAGE_SIZE*cnt);
		if((*(uint32_t *)pagesAdr[cnt])==EE_PAGE_CURRENT)
		{
			currentPageNo=cnt;
			offset= FindOffset(cnt, FLASH_PAGE_SIZE);
		}
	}
	
	if(currentPageNo==250)
	{
		FLASH_Unlock();
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		
		FLASH_ErasePage(pagesAdr[0]);
		FLASH_ProgramWord(pagesAdr[0], EE_PAGE_CURRENT);
		offset=0;
		currentPageNo=0;
		
		FLASH_Lock();
	}
}


uint8_t EERead_Byte(uint16_t idx)
{
	
}

void EEWrite_Byte(uint16_t idx, uint8_t data)
{
	uint32_t varTitle= ((uint32_t)idx)<<16 | 0x00000001;
	
	if(!IsEnoughtMemory(FLASH_PAGE_SIZE, 1))
		currentPageNo= ChangePage(currentPageNo);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ProgramWord(pagesAdr[currentPageNo]+(1+offset)*4, varTitle);
	FLASH_ProgramWord(pagesAdr[currentPageNo]+(2+offset)*4, (uint32_t)data);
	offset+=2;
	FLASH_Lock();
}

char* EERead_String(uint16_t idx)
{
	uint32_t varAdr= FindVarAddress(idx, currentPageNo);	
	uint16_t varLen;
	uint8_t *charPtr;
	char *str;
	uint8_t cnt=0;
	
	if(varAdr)
	{
		varLen= *(uint16_t *)(varAdr+2);
		str=malloc(varLen*4);
		
		charPtr= (uint8_t *)(varAdr+4);
		while(charPtr[cnt]!=0)
		{
			str[cnt]=charPtr[cnt];
			cnt++;
		}
		
		str[cnt]=0;
	}
	
	return str;
}

void EEWrite_String(uint16_t idx, char* str)
{
	uint32_t varLen= strlen(str)&0x0000ffff;
	uint32_t varTitle= ((uint32_t)idx)<<16 | varLen/4;
	uint16_t cnt;
	uint32_t memTmp;
	uint16_t lastChar;
	
	if(varLen%4)
		varTitle++;
	
	if(!IsEnoughtMemory(FLASH_PAGE_SIZE, varLen/4))
		currentPageNo= ChangePage(currentPageNo);
	
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	FLASH_ProgramWord(pagesAdr[currentPageNo]+(1+offset)*4, varTitle);
	
	for(cnt=0; cnt<varLen/4; cnt++)
	{
		memTmp= ((uint32_t)str[cnt*4]) | ((uint32_t)str[(cnt*4)+1])<<8 | ((uint32_t)str[(cnt*4)+2])<<16 | ((uint32_t)str[(cnt*4)+3])<<24;
		FLASH_ProgramWord(pagesAdr[currentPageNo]+((2+offset)*4)+(cnt*4), memTmp);
	}
	
	if(varLen%4)
	{
		lastChar= cnt*4;	
		memTmp=0;
		for(cnt=0; cnt<varLen%4; cnt++)
		{
			memTmp |= ((uint32_t)str[lastChar+cnt])<<(cnt)*8;
		}
		FLASH_ProgramWord(pagesAdr[currentPageNo]+(2+offset)*4+lastChar, memTmp);
		offset++;
	}
	
	offset+=varLen/4;
	offset++;
	
	FLASH_Lock();
}
#endif /* __EEPROM_C */
