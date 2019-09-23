/*
 * HTTPResponders.c
 *
 *  Created on: 2019年9月22日
 *      Author: Zening
 */

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
	char *A, *B;
	if(A = getHTTPArg(pS, "A") && B = getHTTPArg(pS, "B"))
	{
		uint8_t nA = atoi(A);
		uint8_t nB = atoi(B);
		if(nA >=0 && nB>=0 && nA <= NUM_ANTENNA && nB<=NUM_ANTENNA)
		{
			swith_Antenna(nA, nB);
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
	sprintf(ch395.buffer, "A=%d&B=%d\r\n", Selector[0].sel, Selector[1].sel);
	HTTPSendStr(pS, 200, ch395.buffer);
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
	DEBUG_LOG("Restart\n");
}

HTTPResponder_typedef HTTPResponders[] ={
		{.uri="/", .func=onHome},
		{.uri = "/switch", .func=onSwitch},
		{.uri = "/getAlloc", .func=onGetAlloc},
		{.uri = "/status", .func=onStatus},
		{.uri - "/reset", .func=onReset},
};
