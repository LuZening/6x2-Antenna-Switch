#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/***************************************************************************
 *                 BASE64                                            *
 ***********************************************************************/
/**
 * @brief Encode binary data to Base64 string
 * @param input: Pointer to binary data buffer
 * @param len: Length of input data in bytes
 * @param output: Pre-allocated buffer for Base64 result (min size: 4*((len+2)/3)+1)
 * @note Uses static lookup table for efficiency. Handles padding dynamically.
 */
int base64_encode(uint8_t *input, size_t len, char *output, size_t capacity);




/*********************************************************************************
 * ********  Websocket protocol 		*************************************
 * **************************************************************************/

typedef enum
{
	WS_WAIT_FOR_HANDSHAKE,
	WS_HANDSHAKED,
	WS_TO_CLOSE, // CLOSE frame has been received
	WS_CLOSED, // response CLOSE frame has been scheduled to transmit
} ws_state_t;

typedef enum
{
	WS_CLOSE_REASON_NORMAL,
	WS_CLOSE_REASON_BY_PEER,
} ws_close_reason_t;
/**
 * @brief Generate WebSocket handshake response key
 * @param client_key: Client's Sec-WebSocket-Key header value
 * @param output: Buffer for Base64-encoded response key
 * @note Computes SHA-1(client_key + GUID) then Base64 encodes
 */
int ws_handshake_response(char *client_key,  size_t lenin, /* out */  char *output, size_t lenout);


#define WS_OPCODE_TEXT 0x1U
#define WS_OPCODE_BINARY 0x2U
#define WS_OPCODE_PING 0x9U
#define WS_OPCODE_PONG 0xAU
#define WS_OPCODE_CLOSE 0x8U
// WebSocket frame structure (minimal)
typedef struct {
    uint8_t fin;            // FIN bit (1=last fragment)
    uint8_t opcode;         // Frame type (e.g., 0x1=text, 0x8=close)
    uint8_t mask;           // Masking flag (client→server must mask)
    const uint8_t* payload;
    uint32_t payload_len;   // Payload length in bytes
    uint8_t masking_key[4]; // XOR masking key (if present, big-endian)
} WS_Frame;

/**
 * @brief Parse WebSocket frame header
 * @param data: Input buffer starting with frame header
 * @param frame: Output parsed frame metadata
 * @return Header length in bytes, -1 if error
 * @note Supports payload lengths ≤64KB (no 64-bit length)
 */
int ws_parse_frame(uint8_t *data, size_t len, /* out */ WS_Frame *frame);

/**
 * @brief Unmask WebSocket payload data recieved from Client
 * @param payload: Data buffer to unmask (modified in-place)
 * @param len: Payload length
 * @param masking_key: 4-byte XOR key
 * @note RFC 6455 requires masking for client→server frames, server->client path should not be masked
 */
void ws_unmask_payload(/* out */ uint8_t *payload, size_t len, /* in */ uint8_t *masking_key);


int ws_make_pong_frame(uint8_t* buf);

int ws_make_close_frame(uint8_t* buf, ws_close_reason_t reason);

int ws_make_text_frame(uint8_t* buf, size_t lenbuf, const uint8_t* in, size_t lenin);
