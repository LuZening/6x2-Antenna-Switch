/*
 * CH395.C
 *
 *  Created on: Sep 14, 2019
 *      Author: Zening
 */

#include "CH395.H"
#include "CH395CMD.H"

BOOL CH395TCPServerStart(CH395_TypeDef *pch395, UINT32 ip, UINT16 port)
{
	int i;
	// Initialize Stack
	CH395CMDInitCH395();
	// DHCP
	if(!CH395DHCPEnable(TRUE))
	{
		// DHCP not enabled
		CH395CMDSetIPAddr((UINT8*)(&ip)); // set IP address;
		CH395CMDSetGWIPAddr((UINT8*)(&ip)); // set gateway IP
	}
	// TODO: enable multi connections
	CH395SetStartPara(FUN_PARA_FLAG_TCP_SERVER);
	// set sockets
	for(i=1; i<4; ++i)
	{
		CH395SetSocketProtType(i, PROTO_TYPE_TCP); // set SOCKET 0 to be TCP server
		CH395SetSocketSourPort(i, port);
	}

	CH395OpenSocket(0); // open socket 0
	CH395TCPListen(0); // start lisening
}


