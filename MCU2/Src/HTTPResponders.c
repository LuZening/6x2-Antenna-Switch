/*
 * HTTPResponders.c
 *
 *  Created on: 2019年9月22日
 *      Author: Zening
 */
#include <string.h>
#include "main.h"
#include "HTTPServer.h"
#include "FS.h"
#include "Flash_EEPROM/Flash_EEPROM.h"
#include "Config/Config.h"

/* URI: /
 * METHOD: GET
 * Usage: Homepage*/
void onHome(HTTPRequestParseState *pS)
{
	strcat(pS->URI, "index.html");
	HTTPonNotFound(pS);
}

/* URI: /switch
 * METHOD: POST
 * Args: sel1=[0-6]&sel2=[0-6]
 * Usage: switch antenna */
void onSwitch(HTTPRequestParseState* pS)
{
	const char *A = getHTTPArg(pS, "sel1");
	const char *B = getHTTPArg(pS, "sel2");
	uint8_t antnums[2];
	if(A && B)
	{
		antnums[0] = atou8(A);
		antnums[1] = atou8(B);
		if(antnums[0] <= NUM_ANTENNA && antnums[1] <= NUM_ANTENNA)
		{
			switch_Antenna(antnums, 2);
			//HTTPSendStr(pS, 200, "OK\r\n");
			HTTPredirect(pS, "/");
			return;
		}
	}
	HTTPSendStr(pS, 300, "Invalid\r\n");
}

/* URI: /getAlloc
 * METHOD: GET
 * Args: sel1=%d&sel2=%d
 * Usage: get current antenna allocation
 * */
void onGetAlloc(HTTPRequestParseState* pS)
{
	static char s_tmp[16];
	uint8_t antnums[N_SELECTORS];
	get_Antenna_real_BCDs(antnums, N_SELECTORS);
	char *p;
	p = s_tmp;
	strcpy(p, "sel1=");
	p+=5;
	*p = (antnums[0]) + '0'; // "sel1=%d"
	p++;
	strcpy(p, "&sel2="); // "sel1=%d&sel2="
	p+=6;
	*p = antnums[1] + '0'; // "sel1=%d&sel2=%d"
	p++;
	strcpy(p, "\r\n");
	p+=2;
	*p = 0;
	HTTPSendStr(pS, 200, s_tmp);
}

/* URI: /status
 * METHOD: GET
 * */
void onStatus(HTTPRequestParseState* pS)
{
	HTTPSendStr(pS, 200, "Status\r\n");
}

void onReset(HTTPRequestParseState* pS)
{
	HTTPSendStr(pS, 200, "Reset\r\n");
//	DEBUG_LOG("Restart\n");
}
/* URI: /getAlloc
 * METHOD: POST
 * Args: ant1=name&ant2=name&ant3=name&ant4=name&ant5=name&ant6=name
 * Usage: set antenna label*/
void onSetLabel(HTTPRequestParseState* pS)
{
	uint8_t i;
	char s_tmp[5] = "ant";
	if(pS->argc == 0)
	{
		HTTPSendStr(pS, 300, "Bad args");
		return;
	}
	for(i=1; i<=NUM_ANTENNA; ++i)
	{
		u16toa(i, s_tmp+3); //s_tmp = ant[i]
		const char* s_label;
		if((s_label = getHTTPArg(pS, s_tmp)) != NULL)
		{
			strncpy(cfg.sAntNames[i-1], s_label, MAX_LEN_ANT_LABEL);
		}
	}
//	EEPROM_WriteBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
	//HTTPSendStr(pS, 200, "OK");
	HTTPredirect(pS, "/");
}

void onGetLabel(HTTPRequestParseState* pS)
{
	static char s_tmp[(MAX_LEN_ANT_LABEL+5) * NUM_ANTENNA];
	uint8_t i;
	char* s = s_tmp;
	for(i=0; i<NUM_ANTENNA; ++i)
	{
		s = strcpy_f(s, "ant");
		*s = i + '1';
		s++;
		*s = '=';
		s++;
		s = strncpy_f(s, cfg.sAntNames[i], MAX_LEN_ANT_LABEL);
		isModified = true;
		if(i != NUM_ANTENNA-1)
		{
			*s = '&';
			s++;
		}
	}
	HTTPSendStr(pS, 200, s_tmp);
}

HTTPResponder_typedef HTTPResponders[] ={
		{.uri="/", .func=onHome},
		{.uri = "/switch", .func=onSwitch},
		{.uri = "/getalloc", .func=onGetAlloc},
		{.uri = "/status", .func=onStatus},
		{.uri = "/reset", .func=onReset},
		{.uri = "/setlabel", .func=onSetLabel},
		{.uri = "/getlabel", .func=onGetLabel},
};
