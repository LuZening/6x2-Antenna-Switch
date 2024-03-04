/*
 * commands.c
 *
 *  Created on: 2023年3月2日
 *      Author: cpholzn
 */


#include "commands.h"
#include "command_funcs_OTRSP.h"
#include <stdint.h>
#include <string.h>
#include "strsep.h"

#define MIN(x,y) ((x < y)?(x):(y))



CommandParser_t CommandParser;


void command_parser_init(CommandParser_t *p, CommandProtocol_t proto)
{
	p->protocol = proto;
	p->hasResponse = 0;
}

size_t utoaz(uint32_t v, char* buf, int8_t nDigits)
{
	size_t i = 0;
	while(v || (nDigits > 0))
	{
		buf[i++] = (v % 10) + '0';
		v /= 10;
		nDigits--;
	}
	uint8_t j = 0;
	char c;
	for(j=0; j < i / 2; ++j)
	{
		c = buf[j];
		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = c;
	}
	buf[i] = 0;
	return i;
}

size_t itoaz(int32_t v, char* buf, int8_t nDigits)
{
	size_t i = 0;
	bool isNeg =false;
	if(v < 0)
	{
	    v = -v;
	    isNeg = true;
	}
	while((v != 0) || (nDigits > 0))
	{
		buf[i++] = (((uint32_t)v) % 10) + '0';
		v /= 10;
		nDigits--;
	}
    if(isNeg) buf[i++] = '-';
	uint8_t j = 0;
	char c;
	for(j=0; j < i / 2; ++j)
	{
		c = buf[j];
		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = c;
	}
	buf[i] = 0;
	return i;
}

err_t my_atou(const char* buf, int8_t nMaxLen, uint32_t* retval)
{
	const char* p = buf;
	uint32_t v = 0;
	while(*p && (nMaxLen > 0))
	{
        uint8_t d = *p - '0';
        if(d > 9) goto failed;
		v = v * 10 + (*p - '0');
		p++;
		nMaxLen--;
	}
    *retval = v;
    return ERR_OK;
failed:
    return ERR_FAILED;
}

err_t my_atoi(const char* buf, int8_t nMaxLen, int32_t* retval)
{
	const char* p = buf;
	int32_t v = 0;
	bool isNeg;
	if(*p == '-')
	{
		isNeg = true;
		p++;
	}
	while(*p && (nMaxLen > 0))
	{
		v = v * 10 + (*p - '0');
		p++;
		nMaxLen--;
	}

	*retval = (isNeg ? (-v) : v);

    return ERR_OK;
failed:
    return ERR_FAILED;
}








int execute_command_string(CommandParser_t* pParser, const char* s, size_t len)
{
    int r = -1;
    switch(pParser->protocol)
    {
    case COMMAND_PROTOCOL_OTRSP:
    	pParser->bufRet[0] = 0;
    	r = parse_command_OTRSP(s, len, pParser->bufRet);
    	if(pParser->bufRet[0]) // parsed is valid
    		pParser->hasResponse = strnlen(pParser->bufRet, sizeof(pParser->bufRet));
    	break;
    default:
    	break;
    }
    return r;
}



