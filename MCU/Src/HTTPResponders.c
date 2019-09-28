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
#include "Flash_EEPROM.h"

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
 * Args: A=[0-6]&B=[0-6]
 * Usage: switch antenna */
void onSwitch(HTTPRequestParseState* pS)
{
	const char *A = getHTTPArg(pS, "A");
	const char *B = getHTTPArg(pS, "B");
	if(A && B)
	{
		//  TODO: remove stdlib
		uint8_t nA = atou8(A);
		uint8_t nB = atou8(B);
		if(nA <= NUM_ANTENNA && nB<=NUM_ANTENNA)
		{
			switch_Antenna(nA, nB);
			HTTPSendStr(pS, 200, "OK\r\n");
			return;
		}
	}
	HTTPSendStr(pS, 300, "Invalid\r\n");
}

/* URI: /getAlloc
 * METHOD: GET
 * Args:
 * Usage: get current antenna allocation*/
void onGetAlloc(HTTPRequestParseState* pS)
{
	static char s_tmp[10];
	uint8_t d = get_Antenna();
	char *p;
	p = s_tmp;
	strcpy(p, "A=");
	p+=2;
	*p = (uint8_t)(d & 0xf) + '0'; // "A=%d"
	p++;
	strcpy(p, "&B="); // "A=%d&B="
	p+=3;
	*p = (uint8_t)(d >> 4) + '0'; // "A=%d&B=%d"
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
// TODO: set get label
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
		char* s_label;
		if((s_label = getHTTPArg(pS, s_tmp)) != NULL)
		{
			strncpy(SavedData.ant_labels[i-1], s_label, MAX_LEN_ANT_LABEL);
		}
	}
	EEPROM_WriteBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
	HTTPSendStr(pS, 200, "OK");
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
		s = strncpy_f(s, SavedData.ant_labels[i], MAX_LEN_ANT_LABEL);
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
