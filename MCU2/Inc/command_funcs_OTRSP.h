/*
 * command_funcs.h
 *
 *  Created on: Mar 6, 2023
 *      Author: cpholzn
 */

/* OTRSP protocol
 *
 * */

#ifndef INC_COMMAND_FUNCS_H_
#define INC_COMMAND_FUNCS_H_
#include <stddef.h>
#include <stdint.h>


#define N_commands_OTRSP 5

typedef int (*cmd_executer_func_t)(uint8_t argc, char** argv, char* ret);


extern const char* commands_OTRSP[];

extern const cmd_executer_func_t command_executers_OTRSP[];




/* fn_ping: ping the connection
 * args:
 * response: ?\r
 */
int fn_ping(uint8_t argc, char** argv, char* ret);

/* fn_get_ant_assignment: get antenna assignment for Radio ?
 * args: RadioNum[1|2]
 * response: AUX{RadioNum}{0~6}\r
 */
int fn_get_ant_assignment(uint8_t argc, char** argv, char* ret);

/* fn_set_ant_assignment: switch antenna
 * args: RadioNum[1|2]
 * response: AUX{RadioNum}{0~6}\r
 */
int fn_set_ant_assignment(uint8_t argc, char** argv, char* ret);

/* fn_set_ant_name:
 * args: AntNum{1~6}
 * response: NAME{AntNum}{str16}\r
 */
int fn_set_ant_name(uint8_t argc, char** argv, char* ret);

/* fn_get_ant_name: switch antenna
 * args: AntNum{1~6}
 * response: NAME{AntNum}{str16}\r
 */
int fn_get_ant_name(uint8_t argc, char** argv, char* ret);

int parse_command_OTRSP(const char* s, uint16_t len, char* ret);

#endif /* INC_COMMAND_FUNCS_H_ */
