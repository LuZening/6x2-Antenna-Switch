/*
 * HTTPServer.c
 *
 *  Created on: Sep 13, 2019
 *      Author: Zening
 */
#define __ON_BOARD_
//#define __DEBUG_

#ifdef __ON_BOARD_
#include "CH395.H"
#endif
#include "main.h"
#include "HTTPServer.h"
#include "FS.h"
#include <string.h>


//#define __ON_BOARD_




/*	HTTP String Constants	*/
const char HTTP_ITEM_STR_CONNECTION[] = "Connection";
const char HTTP_ITEM_STR_COOKIES[] = "Cookie";
const char HTTP_ITEM_STR_POST[] = "POST";
const char HTTP_ITEM_STR_GET[] = "GET";
const char HTTP_LINE_DELIM[] = "\r\n";
const char HTTP_DELIM[] = " ";
const char HTTP_COLUMN_DELIM[] = ": ";
const char HTTP_STR_VERSION[] = "HTTP/1.1";
const char HTTP_CONTENT_TYPE_PLAIN[] = "text/plain";
const char HTTP_CONTENT_TYPE_HTML[] = "text/html";
const char HTTP_CONTENT_TYPE_CSS[] = "text/css";
const char HTTP_CONTENT_TYPE_JS[] = "application/javascript";
const char HTTP_CONTENT_TYPE_PNG[] = "image/png";
const char HTTP_CONTENT_TYPE_JPEG[] = "image/jpeg";
const char HTTP_CONTENT_TYPE_GIF[] = "image/gif";

HTTPRequestParseState parseState = 
{
	.state = 0,
	.argc = 0,
	.connection = CLOSED,
	.method = HTTP_GET,
	.ready = FALSE,
};

void resetHTTPParseState(HTTPRequestParseState *pS)
{
	pS->state = 0;
	pS->argc = 0;
	pS->connection = CLOSED;
	pS->method = HTTP_GET;
	pS->ready = FALSE;
}

const char* HTTPGetContentType(const char* filename)
{
	if(strlen(filename) > 3)
	{
		if(ENDSWITH(filename, ".html"))
			return HTTP_CONTENT_TYPE_HTML;
		else if(ENDSWITH(filename, ".htm"))
			return HTTP_CONTENT_TYPE_HTML;
		else if(ENDSWITH(filename, ".css"))
			return HTTP_CONTENT_TYPE_CSS;
		else if(ENDSWITH(filename, ".js"))
			return HTTP_CONTENT_TYPE_JS;
		else if(ENDSWITH(filename, ".png"))
			return HTTP_CONTENT_TYPE_PNG;
		else if(ENDSWITH(filename, ".jpg"))
			return HTTP_CONTENT_TYPE_JPEG;
		else if(ENDSWITH(filename, ".jpeg"))
			return HTTP_CONTENT_TYPE_JPEG;
		else if(ENDSWITH(filename, ".gif"))
			return HTTP_CONTENT_TYPE_GIF;
	}
	return HTTP_CONTENT_TYPE_PLAIN;
}

