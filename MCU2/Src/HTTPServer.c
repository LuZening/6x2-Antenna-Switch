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
#include "my_websocket.h"


//#define __ON_BOARD_

char response_header_shared_buffer[MAX_LEN_RESPONSE_HEADER];


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

HTTPRequestParseState parseStates[NUM_SOCKETS - 1] =
{
	{.state = 0,
	.argc = 0,
	.connection = CLOSED,
	.method = HTTP_GET,
	.ready = false}
};

void resetHTTPParseState(HTTPRequestParseState *pS)
{
	pS->state = 0;
	pS->argc = 0;
	pS->connection = CLOSED;
	pS->method = HTTP_GET;
	pS->ready = false;
	pS->response_stage = RESPONSE_NOT_PREPARED;
	pS->len_response_content_remain = 0;
	pS->response_header = response_header_shared_buffer; // use shared buffer
}

void resetHTTPParseState_for_long_connection(HTTPRequestParseState *pS)
{
	pS->state = 0;
	pS->argc = 0;
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
	char* buf = pS->response_header;
	BOOL is_gzip = false;
	/* preprocessing */
	if(ENDSWITH(file.path, ".gz"))
	{
		is_gzip = true;
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
	char* buf = pS->response_header;
	*buf = 0; // clear buffer
	strcat(buf, HTTP_STR_VERSION);
	// Line 1
	// HTTP/1.1
	buf += strlen(HTTP_STR_VERSION);
	*buf = ' '; // HTTP/1.1_
	buf++;
	// CODE
	buf += u16toa(code, buf);
	switch(code)
	{
	case 200:
		strcat(buf, " OK\r\n");
		buf += 5;	// 200 OK\r\n;
		break;
	case 400:
		strcat(buf, " Error\r\n");
		buf += 8; // 400 Error\r\n
		break;
	}

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
	pS->response_content = (uint8_t*)content;
	pS->response_stage = RESPONSE_PREPARED;
}


int HTTPSendWebSocketHandshakeResponse(HTTPRequestParseState* pS, char* client_key, size_t lenKey)
{
	// construct header
//	unsigned char* buf = ch395.buffer;
	char* buf = pS->response_header;
	*buf = 0; // clear buffer
	strcpy(buf, HTTP_STR_VERSION);
	// Line 1
	// HTTP/1.1
	buf += strlen(HTTP_STR_VERSION);
	*buf = ' '; // HTTP/1.1_
	buf++;
	// CODE 101 Switching Protocols
	buf += u16toa(101, buf);
	strcpy(buf, " Switching Protocols\r\n"); // NOTE: leading space is necessary
	buf += 22;	// _Switching Protocols\r\n
	// Line 2: Upgrade: websocket
	strcpy(buf, "Upgrade: websocket\r\n");
	buf += 20;
	// Line 3: Connection: Upgrade
	strcpy(buf, "Connection: Upgrade\r\n");
	buf += 21;
	// Line 4: Sec-WebSocket-Accept: ?????????
	strcpy(buf, "Sec-WebSocket-Accept: ");
	buf += 22;
	// generate WS key
	int lenOut = ws_handshake_response(client_key, lenKey, buf, 48);
	buf += lenOut;
	if(lenOut > 0) // key generation OK
	{
		strcpy(buf, "\r\n");
		buf += 2;
		*buf = 0; // terminate
		// Start Sending
		pS->len_response_header = buf - pS->response_header;
		pS->len_response_content_remain = 0;
		pS->response_content = NULL;
		pS->response_stage = RESPONSE_PREPARED;
		return 0; // success
	}
	else // key generation failed
	{
		// Start Sending
		HTTPSendStr(pS, 400, "Key Failed");
		return -1; // failed
	}
}

void HTTPredirect(HTTPRequestParseState *pS, const char* URI)
{
	char* buf = pS->response_header;
	*buf = 0; // clear buffer
	// HTTP/1.1 302 Found
	// Location: /
	strcat(buf, HTTP_STR_VERSION);
	buf += strlen(HTTP_STR_VERSION);
	strcat(buf, " 302");
	buf += 4;
	strcat(buf, " Found");
	buf += 6;
	strcat(buf,HTTP_LINE_DELIM);
	buf +=2 ;
	strcat(buf, "Connection: ");
	buf += 12;
	strcat(buf, "Closed");
	buf += 6;
	strcat(buf, HTTP_LINE_DELIM);
	buf += 2;
	strcat(buf, "Content-Length: ");
	buf += 16;
	strcat(buf, "0");
	buf += 1;
	strcat(buf, HTTP_LINE_DELIM);
	buf += 2;
	strcat(buf, "Location: ");
	buf+= 10;
	strcat(buf, URI);
	strcat(buf, HTTP_LINE_DELIM);
	strcat(buf, HTTP_LINE_DELIM);
	// Start Sending
	pS->len_response_header = strlen(pS->response_header);
	pS->len_response_content_remain = 0;
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

	int8_t i = pch395->SOCK_responding; // NOTE: number:i begins from 1
	uint8_t j;
	if(i > 0)
	{
		HTTPRequestParseState *pS = &parseStates[i-1];
		// STAGE 0: Socket recv has a request, but no response under preparation, can prepare a new response
		if(pS->ready  && pS->response_stage == RESPONSE_NOT_PREPARED)
		{
			// CASE 0: respond to normal HTTP request
			if(pS->connection != UPGRADED_WS)
			{
				for(j=0; j < NUM_HTTP_RESPONDERS; ++j)
				{
					if(strncmp(pS->URI, HTTPResponders[j].uri, MAX_LEN_URI) == 0) // matches
					{
						(HTTPResponders[j].func)(pS); // call HTTPResponder service function
						break;
					}
				}
				if(j >= NUM_HTTP_RESPONDERS) // Resource not found
				{
					HTTPonNotFound(pS);
				}
			}
			// CASE 1: respond to Websocket upgrade request
			else if(pS->connection == UPGRADED_WS)
			{
				// client key is stored in cookies buffer
				int r = HTTPSendWebSocketHandshakeResponse(pS, pS->cookies, strlen(pS->cookies));
			}

		}

		// STAGE 1 : CH395 chip is free for transmission,
		// judge if response has been prepared for transmission
		// if the CHIP is free, set off the transmission
		// otherwise next time
		if((pch395->TX_available & (1 << i)) != 0 && pS->ready)
		{
			// CASE 1: parser has prepared the response content,
			// start the process of data transmission immediately
			if(pS->response_stage == RESPONSE_PREPARED)
			{
				uint16_t max_len_content = MAX_SIZE_PACK - pS->len_response_header;
				uint16_t len_content_this_time = ((pS->len_response_content_remain < max_len_content)
						? (pS->len_response_content_remain)
								: (max_len_content));
				CH395StartSendingData(i, pS->len_response_header + len_content_this_time);
				CH395ContinueSendingData((uint8_t*)pS->response_header, pS->len_response_header);
				CH395ContinueSendingData((const uint8_t*)pS->response_content, len_content_this_time);
				CH395Complete();
				ch395.TX_available &= ~(1<<i);
				pS->len_response_content_remain -= len_content_this_time;
				pS->response_content += len_content_this_time;
				if(pS->len_response_content_remain == 0) // all content completely sent this time
				{
					if(pS->connection == CLOSED)
					{
						HTTPclose(i);
						resetHTTPParseState(pS);
					}
					else
					{
						HTTPclose_for_long_connection(i);
						resetHTTPParseState_for_long_connection(pS);
					}
					// enqueue next sock to respond
					ch395.SOCK_responding = getNextSock();
				}
				else // content remained to be sent next time
				{
					pS->response_stage = RESPONSE_CONTENT_REMAIN;
				}
			}
			// CASE2: previous response has remaining parts to be done, continue the sending process
			else if(pS->response_stage == RESPONSE_CONTENT_REMAIN)
			{
				uint16_t len = pS->len_response_content_remain;
				CH395SendData(i, pS->response_content, ((len<MAX_SIZE_PACK)?(len):(len=MAX_SIZE_PACK)));
				ch395.TX_available &= ~(1<<i);
				// move the cursor to record how much content has been transmitted
				pS->len_response_content_remain -= len;
				pS->response_content += len; // move the cursor
				// judge if transmission has finished
				if(pS->len_response_content_remain == 0)
				{
					if(pS->connection == CLOSED)
					{
						HTTPclose(i);
						resetHTTPParseState(pS);
					}
					else
					{
						HTTPclose_for_long_connection(i);
						resetHTTPParseState_for_long_connection(pS);
					}
					// enqueue next sock to respond
					ch395.SOCK_responding = getNextSock();
				}
			}
		}

		// Iterate the next socket
		if(pS->sock_index == i && !pS->ready)
		{
			ch395.SOCK_responding = getNextSock();
//			CH395TCPDisconnect(i);
		}
	}
}
#endif


int8_t getNextSock()
{
	int8_t j;
	for(j=1; j < NUM_SOCKETS; ++j)
	{
		if(parseStates[j-1].ready && (ch395.socket_connected & (1 << j)))
		{
			return j;
		}
	}
	return -1;
}

void HTTPclose(uint8_t i) // Sock Index to disconnect
{
//	CH395TCPDisconnect(i);
	ch395.RX_received &= ~(1 << i);
	ch395.socket_connected &= ~(1 << i);
	ch395.SOCK_responding = -1;
	pS->connection = CLOSED;
}

void HTTPclose_for_long_connection(uint8_t i)
{
	ch395.RX_received &= ~(1 << i);
	ch395.SOCK_responding = -1;
}

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
	// start: parse request line
	// example:
	// GET /socket HTTP/1.1
	case 0:
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
			/* no break */
			// do not break, let the following execute
		}
		else
			goto HTTP_PARSE_ERROR;
			/* no break */
			// do not break, let the following execute
	// parse header
	// example:
	//		Host: example.com
	//		Upgrade: websocket
	//		Connection: Upgrade
	//		Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==
	//		Sec-WebSocket-Version: 13
	case 1:
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
					// CASE 1: keep alive connection
					if(strstr(tok, "Keep") != NULL)
						pS->connection = KEEP_ALIVE;
					// CASE 2: upgrade to WebSocket
					else if(strstr(tok, "Upgrade") != NULL)
					{
						pS->connection = UPGRADED_WS;
					}

				}
				// Header: Sec-WebSocket-Key
				else if(strcmp(tok, "Sec-WebSocket-Key") == 0)
				{
					tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
					if(tok == NULL) goto HTTP_PARSE_ERROR;
					// temporarily store client key in coolies
					char* pEnd = strncpy(pS->cookies, tok, MAX_LEN_COOKIES);
					*pEnd = 0;
				}
				// Header: Cookie
				else if(strcmp(tok, HTTP_ITEM_STR_COOKIES) == 0)
				{
					if(pS->connection != UPGRADED_WS)
					{
						tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
						if(tok == NULL) goto HTTP_PARSE_ERROR;
						char* pEnd = strncpy(pS->cookies, tok, MAX_LEN_COOKIES);
						*pEnd = 0;
					}
				}

			}
			line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		}
		pS->state++;
		/* no break */
		// do not break, let the following execute
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
		/* no break */
		// do not break, let the following execute
	case 4: // finished
		pS->ready = true;
		/* no break */
		// do not break, let the following execute
	}
	return true;
	HTTP_PARSE_ERROR:
		pS->state = 0;
		pS->ready = false;
		return false;
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
		if(*s >= '0' && *s <= '9')
		{
			sum *= 10;
			sum += *s - '0';
			s++;
		}
		else
			return 255; // error
	}
	return sum;
}
uint16_t atou16(const char* s)
{
	uint8_t i;
	uint8_t sum = 0;
	for(i=0; i<5 && *s; ++i) // at most 5 digits
	{
		if(*s >= '0' && *s <= '9')
		{
			sum *= 10;
			sum += *s - '0';
			s++;
		}
		else
			return 65535; // error
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

uint8_t IPv4_to_s(char* __restrict s ,const  uint8_t* __restrict IP)
{
  	uint8_t n = 0;
	uint8_t idx = 0;
  for(uint8_t i = 0; i < 4; ++i)
  {
	uint8_t d = IP[i];
	uint8_t nd = 0;
	if(d > 0)
	{
		while(d)
		{
			s[idx++] = (d % 10) + '0';
			d /= 10;
			++nd;
		}
	}
	else
	{
		s[idx++] = '0';
	}
	// reverse
	for(uint8_t j = 0; j < nd / 2; ++j)
	{
		char c;
		c = s[idx - 1 - j];
		s[idx - 1 - j] = s[idx - nd + j];
		s[idx - nd + j] = c;
	}
	// add dot
	if(i < 3)
	{
		s[idx++] = '.';
	}
	else
	{
		s[idx] = 0;
	}
  }
	return idx;
}
