/*
 * CH395.C
 *
 *  Created on: Sep 14, 2019
 *      Author: Zening
 */

#include <CH395.h>
#include <CH395CMD.h>
#include "Delay.h"

void CH395SetBuffer() // use all 8 sockets, each socket 1.5KB x 2 buffer (each buffer block has 512B)
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
	//enable multi connections
	// !!!setStartPara must before Init!!!!
	CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER | FUN_PARA_FLAG_LOW_PWR);
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
//	HAL_Delay(100);

	// set multiple sockets
	uint8_t err = 0;
	if(port_http_server > 0)
	{

		// open socket 0 for listening HTTP
		CH395SetSocketProtType(0, PROTO_TYPE_TCP); // set SOC
		CH395SetSocketSourPort(0, port_http_server); // listen on HTTP port
		CH395OpenSocket(0);
		err = CH395TCPListen(0); // start lisening

		// open socket 5 for listening TCP
		CH395SetSocketProtType(CH395_TCP_LISTEN_SOCK, PROTO_TYPE_TCP); // set SOC
		CH395SetSocketSourPort(CH395_TCP_LISTEN_SOCK, port_tcp_server); // listen on TCP port
		CH395OpenSocket(CH395_TCP_LISTEN_SOCK);
		err = CH395TCPListen(CH395_TCP_LISTEN_SOCK); // start lisening

		// open other multiple sockets
		for(i=1; i < 7; ++i)
		{
			if(i == CH395_TCP_LISTEN_SOCK) continue; // skip port 5, for it's a listener
			CH395_protocol_t proto = ch395.cfg.protocols[i];
			uint16_t port = ch395.cfg.ports[i];
			if(proto != CH395_PROTOCOL_NOT_USED && port > 0)
			{
				CH395SetSocketSourPort(i, port); // listen on HTTP port
				CH395SetSocketProtType(i, PROTO_TYPE_TCP); // set SOCKET  to be TCP server
			}
		}

		/* UDP Server BEGIN */
		// open socket 7 for listening UDP
		i = 7;
		CH395SetSocketProtType(i, PROTO_TYPE_UDP); // set SOC
		// enter UDP server mode by setting IP as 255.255.255.255
		uint8_t IP_UDP[4] = {255,255,255,255};
		CH395SetSocketDesIP(i, IP_UDP);
		CH395SetSocketDesPort(i, ch395.cfg.ports[i]); // listen on TCP port
		CH395SetSocketSourPort(i, ch395.cfg.ports[i]); // listen on TCP port
		CH395OpenSocket(i);
		/* UDP Server END */
	}

	return (err == 0);
}


