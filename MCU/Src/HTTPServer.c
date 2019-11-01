/*
 * HTTPServer.c
 *
 *  Created on: Sep 13, 2019
 *      Author: Zening
 */
#define __ON_BOARD_
//#define __DEBUG_

#ifdef __ON_BOARD_
#include <CH395.h>
#endif
#include "main.h"
#include "HTTPServer.h"
#include "FS.h"
#include "Delay.h"
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

HTTPRequestParseState parseStates[NUM_SOCKETS] =
{
	{.state = 0,
	.argc = 0,
	.connection = CLOSED,
	.method = HTTP_GET,
	.ready = FALSE}
};

void resetHTTPParseState(HTTPRequestParseState *pS)
{
	pS->state = 0;
	pS->argc = 0;
	pS->connection = CLOSED;
	pS->method = HTTP_GET;
	pS->ready = FALSE;
	pS->response_stage = RESPONSE_NOT_PREPARED;
	pS->len_response_content_remain = 0;
	pS->response_header = response_header_shared_buffer; // use shared buffer
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
	const char* content_type;
//	unsigned char* buf = ch395.buffer;
	unsigned char* buf = pS->response_header;
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
	// Line 1
	// HTTP/1.1
	strcat(buf, HTTP_STR_VERSION);
	buf += strlen(HTTP_STR_VERSION); // HTTP/1.1
	*buf = ' '; // HTTP/1.1_
	buf++;
	// CODE
	buf += u16toa(code, buf);
	strcat(buf, " OK\r\n");
	buf += 5;	// 200 OK\r\n
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
	buf += u16toa((uint16_t)file.size, buf); // Content-Length: file.size
//	sprintf(buf, "%d\r\n", file.size);
	strcat(buf, HTTP_LINE_DELIM);
	buf += 2;
	// Line 6: end of header
	strcat(buf, HTTP_LINE_DELIM); //\r\n\r\n
	buf += 2;
	pS->len_response_header = strlen(pS->response_header);
	pS->len_response_content_remain = (uint16_t)file.size;
	pS->response_content = file.p_content;
	pS->response_stage = RESPONSE_PREPARED;

}

void HTTPSendStr(HTTPRequestParseState* pS, int code, const char* content)
{
	// construct header
//	unsigned char* buf = ch395.buffer;
	unsigned char* buf = pS->response_header;
	*buf = 0; // clear buffer
	strcat(buf, HTTP_STR_VERSION);
	// Line 1
	// HTTP/1.1
	buf += strlen(HTTP_STR_VERSION);
	*buf = ' '; // HTTP/1.1_
	buf++;
	// CODE
	buf += u16toa(code, buf);
	strcat(buf, " OK\r\n");
	buf += 5;	// 200 OK\r\n
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
	const char* content_type = HTTPGetContentType(pS->URI);
	strcat(buf, "Content-Type: ");
	buf += 14;
	strcat(buf, content_type);
	buf += strlen(content_type);
	strcat(buf, "\r\n");
	buf += 2;
	// Line 4: Content-Length
	strcat(buf, "Content-Length: ");
	buf += 16;
	uint16_t len_content = strlen(content);
	buf += u16toa(len_content, buf); // Content-Length: file.size
//	sprintf(buf, "%d\r\n", file.size);
	strcat(buf, HTTP_LINE_DELIM);
	buf += 2;
	// Line 5: End of header
	strcat(buf, HTTP_LINE_DELIM);
	buf += 2;
	// Start Sending
	pS->len_response_header = strlen(pS->response_header);
	pS->len_response_content_remain = strlen(content);
	pS->response_content = content;
	pS->response_stage = RESPONSE_PREPARED;
}

