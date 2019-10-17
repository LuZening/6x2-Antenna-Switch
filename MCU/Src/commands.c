#include "commands.h"
#include "Lib485.h"
extern struct Serial485* p485;
// YAESU GS232 compatible commands
// callback functions of each command
const char *commands[N_COMM] = {
//    "R", // Clockwise
	"R", // reset
	"S", // swtich S [0-6] [0-6]\r
	"G", // get G\r Return: [0-6] [0-6]\r
};
bool (*command_calls[N_COMM])(int, char **) = {
		on_485_reset,
		on_485_switch,
		on_485_getalloc,
};


/************************************************
 *             HANDLE RS485 commands            *
 * *********************************************/
bool on_485_reset(int, )
