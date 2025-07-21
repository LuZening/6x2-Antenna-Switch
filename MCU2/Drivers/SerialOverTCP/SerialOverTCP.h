/*
 * SerialOverTCP.h
 *
 *  Created on: Mar 9, 2024
 *      Author: cpholzn
 */

#ifndef SERIALOVERTCP_SERIALOVERTCP_H_
#define SERIALOVERTCP_SERIALOVERTCP_H_

#include<string.h>
#include<stdint.h>

#define LEN_USR_DETECTION_MSG 40U
extern const char USR_DETECTION_MSG[LEN_USR_DETECTION_MSG+1];

typedef struct
{
	uint8_t* MAC;
	uint8_t* IPv4_mask; // Big endian, e.g. 255.255.255.0 -> ff,ff,ff,0
	uint8_t* IPv4_target; //
	uint16_t port_target; //
	uint8_t* IPv4_self; //
	uint16_t port_self; //
	uint8_t* IPv4_gateway;
	uint8_t mode; // 0: UDP 1: TCP Client, 2:UDP Server, 3:TCP Server
	uint32_t baud;
	uint8_t stopbit_mode; // 3: N,8,1
} USR_response_t;

size_t prepare_USR_response_msg(const USR_response_t *p, char* buf);

#endif /* SERIALOVERTCP_SERIALOVERTCP_H_ */
