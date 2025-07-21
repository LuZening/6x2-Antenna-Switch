#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "Config_user_define.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#define USE_MUTEX_ON_CFG 1
#endif


extern const char* FS_BASE_PATH_CONFIG;
extern const char* FS_PATH_CONFIG_FILE;

extern const char* VALID_STRING;

typedef unsigned char byte;




union ConfigWriteBlock
{
    Config body;
    uint8_t bytes[sizeof(Config)];
};

/* Config name->pointer mapper  */
typedef enum
{
    CONFIG_VAR_U8 = 0,
    CONFIG_VAR_I32,
	CONFIG_VAR_BYTESTRING,
    CONFIG_VAR_BYTESTRING_LONG,
	CONFIG_VAR_OBJ,
} config_var_type_t;

typedef struct
{
    const char* name;
    void* pV;
    config_var_type_t typ;
} config_var_map_t;

extern config_var_map_t configNameMapper[CONFIG_NUM_OF_VARS];

#ifdef USE_MUTEX_ON_CFG
extern SemaphoreHandle_t mtxConfig;
#endif

extern bool isModified;

void load_config(Config* p_cfg);
// if config read from EEPROM is not 
void init_config(Config *p);


void save_config(Config* p_cfg);

bool config_check_valid(Config* p);

// set the value of the config variable by name
bool set_config_variable_by_name(const char* name, const void* pV);
// get the value of the config variable by name
// @name the name of the config variable
// @buf the memory to store the value of the variable. If the variable's value is a string, the size of the buf must >= 32 
bool get_config_variable_by_name(const char* name, void* buf);
bool get_if_config_modified();
// get the pointer to the config variable by name
// return NULL if not found
config_var_map_t* get_config_variable_mapper_item_by_name(const char* name);

// Sync working objects, such as RotSensor and etc, with config variables
// call after config variables are updated
void push_config_to_volatile_variables(Config* p);


int get_config_string(char *buf, int lenbuf);
