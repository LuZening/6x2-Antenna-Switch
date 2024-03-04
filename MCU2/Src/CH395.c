/*
 * CH395.C
 *
 *  Created on: Sep 14, 2019
 *      Author: Zening
 */

#include <CH395.h>
#include <CH395CMD.h>
#include "Delay.h"
void CH395SetBuffer() // use all 8 sockets, each socket 1.5KB x 2 buffer
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

BOOL CH395TCPServerStart(uint32_t ip, uint16_t port_http_server, uint16_t port_tcp_server)
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
		HAL_Delay(500);
	}while(i<20);
	if(i >= 20) return false;
	// DHCP
	if(CH395DHCPEnable(true) != 0)
	{
		// DHCP not enabled
//		CH395CMDSetIPAddr((uint8_t*)(&ip)); // set IP address;
//		CH395CMDSetGWIPAddr((uint8_t*)(&ip)); // set gateway IP
		HAL_Delay(100);
	}
	HAL_Delay(100);
	//enable multi connections
	CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER);
	// set sockets
	uint8_t err = 0;
	if(port_http_server > 0)
	{
		for(i=0; i < CH395_SOCKS_AVAIL; ++i)
		{
			CH395_protocol_t proto = ch395.cfg.protocols[i];
			uint16_t port = ch395.cfg.ports[i];
			if(proto != CH395_PROTOCOL_NOT_USED && port > 0)
			{
				CH395SetSocketProtType(i, PROTO_TYPE_TCP); // set SOCKET  to be TCP server
				CH395SetSocketSourPort(i, port); // listen on HTTP port
			}
		}
		CH395OpenSocket(0); // open socket 0
		err = CH395TCPListen(0); // start lisening
	}

	return (err == 0);
}


