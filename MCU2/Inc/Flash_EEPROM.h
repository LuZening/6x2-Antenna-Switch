/*
 * Flash_EEPROM.h
 *
 *  Created on: 2019年9月28日
 *      Author: Zening
 */

#ifndef FLASH_EEPROM_H_
#define FLASH_EEPROM_H_

#define EEPROM_VALID_BYTE 0xAA
#define EEPROM_BASE_ADDR 0x0800BC00 // 1KB before FS

typedef struct
{
	uint8_t* base_addr; // 4-bit alignment!!!!!!! base_addr must be divisible by 4
	uint16_t size;
} EEPROM_typedef;
extern EEPROM_typedef EEPROM;
/*------------------------------------------------------------
 Func: EEPROM数据按字节写入
 Note:
-------------------------------------------------------------*/
void EEPROM_WriteBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length);
void EEPROM_ReadBytes(EEPROM_typedef* pEEPROM, uint8_t *Buffer,uint16_t Length);

#endif /* FLASH_EEPROM_H_ */
