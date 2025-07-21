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
 * Args: sel1=[0-6|255]&sel2=[0-6|255]
 * Usage: switch antenna */
void onSwitch(HTTPRequestParseState* pS)
{
	// arguments my not be fully filled
	const char *A = getHTTPArg(pS, "sel1");
	const char *B = getHTTPArg(pS, "sel2");
	uint8_t antnums[N_SELECTORS];
	char argname[8] = "sel";
	for(uint8_t i = 0; i < N_SELECTORS; ++i)
	{
		argname[3] = '1' + i;
		argname[4] = 0;
		const char* sV = getHTTPArg(pS, argname);
		uint8_t v = 255; // 255 means no change
		if(sV) v = atou8(sV); // if seli does not exist, treat it as 255
		antnums[i] = v;
	}

	int8_t r = switch_Antenna(antnums, N_SELECTORS);

	if(r == 0) // OK
	{
		//HTTPSendStr(pS, 200, "OK\r\n");
		HTTPredirect(pS, "/");
	}
	else
	{
		HTTPSendStr(pS, 300, "Invalid\r\n");
	}
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
			isModified = true;
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

/* URI: /setport
 * METHOD: POST
 * Args: portHTTP=80&portTCP=502
 * Usage: switch antenna */
void onSetPort(HTTPRequestParseState* pS)
{
	const uint8_t MAXLEN = 5;
	if(pS->argc < 2)
	{
		HTTPSendStr(pS, 300, "Bad args");
		return;
	}

	const char* s;
	uint16_t v = 0;
	s = getHTTPArg(pS, "portHTTP");
	if (s)
	{
		uint16_t len = strnlen(s, MAXLEN);
		if(len <= MAXLEN && len > 0)
		{
			v = atou16(s);
		}
	}
	if(v)
	{
		cfg.portHTTP = (uint16_t)v;
		isModified = true;
//		HTTPredirect(pS, "/");
	}
	else
	{
		HTTPSendStr(pS, 300, "Bad HTTP port number");
		return;
	}

	v = 0;
	s = getHTTPArg(pS, "portTCP");
	if (s)
	{
		uint16_t len = strnlen(s, MAXLEN);
		if(len <= MAXLEN && len > 0)
		{
			v = atou16(s);
		}
	}
	if(v)
	{
		cfg.portTCP = (uint16_t)v;
		isModified = true;
//		HTTPredirect(pS, "/");
	}
	else
	{
		HTTPSendStr(pS, 300, "Bad HTTP port number");
		return;
	}

	HTTPredirect(pS, "/");

//	EEPROM_WriteBytes(&EEPROM, (uint8_t*)&SavedData, sizeof(SavedData_typedef));
	//HTTPSendStr(pS, 200, "OK");

}


void onGetPort(HTTPRequestParseState* pS)
{
	static char s_tmp[9+5+9+5+1]; // portHTTP=65535&portTCP=65535
	char* s = s_tmp;
	uint16_t port;

	// MAXLEN = 9 + 5
	strcpy(s, "portHTTP=");
	s += 9;
	port = cfg.portHTTP;
	s += u16toa(port, s);

	// MAXLEN = 9 + 5
	strcpy(s, "&portTCP=");
	s += 9;
	port = cfg.portTCP;
	s += u16toa(port, s);

	*s = '\0';

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
		{.uri = "/setport", .func=onSetPort},
		{.uri = "/getport", .func=onGetPort},
};
