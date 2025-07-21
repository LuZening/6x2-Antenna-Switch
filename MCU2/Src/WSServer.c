/*
 * WSServer.c
 *
 *  Created on: Jul 17, 2025
 *      Author: cpholzn
 */


#define __ON_BOARD_
//#define __DEBUG_

#ifdef __ON_BOARD_
#include <CH395.h>
#endif
#include "main.h"
#include "FS.h"
#include "Delay.h"
#include <string.h>
#include "my_websocket.h"