void HTTPonNotFound(HTTPRequestParseState *pS)
{
	char s_tmp[MAX_LEN_URI + 4];
	static char s_notfound[MAX_LEN_URI+16];
	// look for file
	if(FS_exists(&FS, pS->URI))
	{
		FSfile_typedef file = FS_open(&FS, pS->URI);
		if(file.path)
		{
			HTTPSendFile(pS, 200, FS_open(&FS, pS->URI));
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
	strcpy(s_notfound, "Not Found: ");
	strncat(s_notfound, pS->URI, MAX_LEN_URI);
	strcat(s_notfound, HTTP_LINE_DELIM);
	HTTPSendStr(pS, 404, s_notfound);
}

void HTTPHandle(CH395_TypeDef *pch395) // call on interrupt
{
	uint8_t i = pch395->SOCK_responding;
	if(i)
	{
		HTTPRequestParseState *pS= parseStates +i  - 1;
		if((pch395->TX_available & (1 << i)) && pS->ready) // socket recv buffer non-empty, bit_i is 1
		{
			uint8_t j;
			if(TRUE)
			{
				for(j=0; j<NUM_HTTP_RESPONDERS; ++j)
				{
					if(strncmp(pS->URI, HTTPResponders[j].uri, MAX_LEN_URI) == 0) // matches
					{
						(HTTPResponders[j].func)(pS); // call HTTPResponder service function
						break;
					}
				}
				if(j == NUM_HTTP_RESPONDERS) // Resource not found
				{
					HTTPonNotFound(pS);
				}
				if(pS->response_stage == RESPONSE_PREPARED) // start the process of data sending
				{
					uint16_t max_len_content = MAX_SIZE_PACK - pS->len_response_header;
					uint16_t len_content_this_time = ((pS->len_response_content_remain < max_len_content)
							? (pS->len_response_content_remain)
									: (max_len_content));
					CH395StartSendingData(i, pS->len_response_header + len_content_this_time);
					CH395ContinueSendingData(pS->response_header, pS->len_response_header);
					CH395ContinueSendingData(pS->response_content, len_content_this_time);
					CH395Complete();
					pS->len_response_content_remain -= len_content_this_time;
					pS->response_content += len_content_this_time;
					if(pS->len_response_content_remain == 0) // all content completely sent this time
					{
						resetHTTPParseState(pS);
						// enque next sock to respond
						for(j=1; j <= NUM_SOCKETS; ++j)
						{
							if(parseStates[j-1].ready && (pch395->socket_connected & (1 << j)))
							{
								pch395->SOCK_responding = j;
								break;
							}
						}
					}
					else // content remained to be sent next time
					{
						pS->response_stage = RESPONSE_CONTENT_REMAIN;
					}
				}
				else if(pS->response_stage == RESPONSE_CONTENT_REMAIN) // continue the sending process
				{
					uint16_t len = pS->len_response_content_remain;
					CH395SendData(i, pS->response_content, ((len<MAX_SIZE_PACK)?(len):(len=MAX_SIZE_PACK)));
					// sent
					pS->len_response_content_remain -= len;
					pS->response_content += len; // move the cursor
					if(pS->len_response_content_remain == 0) // finished
					{
						resetHTTPParseState(pS);
						// enque next sock to respond
						for(j=1; j <= NUM_SOCKETS; ++j)
						{
							if(parseStates[j-1].ready && (pch395->socket_connected & (1 << j)))
							{
								pch395->SOCK_responding = j;
								break;
							}
						}
					}
					// TODO: multiple packages
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



const char* getHTTPArg(HTTPRequestParseState *pS, const char* name)
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
uint8_t atou8(const char* s)
{
	uint8_t i;
	uint8_t sum = 0;
	for(i=0; i<3 && *s; ++i) // at most 3 digits
	{
		sum *= 10;
		sum += *s - '0';
		s++;
	}
	return sum;
}
uint8_t u16toa(uint16_t d, char* buf) // return: digits
{
	uint8_t i = 0, j;
	while(d)
	{
		buf[i++] = (d % 10) + '0';
		d /= 10;
	}
	buf[i] = 0;
	for(j = 0; j < i/2; ++j)
	{
		char tmp;
		tmp = buf[j];
		buf[j] = buf[i-j-1];
		buf[i-j-1] = tmp; // swap
	}
	return i;
}

// Fast strcpy
// Return: pointer to the end of dest + src
char* strcpy_f(char* dest, const char* src)
{
	while(*src)
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dest;
}

char* strncpy_f(char* dest, const char* src, uint16_t len)
{
	while(*src && len)
	{
		*dest = *src;
		src++;
		dest++;
		len--;
	}
	*dest = 0;
	return dest;
}
