/*
 * Flash_EEPROM.c
 *
 *  Created on: 2019年9月28日
 *      Author: Zening
 */


#include "main.h"
#include "Flash_EEPROM.h"
#include "string.h"
EEPROM_typedef EEPROM = {.base_addr=(uint8_t*)EEPROM_BASE_ADDR, .size=0x400};

typedef union
{
	uint8_t bytes[4];
	uint32_t word;
} Quadbyte;

// Notice: 4-byte alignment
void EEPROM_WriteBytes(EEPROM_typedef *pEEPROM,uint8_t *Buffer,uint16_t Length)
{
	uint16_t i;
	uint32_t* addr = (uint32_t*)pEEPROM->base_addr;
	Quadbyte quadbyte;
	FLASH_EraseInitTypeDef fe = {.TypeErase=FLASH_TYPEERASE_PAGES, .PageAddress=(uint32_t)addr, .NbPages=1};
	uint32_t PageError = 0;
    __disable_irq();
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&fe, &PageError);
    while(Length)
    {
    	memset(quadbyte.bytes, 0, 4);
    	for(i=0; i<4 && Length; ++i)
    	{
    		quadbyte.bytes[i] = *Buffer;
    		Length--;
    		Buffer++;
    	}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addr++, quadbyte.word);
    }
    HAL_FLASH_Lock();
    __enable_irq();
}

void EEPROM_ReadBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length)
{
	uint8_t* addr = pEEPROM->base_addr;
	while(Length)
	{
		*Buffer = *addr;
		Buffer++;
		addr++;
		Length--;
	}
}