#ifdef __ON_BOARD_
void HTTPSendFile(HTTPRequestParseState *pS, int code, FSfile_typedef file)
{
	char s_tmp[32];
	char* content_type;
	char* buf = CH395.buffer;
	BOOL is_gzip = FALSE;
	/* preprocessing */
	if(ENDSWITH(file.path, ".gz"))
	{
		is_gzip = TRUE;
		strcpy(s_tmp, file.path);
		s_tmp[strlen(s_tmp) - 3] = 0;
		content_type = HTTPGetContentType(s_tmp);
	}
	else
	{
		content_type = HTTPGetContentType(file.path);
	}
	// construct header
	*buf = 0; // clear buffer
	strcat(buf, HTTP_STR_VERSION);
	// Line 1
	// HTTP/1.1
	buf += strlen(HTTP_STR_VERSION);
	// CODE
	sprintf(buf, " %3d OK\r\n", code);
	buf += 8;
	// Line 2: Connection
	strcat(buf, "Connection: ");
	buf += 12;
	if(pS->connection == KEEP_ALIVE)
	{
		strcat(buf, "Keep-Alive\r\n");
		buf += 12;
	}
	else
	{
		strcat(buf, "Closed\r\n");
		buf += 8;
	}
	// Line 3: Content-Encoding
	if(is_gzip)
	{
		strcat(buf, "Content-Encoding: gzip\r\n");
		buf += 24;
	}
	// Line 4: Content-Type
	strcat(buf, "Content-Type: ");
	buf += 14;
	strcat(buf, content_type);
	buf += strlen(content_type);
	strcat(buf, "\r\n");
	buf += 2;
	// Line 5: Content-Length
	strcat(buf, "Content-Length: ");
	buf += 16;
	sprintf(buf, "%d\r\n", file.size);
	// Line 6: end of header
	strcat(buf, "\r\n");
	int len_buffer = strlen(ch395.buffer);
	while(ch395.TX_available & (1 << pS->sock_index))
	{
		DEBUG_LOG("Waiting for free TX buffer SOCK %d\n", pS->sock_index);
		Delay_ms(10);
	}
	CH395StartSendingData(pS->sock_index, len_buffer + file.size);
	// send header
	CH395ContinueSendingData(ch395.buffer, len_buffer);
	// send content
	CH395ContinueSendingData(file.p_content, file.size);
	CH395Complete();
}

void HTTPSendStr(HTTPRequestParseState* pS, int code, const char* content)
{
	// construct header
	char* buf = ch395.buffer;
	*buf = 0; // clear buffer
	strcat(buf, HTTP_STR_VERSION);
	// Line 1
	// HTTP/1.1
	buf += strlen(HTTP_STR_VERSION);
	// CODE
	sprintf(buf, " %3d OK\r\n", code);
	buf += 8;
	// Line 2: Connection
	strcat(buf, "Connection: ");
	buf += 12;
	if(pS->connection == KEEP_ALIVE)
	{
		strcat(buf, "Keep-Alive\r\n");
		buf += 12;
	}
	else
	{
		strcat(buf, "Closed\r\n");
		buf += 8;
	}
	// Line 3: Content-Type
	strcat(buf, "Content-Type: ");
	buf += 14;
	strcat(buf, content_type);
	buf += strlen(content_type);
	strcat(buf, "\r\n");
	buf += 2;
	// Line 4: Content-Length
	strcat(buf, "Content-Length: ");
	buf += 16;
	UINT16 len_content = strlen(content);
	sprintf(buf, "%d\r\n", len_content);
	buf += strlen(buf);
	// Line 5: End of header
	strcat(buf, "\r\n");
	buf += 2;
	// Start Sending
	UINT16 len_header = strlen(ch395.buffer);
	do
	{
		DEBUG_LOG("Waiting for free TX buffer SOCK %d\n", pS->sock_index);
		UINT8 sock_int = CH395GetSocketInt(pS->sock_index);
	}while(!(sock_int & SINT_STAT_SENBUF_FREE));
	CH395StartSendingData(pS->sock_index,
			len_header + len_content);
	CH395ContinueSendingData((UINT8*)ch395.buffer, len_header);
	CH395ContinueSendingData((UINT8*)content, len_content);
	CH395Complete();
}

