/*
 * HTTPServer.h
 *
 *  Created on: Sep 13, 2019
 *      Author: Zening
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
//#define __ON_BOARD_
#include <main.h>
#include <stdbool.h>
#include "CH395.H"
#include "CH395CMD.H"
#define PHYNAME CH395
#include "FS.h"
#define PHY(method) PHYNAME##method
#define ENDSWITH(s,sub) (strcmp(s+strlen(s)-strlen(sub), sub)==0)

#define MAX_LEN_URI 64
#define MAX_NUM_ARGS 4
#define MAX_LEN_COOKIES 32
#define MAX_LEN_RESPONSE_HEADER 512
#define MAX_SIZE_PACK 1350 // maximum size of single TCP package in one transmission
#define NUM_SOCKETS CH395_SOCKS_AVAIL

#include "my_websocket.h"
#define MAX_HTTP_TICK_IDLE 32000 // ms, timeout for idle HTTP connection
#define TICK_NOW HAL_GetTick

typedef enum
{
	HTTP_GET,
	HTTP_POST
} Method;


typedef enum
{
	CLOSED,
	KEEP_ALIVE,
	UPGRADED_WS,
} HTTPConnection_typedef;

typedef enum
{
	RESPONSE_NOT_PREPARED= 0,
	RESPONSE_PREPARED,
	RESPONSE_CONTENT_REMAIN,
} HTTPResonseStage_typedef;


typedef struct
{
	// 0: Start: Parsing method
	// 1: Parsing URI
	// 2: Parsing HTTP version
	// 3: Parsing headers
	// 4: Parsing header values
	uint8_t state;	// RESET after use
	uint8_t parse_Connection_cursor; // RESET after use
	uint8_t parse_cursor;	// RESET after use
	Method method;
	HTTPConnection_typedef connection;
	char URI[MAX_LEN_URI]; // RESET after use
	uint16_t port;
	uint8_t argc;	// RESET after use
	char* argv[MAX_NUM_ARGS];
	char cookies[MAX_LEN_COOKIES];	// RESET after use
	//BOOL response_ready;	// RESET after use
	HTTPResonseStage_typedef response_stage;
	uint16_t len_response_header; // RESET after use
//	char response_header[MAX_LEN_RESPONSE_HEADER];
	char* response_header; // shared buffer
	uint16_t len_response_content_remain;
	const uint8_t* response_content;
	BOOL ready_for_making_response;	// ready for making response, reset after response has been transmitted
	uint8_t sock_index; //initialize sock_index
	ws_state_t ws_handshaked;
	WS_Frame ws; // websocket context
	uint32_t last_active_tick; // tick count of last activity, for timeout check
} HTTPRequestParseState;
extern HTTPRequestParseState parseStates[NUM_SOCKETS - 1];
extern char response_header_shared_buffer[MAX_LEN_RESPONSE_HEADER];
// The Socket number responding to, which serializes the process on multiple sockets


#define NUM_HTTP_RESPONDERS 9U
typedef void(*HTTPWSResponder_FuncType)(HTTPRequestParseState *) ;
typedef struct
{
	const char* uri;
	HTTPWSResponder_FuncType func;
} HTTPWSResponder_typedef;
extern HTTPWSResponder_typedef HTTPWSResponders[];

const char* HTTPGetContentType(const char* filename);
char* strsepstr(char** stringp, const char* delim);
BOOL parse_http(HTTPRequestParseState *pS, char* buffer);
const char* getHTTPArg(HTTPRequestParseState *pS, const char* name);
void HTTPSendFile(HTTPRequestParseState *pS, int code, FSfile_typedef file);
int WSMakeStrOriginal(char* buf, size_t lenbuf, const char* URI, const char* content);
// WSSendStr: non-reentrant
void WSSendStr(HTTPRequestParseState* pS, const char* URI, const char* content);
void HTTPSendStr(HTTPRequestParseState* pS, int code, const char* content);
int HTTPSendWebSocketHandshakeResponse(HTTPRequestParseState* pS, char* client_key, size_t lenKey);
void HTTPHandle(CH395_TypeDef* pch395);
void HTTPredirect(HTTPRequestParseState *pS, const char* URI);
void HTTPonNotFound(HTTPRequestParseState *pS);
void resetHTTPParseState(HTTPRequestParseState *pS);
void resetHTTPParseState_for_long_connection(HTTPRequestParseState *pS);

void activateHTTPParseState(HTTPRequestParseState *pS);
void HTTPclose(uint8_t i); // Sock Index to disconnect
void HTTPclose_for_long_connection(uint8_t i);
//void HTTPRegisterResponder(const char* uri, HTTPResponder_FuncType func);
int8_t getNextSock();
uint8_t atou8(const char* s);
uint16_t atou16(const char* s);
uint8_t u16toa(uint16_t, char*);
char* strcpy_f(char* dest, const char* src); // fast strcpy
char* strncpy_f(char* dest, const char* src, uint16_t len);
uint8_t IPv4_to_s(char* __restrict s ,const  uint8_t* __restrict IP);
#endif /* HTTPSERVER_H_ */
