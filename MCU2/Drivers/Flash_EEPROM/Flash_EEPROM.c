/*
 * Flash_EEPROM.c
 *
 *  Created on: 2019年9月28日
 *      Author: Zening
 */


#include "main.h"
#include "Flash_EEPROM.h"
#include <string.h>
#include <stdint.h>

#if defined(STM32F0)
	#include "stm32f0xx_hal_flash.h"
	const uint16_t PAGE_SIZE = 2 * 1024;// 2KB
	EEPROM_typedef EEPROM = {.base_addr=(uint32_t*)EEPROM_BASE_ADDR, .size=0x400};
#elif defined(STM32G0)
	#include "stm32g0xx_hal_flash.h"
	const uint16_t PAGE_SIZE = 2 * 1024;// 2KB
	EEPROM_typedef EEPROM = {.base_addr=(uint8_t*)EEPROM_BASE_ADDR, .size=0x400};
	#define FLASH_TYPEPROGRAM_WORD FLASH_TYPEPROGRAM_FAST
#elif defined(STM32F4)
	// TODO: implement
	const uint16_t PAGE_SIZE = 128 * 1024;// 128KB
	EEPROM_typedef FlashEEPROM = {.base_addr=(uint8_t*)(FLASH_LAST_SECTOR_BASE_ADDR), .size=FLASH_LAST_SECTOR_SIZE};
#endif

typedef union
{
	uint8_t bytes[4];
	uint32_t word;
} Quadbyte;


// Notice: 4-byte alignment
void EEPROM_WriteBytes(EEPROM_typedef *pEEPROM, uint8_t *Buffer,uint16_t Length)
{
	uint16_t i;
	uint32_t* addr = (uint32_t*)pEEPROM->base_addr;
	uint32_t* addrPageEnd =(addr + (PAGE_SIZE >> 2));
	Quadbyte quadbyte;

	uint32_t* addrOldContentBegin = addr;
	uint32_t* addrOldContentEnd;
	uint32_t* addrNewContentBegin = addr;
#if ENABLE_WEAR_LEVELING
	/* detect range of old content, non-zero non-0xff range */
	// find the head
	while((addrOldContentBegin < addrPageEnd) && (*addrOldContentBegin == 0x00000000U))
		addrOldContentBegin++;
	// find the tail
	addrOldContentEnd = addrOldContentBegin;
	while((addrOldContentEnd < addrPageEnd) && (*addrOldContentEnd != 0xFFFFFFFFU))
		addrOldContentEnd++;


#endif
    __disable_irq();
    HAL_FLASH_Unlock();

#if defined(STM32F4)
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
		FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
#endif
	uint32_t PageError = 0;
#if defined(STM32F4)
	FLASH_EraseInitTypeDef fe = {.TypeErase=FLASH_TYPEERASE_SECTORS,
			.Sector = addr_to_sector(addr),
			.NbSectors=1,
			.VoltageRange=FLASH_VOLTAGE_RANGE_3};
#else
	FLASH_EraseInitTypeDef fe = {.TypeErase=FLASH_TYPEERASE_PAGES, .PageAddress=(uint32_t)addr, .NbPages=1};
#endif


#if ENABLE_WEAR_LEVELING
	// if the remaining length cannot fit the new content, erase the full page
	// add 4bytes to leave room for header
	if(addrOldContentEnd + (Length >> 2) + 1 > addrPageEnd)
	{
		HAL_FLASHEx_Erase(&fe, &PageError);
		addrNewContentBegin = addr;
	}
	/* SET EXISTING DATA TO ALL 0 TO MARK USAGE */
	else
	{
		for(uint32_t* addrToErase = addrOldContentBegin; addrToErase < addrOldContentEnd; addrToErase++)
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addrToErase, 0x00000000U);
		addrNewContentBegin = addrOldContentEnd;
	}

#else
	/* If Wear Leveling is disabled, erase the full page each time before writing*/
	HAL_FLASHEx_Erase(&fe, &PageError);
#endif

	/* WRITE HEADER */
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addrNewContentBegin++, EEPROM_SYMBOL_HEADER);
	/* WRITE CONTENT BEGIN */
    while(Length)
    {
    	memset(quadbyte.bytes, 0, 4);
    	for(i=0; i<4 && Length; ++i)
    	{
    		quadbyte.bytes[i] = *Buffer;
    		Length--;
    		Buffer++;
    	}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)addrNewContentBegin++, quadbyte.word);
    }
    /* WRITE CONTENT END */
    HAL_FLASH_Lock();
    __enable_irq();
}


void EEPROM_ReadBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length)
{
//	uint8_t* ad = pEEPROM->base_addr;
//	uint8_t* addr_end = addr + Length;
	Quadbyte quadbyte;
	uint32_t* addrBase= (uint32_t*)pEEPROM->base_addr;
	uint32_t* addrPageEnd = addrBase + (PAGE_SIZE >> 2);
	uint32_t* addrContentBegin = addrBase;
#if ENABLE_WEAR_LEVELING
	memset(Buffer, 0, Length);
	// read 4-bytes each
	// find header, header
	while(addrContentBegin < addrPageEnd && *addrContentBegin == 0x00000000U)
		addrContentBegin++;
	// skip the header
	if(addrContentBegin < addrPageEnd - 1)
		addrContentBegin += 1;
#else
	addrContentBegin = addrBase + 1; // skip HEAD
	addrContentEnd = addrContentBegin + Length;
#endif
	uint32_t* addr = addrContentBegin;
	while(Length && (addr < addrPageEnd))
	{
		quadbyte.word = *addr;
		for(uint8_t i=0; i<4 && Length; ++i)
		{
			*Buffer = quadbyte.bytes[i];
			Buffer++;
			Length--;
		}
		addr++;
	}
}
