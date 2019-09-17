/*
 * HTTPServer.h
 *
 *  Created on: Sep 13, 2019
 *      Author: Zening
 */

#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_
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

/*	HTTP String Constants	*/
const char HTTP_ITEM_STR_CONNECTION[] = "Connection";
const char HTTP_ITEM_STR_COOKIES[] = "Cookie";
const char HTTP_ITEM_STR_POST[] = "POST";
const char HTTP_ITEM_STR_GET[] = "GET";
const char HTTP_LINE_DELIM[] = "\r\n";
const char HTTP_DELIM[] = " ";
const char HTTP_COLUMN_DELIM = ": ";
const char HTTP_STR_VERSION = "HTTP/1.1";
const char HTTP_CONTENT_TYPE_PLAIN = "text/plain";
const char HTTP_CONTENT_TYPE_HTML = "text/html";
const char HTTP_CONTENT_TYPE_CSS = "text/css";
const char HTTP_CONTENT_TYPE_JS = "application/javascript";
const char HTTP_CONTENT_TYPE_PNG = "image/png";
const char HTTP_CONTENT_TYPE_JPEG = "image/jpeg";
const char HTTP_CONTENT_TYPE_GIF = "image/gif";


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


const char* HTTPGetContentType(const char* filename);
void HTTPSendFile(HTTPRequestParseState *pS, int code, FSfile_typedef file);
void HTTPSendStr(HTTPRequestParseState* pS, int code, const char* content);
void HTTPHandle(CH395_TypeDef* pch395);
void HTTPRegisterResponder(const char* uri);
#endif /* HTTPSERVER_H_ */
