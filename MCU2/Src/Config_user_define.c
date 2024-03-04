/*
 * Config_user_define.c
 *
 *  Created on: Sep 5, 2023
 *      Author: cpholzn
 */

#include <string.h>
#include "Config/Config.h"
#include "Config_user_define.h"

const char* FS_BASE_PATH_CONFIG = "/spiflash";
const char* FS_PATH_CONFIG_FILE = "/spiflash/cfg.bin";

const char* VALID_STRING = "BI4WOP";

// use '.' to indicate the positon in an array
// for '.' is well supported in HTML URL encoding
// either '()' or '[]' will be converted in HTML URL
// WARNING: names can't be longer than 8bytes
config_var_map_t configNameMapper[CONFIG_NUM_OF_VARS] = {
    {"sel.1", &(cfg.nRadioToAntNums[0]), CONFIG_VAR_U8},
    {"sel.2", &(cfg.nRadioToAntNums[1]), CONFIG_VAR_U8},
	{"label.1", &(cfg.sAntNames[0]), CONFIG_VAR_BYTESTRING_LONG},
	{"label.2", &(cfg.sAntNames[1]), CONFIG_VAR_BYTESTRING_LONG},
	{"label.3", &(cfg.sAntNames[2]), CONFIG_VAR_BYTESTRING_LONG},
	{"label.4", &(cfg.sAntNames[3]), CONFIG_VAR_BYTESTRING_LONG},
	{"label.5", &(cfg.sAntNames[4]), CONFIG_VAR_BYTESTRING_LONG},
	{"label.6", &(cfg.sAntNames[5]), CONFIG_VAR_BYTESTRING_LONG},
};

void init_config(Config *p)
{
    int i;
    #ifdef USE_MUTEX_ON_CFG
    if(mtxConfig == NULL) mtxConfig = xSemaphoreCreateMutex();
    xSemaphoreTake(mtxConfig, portMAX_DELAY);
    #endif
    // set valid string
    strncpy(p->sValid, VALID_STRING, 8);
    // set config inital values
    memset(p->sAntNames, 0, sizeof(p->sAntNames));
    memset(p->nRadioToAntNums, 0, sizeof(p->nRadioToAntNums));

    isModified = true;
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreGive(mtxConfig);
    #endif
}
