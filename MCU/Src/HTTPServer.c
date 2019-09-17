/*
 * HTTPServer.c
 *
 *  Created on: Sep 13, 2019
 *      Author: Zening
 */

#include "CH395.H"
#include "HTTPServer.h"
#include "string.h"


typedef struct
{
	// 0: Start: Parsing method
	// 1: Parsing URI
	// 2: Parsing HTTP version
	// 3: Parsing headers
	// 4: Parsing header values
	UINT8 state = 0;
	UINT8 parse_Connection_cursor = 0;
	UINT8 parse_cursor = 0;
	Method method = HTTP_GET;
	HTTPConnection_typedef connection = CLOSED;
	char URI[MAX_LEN_URI];
	UINT8 argc=0;
	char* argv[MAX_NUM_ARGS];
	char cookies[MAX_LEN_COOKIES];
	BOOL ready = FALSE;
	UINT8 sock_index; // TODO: initialize sock_index
} HTTPRequestParseState;

HTTPServerRequest_typedef _request;

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

void HTTPSendFile(HTTPRequestParseState *pS, int code, FSfile_typedef file)
{
	char s_tmp[32];
	char* content_type;
	char* buf = CH395.buffer;
	BOOL is_gzip = FALSE;
	/* preprocessing */
	strncpy(path_confirmed, path);
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
	// TODO: send header to CH395 TX buffer;
	int len_buffer = strlen(ch395.buffer);
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
	CH395StartSendingData(pS->sock_index,
			len_header + len_content);
	CH395ContinueSendingData((UINT8*)ch395.buffer, len_header);
	CH395ContinueSendingData((UINT8*)content, len_content);
	CH395Complete();
}

char* strsepstr(char** stringp, const char* delim)
{
	char* loc_delim = strstr(*stringp, delim);
	char* loc_head = *stringp;
	int len_delim = strlen(delim);
	if(loc_head != NULL)
	{
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

void parse_http(HTTPRequestParseState *pS, char* buffer)
{
	UINT8 error = 0;
	char* line, tok, line_tok_saveptr, word_tok_saveptr;
	switch(pS->state)
	{
	case 0: // start: request line
		line_tok_saveptr = buffer;
		line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		if(line)
		{
			// 1: method
			tok = strtok_r(line, HTTP_DELIM, &word_tok_saveptr);
			if(tok)
			{
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
				strncpy(pS->URI ,tok, MAX_LEN_URI);
			else
				goto HTTP_PARSE_ERROR;
			// TODO: 2.1 parse args
			char* s_args = strchr(pS->URI, '?');
			if(pS->method == HTTP_GET && s_args != NULL)
			{
				*s_args = 0; // terminate the URI at ? symbol, sperate it into two parts
				s_args++; // move to the char next to ?
				if(*s_args)
				{
					char* tok_arg = strtok_r(s_args, "&", &arg_tok_saveptr);
					while(tok_arg)
					{
						pS->argv[pS->argc++] = tok_arg;
						tok_arg = strtok_r(NULL, "&", &arg_tok_saveptr);
					}
				}
			}
			// 3: version
			strtok_r(NULL, HTTP_DELIM, &word_tok_saveptr);
			if(tok == NULL) goto HTTP_PARSE_ERROR;
			pS->state ++;
		}
		else
			goto HTTP_PARSE_ERROR;
	case 1: // parse header
		line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		while(line)
		{
				tok = strtok_r(line, HTTP_COLUMN_DELIM, &word_tok_saveptr);
				if(tok == NULL || *tok = 0)
					break;
				if(tok)
				{
					// Header: Connection
					if(strcmp(tok, HTTP_ITEM_STR_CONNECTION) == 0)
					{
						tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
						if(tok == NULL) goto HTTP_PARSE_ERROR;
						if(strstr(tok, "Keep") != NULL)
							pS->connection = KEEP_ALIVE;
					}
					// Header: Cookie
					else if(strcmp(tok, HTTP_ITEM_STR_COOKIES) == 0)
					{
						tok = strtok_r(NULL, HTTP_COLUMN_DELIM, &word_tok_saveptr);
						if(tok == NULL) goto HTTP_PARSE_ERROR;
						strcpy(pS->cookies, tok, MAX_LEN_COOKIES);
					}
				}
			line = strsepstr(&line_tok_saveptr, HTTP_LINE_DELIM);
		}
		pS->state++;
	case 2: // finished
		pS->ready = TRUE;
	}
	return;
	HTTP_PARSE_ERROR:
		pS->state = 0;
		pS->ready = FALSE;
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
				// if buffer is not empty
				HTTPRequestParseState state;
				while(len = CH395GetRecvLength(i))
				{
					if(len > 0)
					{
						CH395GetRecvData(i, (len < CH395_SIZE_BUFFER)?(len):(CH395_SIZE_BUFFER-1), pch395->buffer);
						pch395->buffer[(len < CH395_SIZE_BUFFER)?(len):(CH395_SIZE_BUFFER)]=0;
						// parse received text
						parse_http(&state, pch395->buffer);
					}
					if(state.ready)
					{
						// TODO: respond HTTP

					}
				}
			}
		}
	}
}
