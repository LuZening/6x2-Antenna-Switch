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
#ifdef __CH395_
#include "CH395.H"
#include "CH395CMD.H"
#define PHYNAME CH395
#endif
#include "FS.h"
#define PHY(method) PHYNAME##method
#define ENDSWITH(s,sub) (strcmp(s+strlen(s)-strlen(sub), sub)==0)

#define MAX_LEN_URI 128
#define MAX_NUM_ARGS 16
#define MAX_LEN_COOKIES 128


typedef enum
{
	HTTP_GET,
	HTTP_POST
} Method;


typedef enum
{
	CLOSED,
	KEEP_ALIVE
} HTTPConnection_typedef;


typedef struct {
	char* uri;
	Method method;
	UINT8 socket; // socket id
} HTTPServerRequest_typedef;
extern HTTPServerRequest_typedef _request;

typedef struct
{
	// 0: Start: Parsing method
	// 1: Parsing URI
	// 2: Parsing HTTP version
	// 3: Parsing headers
	// 4: Parsing header values
	UINT8 state;
	UINT8 parse_Connection_cursor;
	UINT8 parse_cursor;
	Method method;
	HTTPConnection_typedef connection;
	char URI[MAX_LEN_URI];
	UINT8 argc;
	char* argv[MAX_NUM_ARGS];
	char cookies[MAX_LEN_COOKIES];
	BOOL ready;
	UINT8 sock_index; // TODO: initialize sock_index
} HTTPRequestParseState;
extern HTTPRequestParseState parseState;

typedef void(*HTTPResponder_FuncType)(HTTPRequestParseState *) ;
typedef struct
{
	const char* uri;
	HTTPResponder_FuncType func;
} HTTPResponder_typedef;
#define NUM_HTTP_RESPONDERS 5
extern HTTPResponder_typedef HTTPResponders[NUM_HTTP_RESPONDERS];

const char* HTTPGetContentType(const char* filename);
char* strsepstr(char** stringp, const char* delim);
BOOL parse_http(HTTPRequestParseState *pS, char* buffer);
char* getHTTPArg(HTTPRequestParseState *pS, const char* name);
#ifdef __ON_BOARD_
void HTTPSendFile(HTTPRequestParseState *pS, int code, FSfile_typedef file);
void HTTPSendStr(HTTPRequestParseState* pS, int code, const char* content);
void HTTPHandle(CH395_TypeDef* pch395);
void HTTPonNotFound(HTTPRequestParseState *pS);
//void HTTPRegisterResponder(const char* uri, HTTPResponder_FuncType func);
#endif

#endif /* HTTPSERVER_H_ */
