/*
 * SerialOverTCP.c
 *
 *  Created on: Mar 9, 2024
 *      Author: cpholzn
 */


#include "SerialOverTCP.h"


const char USR_DETECTION_MSG[LEN_USR_DETECTION_MSG+1] = "0123456789012345678901234567890123456789";

size_t prepare_USR_response_msg(const USR_response_t *p, char* buf)
{
	const uint8_t LEN_IP = 4;
	const uint8_t LEN_MAC = 6;
	char* s = buf;
	int8_t i = 0; // i must be signed, for counting down can go below 0

	// MAC 6B
	for(i = LEN_MAC - 1; i >= 0; --i )
	{
		*(s++) = p->MAC[i];
	}
	// version 1B
	*(s++) = 0x42;

	/*  21 Bytes */
	//
	for(i = LEN_IP - 1; i >=0; -- i )
	{
		*(s++) = p->IPv4_target[i];
	}
	//
	for(i = 0; i < 2; ++ i )
	{
		*(s++) = (p->port_target >> (i * 8)) & 0xffU;
	}
	//
	for(i = LEN_IP - 1; i >=0; -- i )
	{
		*(s++) = p->IPv4_self[i];
	}
	//
	for(i = 0; i < 2; ++ i )
	{
		*(s++) = (p->port_self >> (i * 8)) & 0xffU;
	}
	//
	for(i = LEN_IP - 1; i >=0; -- i )
	{
		*(s++) = p->IPv4_gateway[i];
	}
	//
	*(s++) = p->mode;
	// baud
	for(i = 0; i < 3; ++i )
	{
		*(s++) = (uint8_t)((p->baud >> 8*(i)) & 0xffU);
	}

	*(s++) = p->stopbit_mode;

	/*  21 Bytes END */
	// ID
	for(i=0; i< 3; ++i)
		*(s++) = 0;
	// mask
	for(i = LEN_IP - 1; i >=0; -- i )
	{
		*(s++) = p->IPv4_mask[i];
	}

	return s - buf;
}
