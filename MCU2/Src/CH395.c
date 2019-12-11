/*
 * CH395.C
 *
 *  Created on: Sep 14, 2019
 *      Author: Zening
 */

#include <CH395.h>
#include <CH395CMD.h>
#include "Delay.h"
void CH395SetBuffer() // use all 8 sockets
{
	uint8_t i;
	uint8_t blk = 0;
	for(i=0; i < 8; ++i)
	{
		CH395SetSocketRecvBuf(i, blk, 3);
		blk += 3;
		CH395SetSocketSendBuf(i, blk, 3);
		blk += 3;
	}
}

BOOL CH395TCPServerStart(uint32_t ip, uint16_t port)
{
	uint8_t i;
	// Initialize Stack
	CH395CMDInitCH395();
	ch395.RX_received = 0;
	ch395.TX_available= 0;
	ch395.SOCK_responding = -1;
	// Check PHY
	i=0;
	do
	{
		uint8_t phy = CH395CMDGetPHYStatus();
		if(phy != PHY_DISCONN) break; // PHY connected
		++i;
		Delay_ms(100);
	}while(i<20);
	if(i >= 20) return false;
	// DHCP
	if(CH395DHCPEnable(true) != 0)
	{
		// DHCP not enabled
		CH395CMDSetIPAddr((uint8_t*)(&ip)); // set IP address;
		CH395CMDSetGWIPAddr((uint8_t*)(&ip)); // set gateway IP
	}
	//enable multi connections
	CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER);
	// set sockets
	for(i=0; i<CH395_SOCKS_AVAIL; ++i)
	{
		CH395SetSocketProtType(i, PROTO_TYPE_TCP); // set SOCKET 0 to be TCP server
		CH395SetSocketSourPort(i, port);
	}

	CH395OpenSocket(0); // open socket 0
	uint8_t s = CH395TCPListen(0); // start lisening
	return true;
}


