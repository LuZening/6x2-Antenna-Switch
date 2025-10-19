/*
 * my_websocket.c
 *
 *  Created on: Jul 12, 2025
 *      Author: cpholzn
 */
#include <stdint.h>
#include <string.h>
#include "my_websocket.h"
#include "LiteSHA1.h"
#define WS_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11" // RFC 6455 magic string
#define MIN(x,y) (((x) > (y)) ? (y) : (x))
/***************************************************************************
 *                 BASE64                                            *
 ***********************************************************************/
size_t base64_expect_output_length(size_t len)
{
	return 4*((len + 2) / 3 + 1);
}

/**
 * @brief Encode binary data to Base64 string
 * @param input: Pointer to binary data buffer
 * @param len: Length of input data in bytes
 * @param output: Pre-allocated buffer for Base64 result (min size: 4*((len+2)/3)+1)
 * @param retval: >=0 : length, <0 : error
 * @note Uses static lookup table for efficiency. Handles padding dynamically.
 */
int base64_encode(uint8_t *input, size_t len, char *output, size_t capacity) {
    uint32_t i = 0, j = 0;
    const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // RFC 4648 alphabet
    size_t expected_len = base64_expect_output_length(len);
    // return -1 if not enough space
    if(expected_len >= capacity)
    	return -1;
    for (i = 0; i < len; i += 3) {
        // Combine 3 bytes into a 24-bit triplet
        uint32_t triplet = (input[i] << 16) |
                          (i+1 < len ? input[i+1] << 8 : 0) |
                          (i+2 < len ? input[i+2] : 0);

        // Split triplet into four 6-bit indices
        output[j++] = base64_table[(triplet >> 18) & 0x3F];
        output[j++] = base64_table[(triplet >> 12) & 0x3F];
        // Add padding if input length not divisible by 3
        output[j++] = i+1 < len ? base64_table[(triplet >> 6) & 0x3F] : '=';
        output[j++] = i+2 < len ? base64_table[triplet & 0x3F] : '=';
    }
    output[j] = '\0'; // Null-terminate string
    return j;
}



/*********************************************************************************
 * ********  Websocket protocol 							*************************************
 * **************************************************************************/



/**
 * @brief Generate WebSocket handshake response key
 * @param client_key: Client's Sec-WebSocket-Key header value
 * @param output: Buffer for Base64-encoded response key
 * @note Computes SHA-1(client_key + GUID) then Base64 encodes
 */
int ws_handshake_response(char *client_key, size_t lenin, char *output, size_t lenout)
{
	// WARNING: no reentrant
    static char combined[128];
    static uint8_t sha1_digest[20];
    SHA1_CTX ctx;

    // Concatenate client key with GUID
    strncpy(combined, client_key, lenin);
    strncpy(combined + lenin, WS_GUID, 128 - lenin);

    sha1_init(&ctx);

    // Compute hash and encode
    sha1_encode(&ctx, (uint8_t*)combined, strlen(combined), sha1_digest);
    int r = base64_encode(sha1_digest, 20, output, lenout);
    return r;
}





/**
 * @brief Parse WebSocket frame header
 * @param data: Input buffer starting with frame header
 * @param frame: Output parsed frame metadata
 * @return Header length in bytes, -1 if error
 * @note Supports payload lengths ≤64KB (no 64-bit length)
 */
int ws_parse_frame(uint8_t *data, size_t len, WS_Frame *frame) {
    // Byte 0: FIN[7] + Opcode[3:0]
    frame->fin = (data[0] >> 7) & 0x01;
    frame->opcode = data[0] & 0x0F;
    // Byte 1: MASK[7] + Payload length[6:0]
    frame->mask = (data[1] >> 7) & 0x01;
    frame->payload_len = data[1] & 0x7F;
    uint8_t *ptr = data + 2;
    // payload_len==126, parse extended length from next 2 bytes
    if(frame->payload_len == 126)
    {
    	frame->payload_len = (((uint16_t)(ptr[0])) << 8) | (ptr[1]);
    	ptr += 2;
    }
    // NOT IMPLEMENTED : payload_len==127, parse extended length from next 4 bytes
    else if(frame->payload_len == 127)
    {
    	// too long for this device
//    	frame->payload_len = 0;
//    	ptr += 8;
    	return -1;
    }

    // Read masking key if present
    if (frame->mask) {
        memcpy(frame->masking_key, ptr, 4);
        ptr += 4;
    }
    // record a pointer to the payload's head
    frame->payload = ptr;
    return ptr - data; // Header length
}

/**
 * @brief Unmask WebSocket payload data
 * @param payload: Data buffer to unmask (modified in-place)
 * @param len: Payload length
 * @param masking_key: 4-byte XOR key
 * @note RFC 6455 requires masking for client→server frames
 */
void ws_unmask_payload(uint8_t *payload, size_t len, uint8_t *masking_key)
{
    for (int i = 0; i < len; ++i)
        payload[i] ^= masking_key[i % 4]; // Cyclic XOR with key
}


int ws_make_pong_frame(uint8_t* buf)
{
	buf[0] = 0x80U | WS_OPCODE_PONG;
	buf[1] = 0;
	return 2;
}

int ws_make_close_frame(uint8_t* buf, ws_close_reason_t reason)
{
	buf[0] = 0x80U | WS_OPCODE_CLOSE;
	buf[1] = 2;
	// reasons:
	// 0x03eb = 1000, normal close
	// 0x03ee = 1006, closed by peer
	buf[2] = 0x03;
	switch(reason)
	{
	case WS_CLOSE_REASON_NORMAL:
		buf[3] = 0xeb;
		break;
	case WS_CLOSE_REASON_BY_PEER:
		buf[3] = 0xee;
		break;
	default:
		buf[3] = 0xeb;
	}
	return 4;
}

int ws_make_text_frame(uint8_t* buf, size_t lenbuf, const uint8_t* in, size_t lenin)
{
	/* the content cannot be longer than 65535 bytes */
	// not enough space in buffer
	if(lenbuf < lenin + 5)
		return -1;

	uint8_t* p = buf;

	// OPCODE, FIN=1
	*(p++) = 0x80U | WS_OPCODE_TEXT;

	// payload length
	if(lenin <= 125)
		*(p++) = lenin;
	else
	{
		p[0] = 126U;
		// 2bytes for length, but big-endian
		p[1] = (lenin >> 8U) & 0xffU; // High
		p[2] = lenin & 0xffU; // Low
		p += 3;
	}

	// payload data
	memcpy(p, in, lenin);
	// terminate
	*(p+lenin) = 0;
	return p - buf + lenin;
}
