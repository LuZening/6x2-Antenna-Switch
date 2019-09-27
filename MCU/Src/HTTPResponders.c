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


/* URI: /
 * METHOD: GET
 * Usage: Homepage*/
void onHome(HTTPRequestParseState *pS)
{
	strcat(pS->URI, "index.html");
	HTTPonNotFound(pS);
}

/* URI: /switch
 * METHOD: GET
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

void onSetLabel(HTTPRequestParseState* pS)
{
}

void onGetLabel(HTTPRequestParseState* pS)
{
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
