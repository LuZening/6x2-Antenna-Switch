#ifndef __COMMANDS_H_
#define __COMMANDS_H_

#include "main.h"
// define compatibility level
#define GS232

#define COMM_LEN 16
#define N_COMM 3 // number of commands


extern const char *commands[N_COMM];
extern bool (*command_calls[N_COMM])(int, char **);

bool on_485_reset(int, char**);
bool on_485_switch(int, char**);
bool on_485_getalloc(int, char**);
#endif
