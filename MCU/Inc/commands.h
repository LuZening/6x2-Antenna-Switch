#ifndef __COMMANDS_H_
#define __COMMANDS_H_

// define compatibility level
#define GS232

#define COMM_LEN 16
#define N_COMM 3 // number of commands


bool CW_command(int argc, char **argv);
bool CCW_command(int argc, char **argv);
bool stop_azi_rotation_command(int argc, char **argv);
bool read_direction_command(int argc, char **argv);
bool rotate_to_command(int argc, char **argv);
bool all_stop_command(int argc, char **argv);
bool full_calib_command(int argc, char **argv);

extern const char *commands[N_COMM];
extern bool (*command_calls[N_COMM])(int, char **);

#endif
