#ifndef _STATIC_MEMORY_DRV_H
#define _STATIC_MEMORY_DRV_H

#include "stm32f10x.h"

#define STATICMEMORY_PAGE_SIZE			0x400
#define STATICMEMORY_START_ADR			0x801f800

typedef enum {READ_FROM_VALID_PAGE, WRITE_IN_VALID_PAGE} ValidPageFor;
typedef enum {Erased, ReceiveData, ValidPage} StaticMemoryState;

void StaticMemory_Init(void);
void StaticMemory_Format(void);
void StaticMemory_Write(uint16_t vAdr, uint16_t data);
uint16_t StaticMemory_Read(uint16_t vAdr);

#endif
