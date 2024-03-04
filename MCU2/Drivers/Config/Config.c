#include "Config.h"


#if defined(ESP32)
#define tag "config"
#include "esp_log.h"
#endif
#include <string.h>
#include "main.h"
#include "string.h"

#if defined(USE_STM32_FLASH_EEPROM)
#include "Flash_EEPROM/Flash_EEPROM.h"
#endif


Config cfg;

#ifdef USE_MUTEX_ON_CFG
SemaphoreHandle_t mtxConfig = NULL;
#endif

bool isModified = false;

void push_config_to_volatile_variables(Config* p)
{
    int i;
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreTake(mtxConfig, portMAX_DELAY);
    #endif
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreGive(mtxConfig);
    #endif
}

// flash wear levelling handle
#ifdef ESP32
static wl_handle_t wlHandle = WL_INVALID_HANDLE;
#endif
void load_config(Config* p_cfg)
{
    #ifdef USE_MUTEX_ON_CFG
    if(mtxConfig == NULL) mtxConfig = xSemaphoreCreateMutex();
    #endif

    #if defined(USE_STM32_FLASH_EEPROM)
    EEPROM_ReadBytes(&EEPROM, (uint8_t*)p_cfg, sizeof(Config));
    #elif defined(USE_24C_EEPROM)
    EEPROM_ReadRange(&eeprom, ADDR_CONFIG_BEGIN, p_cfg->bytes, sizeof(Config));
    #elif defined(ESP32)
    // Use FatFS by default
    esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(FS_BASE_PATH_CONFIG, "storage", &mount_config, &wlHandle);
    if (err != ESP_OK) {
        ESP_LOGE(tag, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        return;
    }
    ESP_LOGD(tag, "Opening file");
    FILE *f = fopen(FS_PATH_CONFIG_FILE, "rb");
    if (f == NULL) {
        ESP_LOGE(tag, "Failed to open file %s for reading", FS_PATH_CONFIG_FILE);
    }
    else
    {
        // load file content to config variable
        ESP_LOGD(tag, "Read from file %s", FS_PATH_CONFIG_FILE);
        size_t n = fread(p_cfg->bytes, sizeof(Config), 1, f);
        fclose(f);
        ESP_LOGD(tag, "Read from file %s succeeded, %d bytes", FS_PATH_CONFIG_FILE, n);
        ESP_LOGD(tag, "cfg.validstring = %s", p_cfg->body.sValid);
    }
    esp_vfs_fat_spiflash_unmount(FS_BASE_PATH_CONFIG, wlHandle);    
    ESP_LOGD(tag, "FS unmounted");
    #endif
}


void save_config(Config* p_cfg)
{
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreTake(mtxConfig, portMAX_DELAY);
    #endif

    #if defined(USE_STM32_FLASH_EEPROM)
    EEPROM_WriteBytes(&EEPROM, (uint8_t*)p_cfg, sizeof(Config));
    #elif defined(USE_24C_EEPROM)
    EEPROM_release_WP(&eeprom);
    // ACK_polling_AT24C(pEEPROM);
    // write_array_AT24C(pEEPROM, 1, p_cfg->bytes, sizeof(ConfigWriteBlock));
    
    EEPROM_WriteRange(&eeprom, ADDR_CONFIG_BEGIN, sizeof(Config), p_cfg->bytes);
    EEPROM_WP(&eeprom);
    #elif defined(ESP32)
    // Use FatFS by default
    esp_vfs_fat_mount_config_t mount_config = {
        .max_files = 4,
        .format_if_mount_failed = true,
        .allocation_unit_size = CONFIG_WL_SECTOR_SIZE,
    };
    esp_err_t err = esp_vfs_fat_spiflash_mount(FS_BASE_PATH_CONFIG, "storage", &mount_config, &wlHandle);
    if (err != ESP_OK) {
        ESP_LOGE(tag, "Failed to mount FATFS (%s)", esp_err_to_name(err));
        goto SAVE_CONFIG_FAILED;
    }
    ESP_LOGD(tag, "Opening file");
    FILE *f = fopen(FS_PATH_CONFIG_FILE, "wb");
    if (f == NULL) {
        ESP_LOGE(tag, "Failed to open file %s for writing", FS_PATH_CONFIG_FILE);
    }
    else
    {
        // load file content to config variable
        ESP_LOGD(tag, "Dump to file %s", FS_PATH_CONFIG_FILE);
        ESP_LOGD(tag, "cfg.validstring = %s", p_cfg->body.sValid);
        size_t n = fwrite(p_cfg->bytes, sizeof(Config), 1, f);
        fclose(f);
        ESP_LOGD(tag, "Dump to file %s succeeded, %d bytes", FS_PATH_CONFIG_FILE, n);
    }
    esp_vfs_fat_spiflash_unmount(FS_BASE_PATH_CONFIG, wlHandle);    
    ESP_LOGD(tag, "FS unmounted");
    #endif

    isModified=false;

SAVE_CONFIG_FAILED:
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreGive(mtxConfig);
    #endif
    return;
}


bool config_check_valid(Config* p)
{
    uint8_t check = strncmp(p->sValid, VALID_STRING, sizeof(p->sValid));
    return (check == 0);

}


bool set_config_variable_by_name(const char* name, const void* pV)
{
    const int N = (sizeof(configNameMapper) / sizeof(config_var_map_t));
    int i;
    bool found = false;
    // match the config entry by comparing names
    for(i = 0; i < N; ++i)
    {
        if(strcmp(configNameMapper[i].name, name) == 0)
        {
            found = true;
            break;
        }
    }
    if(found)
    {
        const config_var_map_t* pMap = &(configNameMapper[i]);
        #ifdef USE_MUTEX_ON_CFG
        xSemaphoreTake(mtxConfig, portMAX_DELAY);
        #endif
        switch (pMap->typ)
        {
        case CONFIG_VAR_U8:
            *(uint8_t*)(pMap->pV) = *(uint8_t*)pV;
//            ESP_LOGD(tag, "set %s = %d", name, *(uint8_t*)pV);
            break;
        case CONFIG_VAR_I32:
            memcpy(pMap->pV, pV, sizeof(int));
//            ESP_LOGD(tag, "set %s = %d", name, *(int32_t*)pV);
            break;
        case CONFIG_VAR_BYTESTRING:
            strncpy((char*)(pMap->pV), (char*)pV, CONFIG_BYTESTRING_LEN);
//            ESP_LOGD(tag, "set %s = %s", name, (char*)pV);
            break;
        case CONFIG_VAR_BYTESTRING_LONG:
            strncpy((char*)(pMap->pV), (char*)pV, CONFIG_BYTESTRING_LONG_LEN);
//            ESP_LOGD(tag, "set %s = %s", name, (char*)pV);
            break;
        default:
            break;
        }
        isModified = true;
        #ifdef USE_MUTEX_ON_CFG
        xSemaphoreGive(mtxConfig);
        #endif
    }
    return found;
}


bool get_if_config_modified()
{
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreTake(mtxConfig, portMAX_DELAY);
    #endif
    bool r = isModified;
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreGive(mtxConfig);
    #endif
    return r;
}


config_var_map_t* get_config_variable_mapper_item_by_name(const char* name)
{
    const int N = (sizeof(configNameMapper) / sizeof(config_var_map_t));
    int i;
//    bool found = false;
    config_var_map_t* pFound = NULL;
    // match the config entry by comparing names
    for(i = 0; i < N; ++i)
    {
        if(strncmp(configNameMapper[i].name, name, CONFIG_VAR_NAME_LEN) == 0)
        {
//            found = true;
            pFound = &(configNameMapper[i]);
            break;
        }
    }
    return pFound;
}

bool get_config_variable_by_name(const char* name, void* buf)
{
    const int N = (sizeof(configNameMapper) / sizeof(config_var_map_t));
    int i;
    bool found = false;
    // match the config entry by comparing names
    for(i = 0; i < N; ++i)
    {
        if(strncmp(configNameMapper[i].name, name, CONFIG_VAR_NAME_LEN) == 0)
        {
            found = true;
            break;
        }
    }
    if(found)
    {
        const config_var_map_t* pMap = &(configNameMapper[i]);
        #ifdef USE_MUTEX_ON_CFG
        xSemaphoreTake(mtxConfig, portMAX_DELAY);
        #endif
        switch (pMap->typ)
        {
        case CONFIG_VAR_U8:
            *(uint8_t*)buf = *(uint8_t*)(pMap);
            break;
        case CONFIG_VAR_I32:
            memcpy(buf, pMap->pV, sizeof(int));
            break;
        case CONFIG_VAR_BYTESTRING:
            strncpy((char*)buf, (char*)(pMap->pV), CONFIG_VAR_BYTESTRING);
            break;
        case CONFIG_VAR_BYTESTRING_LONG:
            strncpy((char*)buf, (char*)(pMap->pV), CONFIG_VAR_BYTESTRING_LONG);
            break;
        }
        #ifdef USE_MUTEX_ON_CFG
        xSemaphoreGive(mtxConfig);
        #endif
    }
    return found;
}

int get_config_string(char* buf, int lenbuf)
{
#if defined(ESP32)
    const int N = (sizeof(configNameMapper) / sizeof(config_var_map_t));
    int i;
    char* p = buf;
    int nWritten = 0;
    int nW = 0;
    if (lenbuf <= 1)
        return 0;
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreTake(mtxConfig, portMAX_DELAY);
    #endif
    for(i = 0; i < N; ++i)
    {
        config_var_map_t *pMap = &(configNameMapper[i]);
        if(nWritten >= lenbuf - 1)
            break;
        switch(pMap->typ)
        {
            case CONFIG_VAR_U8:
            nW = snprintf(p, lenbuf - nWritten - 1, 
                "%s=%d", pMap->name, *((uint8_t*)(pMap->pV)));
            break;
            case CONFIG_VAR_I32:
            nW = snprintf(p, lenbuf - nWritten - 1, 
                "%s=%d", pMap->name, *((int*)(pMap->pV)));
            break;
            case CONFIG_VAR_BYTESTRING:
            nW = snprintf(p, lenbuf - nWritten - 1, 
                "%s=%s", pMap->name, ((char*)(pMap->pV)));
            break;
            case CONFIG_VAR_BYTESTRING_LONG:
			nW = snprintf(p, lenbuf - nWritten - 1,
							"%s=%s", pMap->name, ((char*)(pMap->pV)));
			break;
        }
        nWritten += nW;
        p += nW;
        if(nWritten < lenbuf - 1)
        {
            if(i == N-1)
            {
                *p = '\n';
            }
            else
            {
                *p = '&';
            }
            nWritten++;
            p++;
        }
    }
    buf[nWritten] = 0; // terminate the string
    #ifdef USE_MUTEX_ON_CFG
    xSemaphoreGive(mtxConfig);
    #endif
    return nWritten;
#else
    return 0;
#endif

}