void HTTPonNotFound(HTTPRequestParseState *pS)
{
	char s_tmp[MAX_LEN_URI + 3];
	// look for file
	if(FS_exists(&FS, pS->URI))
	{
		FSfile_typedef file = FS_open(&FS, pS->URI);
		if(file.path)
		{
			HTTPSendFile(pS, 200, FS_open(&FS, path));
			return;
		}
	}
	else
	{
		strcpy(s_tmp, pS->URI);
		strcat(s_tmp, ".gz");
		if(FS_exists(&FS, s_tmp))
		{
			FSfile_typedef file = FS_open(&FS, s_tmp);
			if(file.path)
			{
				HTTPSendFile(pS, 200, FS_open(&FS, s_tmp));
				return;
			}
		}
	}
	sprintf(ch395.buffer, "URI: %s\r\nNot found\r\n", pS->URI);
	HTTPSendStr(pS, 404, ch395.buffer);
}

void HTTPHandle(CH395_TypeDef *pch395) // call on interrupt
{
	UINT8 i;
	UINT8 len;
	if(pch395->RX_received != 0)
	{
		for(i=0; i < 4; ++i)
		{
			if((pch395->RX_received >> i) & 0x01) // socket is connected, bit_i is 1
			{
				pch395->RX_received &= ~(1 << i); // clear availibility symbol
				// if buffer is not empty
				parseState.sock_index = i;
				while(len = CH395GetRecvLength(i))
				{
					if(len > 0)
					{
						CH395GetRecvData(i, (len < CH395_SIZE_BUFFER)?(len):(CH395_SIZE_BUFFER-1), pch395->buffer);
						pch395->buffer[(len < CH395_SIZE_BUFFER)?(len):(CH395_SIZE_BUFFER)]=0;
						// parse received text
						if(!parse_http(&parseState, pch395->buffer))
						{
							CH395ClearRecvBuf(i);
						}
					}
					if(state.ready)
					{
						// TODO: respond HTTP
						UINT8 j;
						for(j=0; j<NUM_HTTP_RESPONDERS; ++j)
						{
							if(strncmp(parseState.URI, HTTPResponders[j].uri, MAX_LEN_URI) == 0) // matches
							{
								DEBUG_LOG("HTTP: Respond %s\n", HTTPResponders[j].uri);
								(HTTPResponders[j].func)(&parseState); // call HTTPResponder service function
								break;
							}
						}
						if(j == NUM_HTTP_RESPONDERS) // Resource not found
						{
							HTTPonNotFound(&parseState);
						}
						CH395ClearRecvBuf(i);
						ch395.RX_received &= ~(1 << i); // clear receive flag
					}
				}
			}
		}
	}
}
#endif

char* strsepstr(char** stringp, const char* delim)
{
	int len_delim = strlen(delim);
	char* loc_head = *stringp;
	if(loc_head != NULL)
	{
		char* loc_delim = strstr(loc_head, delim);
		if(loc_delim != NULL)
		{
			memset(loc_delim, 0, len_delim);
			*stringp = loc_delim + len_delim;
		}
		else
		{
			*stringp = NULL;
		}
	}
	return loc_head;
}

