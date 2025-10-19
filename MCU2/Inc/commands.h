#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>


typedef int8_t err_t;
#define ERR_OK 0
#define ERR_FAILED -1
#define LEN_COMMANDS_RET_BUF 24

#define MSG_BAD_COMMAND "bad command\r"
#define N_ANTENNAS 6
#define N_RADIOS 2


typedef enum
{
	COMMAND_PROTOCOL_OTRSP =0,
} CommandProtocol_t;

typedef struct
{
    CommandProtocol_t protocol;
    volatile uint16_t hasResponse;
    char bufRet[LEN_COMMANDS_RET_BUF];
    /* data */
} CommandParser_t;

extern CommandParser_t CommandParser;

void command_parser_init(CommandParser_t *p, CommandProtocol_t proto);

size_t utoaz(uint32_t v, char* buf, int8_t nDigits);
size_t itoaz(int32_t v, char* buf, int8_t nDigits);
err_t my_atou(const char* buf, int8_t nMaxLen, uint32_t* retval);
err_t my_atoi(const char* buf, int8_t nMaxLen, int32_t* retval);
int execute_command_string(CommandParser_t* pParser, const char* s, size_t len);
