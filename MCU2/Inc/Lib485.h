/**** Library for 485 communications using DMA *******/


#ifndef __LIB485_H_
#define __LIB485_H_

#include "main.h"
#include "string.h"
#include "PIN.h"
#include <stdbool.h>
#include "Lib485_user_define.h"

typedef struct
{
//    uint32_t baud;
    UART_HandleTypeDef *pSerial;
    PIN_typedef pin_RW;
} Serial485_cfg_t;

typedef struct
{
	Serial485_cfg_t cfg;
    char command[LIB485_COMM_BUFFER_SIZE];
    char rx_buffer[LIB485_COMM_BUFFER_SIZE];
    char tx_buffer[LIB485_COMM_BUFFER_SIZE];
    volatile uint16_t len_unprocessed_command;
    uint8_t state_RW;
    volatile bool busy; // indicate if the 485 port is in use
    // for Rx interrupt
    uint16_t rx_head;
    uint16_t rx_old_pos;
} Serial485;

extern Serial485 *p485;


void begin_serial485(Serial485 *p485, const Serial485_cfg_t* pCfg);
void set_direction_serial485(Serial485 *p485, uint8_t direction);
void send_serial485(Serial485 *p485, const char *buffer_send, int16_t len);

#endif
