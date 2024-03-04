/*
 * Config_params.h
 *
 *  Created on: Sep 5, 2023
 *      Author: cpholzn
 */

#ifndef CONFIG_PARAMS_H_
#define CONFIG_PARAMS_H_

#include <stdint.h>

// by default use EEPROM 24C64 with 8KBytes capacity
//#define USE_24C_EEPROM
#define USE_STM32_FLASH_EEPROM

/* do not use float parameters */
// type of variables: uint8_t, int, char
#define CONFIG_VAR_NAME_LEN 8
#define CONFIG_BYTESTRING_LEN 8
#define CONFIG_BYTESTRING_LONG_LEN 16

#define CONFIG_NUM_OF_VARS 8

typedef struct
{
    // check if storage is valid
    char sValid[CONFIG_BYTESTRING_LEN];
    // welding params
    uint8_t nRadioToAntNums[2];
    char sAntNames[6][CONFIG_BYTESTRING_LONG_LEN];

} Config; // size of config is around 192Bytes



extern Config cfg;


#endif /* CONFIG_PARAMS_H_ */
