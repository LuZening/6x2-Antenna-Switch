/**** Library for 485 communications using DMA *******/


#ifndef __LIB485_H_
#define __LIB485_H_

#include "main.h"
#include "string.h"
#include "PIN.h"
#include <stdbool.h>
#include "Lib485_user_define.h"
#include "kfifo_DMA.h"

#define LIB485_COMMAND_BUFFER_DEPTH 8U // MUST BE PERFECT POWER of 2
#define LIB485_COMMAND_BUFFER_MASK (LIB485_COMMAND_BUFFER_DEPTH - 1U)

typedef enum
{
	DIR_RX = 0,
	DIR_TX = 1,
} Serial485_direction_t;

typedef struct
{
//    uint32_t baud;
    UART_HandleTypeDef *pSerial;
    PIN_typedef pin_RW;
    uint8_t* rx_fifo_buf;
    uint16_t fifo_size; // must be full power of 2, recommended 256
} Serial485_cfg_t;

typedef struct
{
	// 4-byte align
    uint8_t rx_dma_bufferA[LIB485_DMA_BUFFER_SIZE];
    uint8_t rx_dma_bufferB[LIB485_DMA_BUFFER_SIZE];
    uint8_t tx_dma_buffer[LIB485_DMA_BUFFER_SIZE];
    uint8_t rx_buffer_id;
	Serial485_cfg_t cfg;
    KFIFO_DMA rx_ring_fifo;
    // stores the heads of the commands in the FIFO
    uint16_t command_sizes[LIB485_COMMAND_BUFFER_DEPTH];
    uint8_t command_out, command_in;

    uint8_t busy; // clear this flag in tx complete IRQ
    Serial485_direction_t dir;
} Serial485;

extern Serial485 *p485;

void begin_serial485(Serial485 *p485, const Serial485_cfg_t* pCfg);
void start_receive_DMA(Serial485 *p485);
void set_direction_serial485(Serial485 *p485, Serial485_direction_t dir);
void send_serial485(Serial485 *p485, const char *buffer_send, uint16_t len);
int read_command_from_serial485(Serial485 *p485, char* buf, uint16_t bufsize);
void test_485_blockmode(Serial485 *p485);
#endif
