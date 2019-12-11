/**** Library for 485 communications *******/

#ifndef __LIB485_H_
#define __LIB485_H_

#include "main.h"
#include "string.h"
#include "PIN.h"
#include <stdbool.h>
#define CRC_POLY 0xA001
#define RE GPIO_PIN_SET
#define DE GPIO_PIN_RESET
#define COMM_BUFFER_SIZE 32
#define N_PARAM_MAX 4
#define PARAM_LEN_MAX 5
#define TIMEOUT_RX 1000
#define DELIM_485 "\r\n"
#define DELIM_COMM_485 " \r\n\t"

#define COMPATIBILITY_OWN 0
#define COMPATIBILITY_GS232 1


typedef unsigned char BYTE;
typedef unsigned int uint;

extern int compatibility;

struct Serial485
{
    char command[COMM_BUFFER_SIZE];
    char rx_buffer[2];
    char tx_buffer[16];
    uint8_t idx_rx;
    int idx_command;
    int argc;
    char *argv[N_PARAM_MAX];
    char argv0[4]; // reserve memory for argv[0]
    bool is_command_ready;
    BYTE state_RW;
    uint baud;
    UART_HandleTypeDef *pSerial;
    PIN_typedef pin_RW;
    int timer_ms; // timer interval
    int timeout_tx;       // estimated transmission time in ms
    int timeout_clear_rx; // time to clear RX buffer
    unsigned int n_available;
    bool is_active; // indicate if the 485 port is in use
};

extern struct Serial485 *p485;

uint CRC16(char *s, int len); // chr_termin excluded
void begin_serial485(struct Serial485 *p485, UART_HandleTypeDef *pSerial, PIN_typedef pin_RW, int timer_ms);
void send_serial485(struct Serial485 *p485, const char *buffer_send);
void onReceived_serial485(struct Serial485 *p485);
void handle_serial485(struct Serial485 *p485); // clear RX buffer at certain intervals
void parse_command(struct Serial485 *p485);
bool execute_command(int argc, char** argv);



#endif