BOOL parse_http(HTTPRequestParseState *pS, char* buffer)
{
	UINT8 error = 0;
	char* line, *tok, *tok_arg, *line_tok_saveptr, *word_tok_saveptr, *arg_tok_saveptr;
	switch(pS->state)
	{
	case 0: // start: request line
		line_tok_saveptr = buffer;
		line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		if(line)
		{
			DEBUG_LOG("Line: %s\r\n", line);
			// check if received info obeys HTTP protocols
			if(strnstr(line, "HTTP/", CH395_SIZE_BUFFER) == NULL)
			{
				DEBUG_LOG("ERROR: NOT HTTP\n");
				goto HTTP_PARSE_ERROR;
			}
			// 1: method
			tok = strtok_r(line, HTTP_DELIM, &word_tok_saveptr);
			if(tok)
			{
				DEBUG_LOG("Method: %s\r\n", tok);
				if(strcmp(tok, HTTP_ITEM_STR_GET) == 0)
					pS->method = HTTP_GET;
				else if(strcmp(tok, HTTP_ITEM_STR_POST) == 0)
					pS->method = HTTP_POST;
				else
					pS->method = HTTP_GET;
			}
			else
				goto HTTP_PARSE_ERROR;
			// 2: URI
			tok = strtok_r(NULL, HTTP_DELIM, &word_tok_saveptr);
			if(tok)
			{
				strncpy(pS->URI ,tok, MAX_LEN_URI);
				DEBUG_LOG("URI: %s\r\n", pS->URI);
			}
			else
				goto HTTP_PARSE_ERROR;
			// TODO: 2.1 parse args
			char* s_args = strchr(pS->URI, '?');
			if(pS->method == HTTP_GET && s_args != NULL)
			{
				*s_args = 0; // terminate the URI at ? symbol, sperate it into two parts
				DEBUG_LOG("Path: %s\r\n", pS->URI);
				s_args++; // move to the char next to ?
				if(*s_args)
				{
					tok_arg = strtok_r(s_args, "&", &arg_tok_saveptr);
					while(tok_arg)
					{
						DEBUG_LOG("Arg: %s\r\n", tok_arg);
						pS->argv[pS->argc++] = tok_arg;
						tok_arg = strtok_r(NULL, "&", &arg_tok_saveptr);
					}
				}
			}
			// 3: version
			tok = strtok_r(NULL, HTTP_DELIM, &word_tok_saveptr);
			if(tok)
			{
				DEBUG_LOG("VERSION: %s\r\n", tok);
			}
			else
				goto HTTP_PARSE_ERROR;
			pS->state ++;
		}
		else
			goto HTTP_PARSE_ERROR;
	case 1: // parse header
		line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		while(line)
		{
			DEBUG_LOG("Line: %s\r\n", line);
			tok = strtok_r(line, HTTP_COLUMN_DELIM, &word_tok_saveptr);
			if(tok == NULL || *tok == 0)
				break;
			if(tok)
			{
				DEBUG_LOG("Header: %s\r\n", tok);
				// Header: Connection
				if(strcmp(tok, HTTP_ITEM_STR_CONNECTION) == 0)
				{
					tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
					if(tok == NULL) goto HTTP_PARSE_ERROR;
					DEBUG_LOG("Value: %s\r\n", tok);
					if(strstr(tok, "Keep") != NULL)
						pS->connection = KEEP_ALIVE;
				}
				// Header: Cookie
				else if(strcmp(tok, HTTP_ITEM_STR_COOKIES) == 0)
				{
					tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
					if(tok == NULL) goto HTTP_PARSE_ERROR;
					strncpy(pS->cookies, tok, MAX_LEN_COOKIES);
				}
			}
			line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		}
		pS->state++;
	case 3: // parse payload
		if(pS->method == HTTP_POST)
		{
			line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
			if(line)
			{
				tok_arg = strtok_r(line, "&", &arg_tok_saveptr);
				while(tok_arg)
				{
					DEBUG_LOG("Arg: %s\r\n", tok_arg);
					pS->argv[pS->argc++] = tok_arg;
					tok_arg = strtok_r(NULL, "&", &arg_tok_saveptr);
				}
			}
		}
		pS->state++;
	case 4: // finished
		pS->ready = TRUE;
	}
	return TRUE;
	HTTP_PARSE_ERROR:
		pS->state = 0;
		pS->ready = FALSE;
		return FALSE;
}



char* getHTTPArg(HTTPRequestParseState *pS, const char* name)
{
	if(pS->argc == 0)
		return NULL;
	int i;
	const char* cur_s; 
	const char* cur_name = name;
	for(i=0; i<pS->argc; ++i)
	{
		// compare until '='
		cur_s = pS->argv[i];
		while(*cur_s && *cur_name)
		{
			if(*cur_s != *cur_name)
			{
				break;
			}
			cur_name++;
			cur_s++;
		}
		if(*cur_name == 0 && *cur_s == '=') // matched
		{
			return cur_s+1;
		}
		cur_name = name;
	}
	return NULL;
}
