#include "commands.h"
#include "Lib485.h"
#include "HTTPServer.h"
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

bool on_485_reset(int argc, char** argv)
{
	return true;
}
bool on_485_switch(int argc, char** argv)
{
	if(argc < 3)
	{
		return false;
	}
	const char *A = argv[1];
	const char *B = argv[2];
	if(A && B)
	{
		uint8_t nA = atou8(A);
		uint8_t nB = atou8(B);
		if(nA <= NUM_ANTENNA && nB<=NUM_ANTENNA)
		{
			switch_Antenna(nA, nB);
			return true;
		}
	}
	return false;
}
bool on_485_getalloc(int argc, char** argv)
{
	return false;
}
