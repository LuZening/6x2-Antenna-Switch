/*
 * Flash_EEPROM_user_define.h
 *
 *  Created on: 2023年9月12日
 *      Author: cpholzn
 */

#ifndef FLASH_EEPROM_USER_DEFINE_H_
#define FLASH_EEPROM_USER_DEFINE_H_



#define ENABLE_WEAR_LEVELING 1
#define EEPROM_SYMBOL_HEADER 0x07CB020AU

/* 4KB before FS, not being too close to FS
 * ------------WEAR LEVELING STRATEGY -------------
 * PAGE: find first non-zero block, write
*/
#define EEPROM_BASE_ADDR 0x0800B000

#endif /* FLASH_EEPROM_USER_DEFINE_H_ */
