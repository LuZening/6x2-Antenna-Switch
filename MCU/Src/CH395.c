/*
 * CH395.C
 *
 *  Created on: Sep 14, 2019
 *      Author: Zening
 */

#include <CH395.h>
#include <CH395CMD.h>
#include "Delay.h"
BOOL CH395TCPServerStart(uint32_t ip, uint16_t port)
{
	uint8_t i;
	// Initialize Stack
	CH395CMDInitCH395();
	// Check PHY
	i=0;
	do
	{
		Delay_ms(100);
		uint8_t phy = CH395CMDGetPHYStatus();
		if(phy != (uint8_t)1) break; // PHY connected
		++i;
	}while(i<5);
	if(i >= 5) return FALSE;
	Delay_ms(200);
	// DHCP
	if(CH395DHCPEnable(TRUE) != 0)
	{
		// DHCP not enabled
		CH395CMDSetIPAddr((uint8_t*)(&ip)); // set IP address;
		CH395CMDSetGWIPAddr((uint8_t*)(&ip)); // set gateway IP
	}
	// TODO: enable multi connections
	CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER);
	// set sockets
	for(i=0; i<CH395_SOCKS_AVAIL; ++i)
	{
		CH395SetSocketProtType(i, PROTO_TYPE_TCP); // set SOCKET 0 to be TCP server
		CH395SetSocketSourPort(i, port);
	}

	CH395OpenSocket(0); // open socket 0
	CH395TCPListen(0); // start lisening
	return TRUE;
}


