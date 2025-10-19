#include "main.h"
#include "Lib485.h"
#include "commands.h"
#include <string.h>

#define RE 0
#define DE 1

Serial485 _serial485;
Serial485 *p485 = &_serial485;

#define MIN(x,y) ((x < y)?(x):(y))


void start_receive_DMA(Serial485 *p485)
{
	set_direction_serial485(p485, DIR_RX);
    if(p485->rx_buffer_id == 0)
    {
		// start DMA
		HAL_UARTEx_ReceiveToIdle_DMA(p485->cfg.pSerial, p485->rx_dma_bufferA, LIB485_DMA_BUFFER_SIZE);
    }
    else
    {
		// start DMA
		HAL_UARTEx_ReceiveToIdle_DMA(p485->cfg.pSerial, p485->rx_dma_bufferB, LIB485_DMA_BUFFER_SIZE);
    }
}

void begin_serial485(Serial485 *p485, const Serial485_cfg_t* pCfg)
{
	memcpy(&p485->cfg, pCfg, sizeof(Serial485_cfg_t));
    p485->busy = 0;

    // TRICK: store Serial485 pointer in AdvFeatureInit(uint32_t) member
	p485->cfg.pSerial->AdvancedInit.AdvFeatureInit = (uint32_t)(p485);

    // init RX fifo
    kfifo_DMA_static_init(&p485->rx_ring_fifo, pCfg->rx_fifo_buf, pCfg->fifo_size, 0);

    // init command heads and sizes
    p485->command_in = 0;
    p485->command_out = 0;

	// for later IRQ use
    p485->dir = DIR_RX; // keep on RX
    set_direction_serial485(p485, DIR_RX);
    p485->rx_buffer_id = 0;
    // start IDLE interrupt
//    __HAL_UART_ENABLE_IT(p485->cfg.pSerial, UART_IT_IDLE);
//    start_receive_DMA(p485);
}

void set_direction_serial485(Serial485 *p485, Serial485_direction_t dir)
{
	//RE: receive; DE: write
	p485->dir = dir;
	switch(dir)
	{
	case DIR_RX:
		HAL_GPIO_WritePin(p485->cfg.pin_RW.group, p485->cfg.pin_RW.pin, RE);
		break;
	case DIR_TX:
		HAL_GPIO_WritePin(p485->cfg.pin_RW.group, p485->cfg.pin_RW.pin, DE);
		break;
	default:
		HAL_GPIO_WritePin(p485->cfg.pin_RW.group, p485->cfg.pin_RW.pin, RE);
		break;
	}
}

void send_serial485(Serial485 *p485, const char *buffer_send, uint16_t len)
{
	if(len == 0)
		len = strnlen(buffer_send, sizeof(p485->tx_dma_buffer));
	if(len == 0) return;

	set_direction_serial485(p485, DIR_TX);
	memcpy(p485->tx_dma_buffer, buffer_send, len);
	uint32_t tickNow = HAL_GetTick();
	while((p485->busy == 1) && (HAL_GetTick() - tickNow < 100)); // at most wait for 100ms
	p485->busy = 1; // clear this flag in TXE interrupt
    HAL_UART_Transmit_DMA(p485->cfg.pSerial, p485->tx_dma_buffer, (uint16_t)len);
    // when DMA interrupts, clear busy flags
}

int read_command_from_serial485(Serial485 *p485, char* buf, uint16_t bufsize)
{

	if(p485->command_in == p485->command_out)
		goto NO_COMMAND;

//	uint8_t in = p485->command_in & LIB485_COMMAND_BUFFER_MASK;
	uint8_t out =  p485->command_out & LIB485_COMMAND_BUFFER_MASK;
	uint16_t cmd_size = p485->command_sizes[out];
	++p485->command_out;

	kfifo_get(&p485->rx_ring_fifo, buf, MIN(bufsize, cmd_size));
	// the buffer is too small
	if(bufsize < cmd_size)
	{
		p485->rx_ring_fifo.out += cmd_size - bufsize; // force the ring buffer to drop oversize messages
		goto NO_COMMAND;
	}

	// return size of the command
	return cmd_size;
NO_COMMAND:
	//return < 0 when error
	return -1;
}

/**
 *  @Attention will be called on either HC/TC or idle!!! and cannot be distinguished
  * @brief  User implementation of the Reception Event Callback
  *         (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	Serial485* p485_ = (Serial485*)(huart->AdvancedInit.AdvFeatureInit);
	if(p485_ == p485)
	{
		uint8_t* recved_buf = NULL;
		// restart DMA
		if(p485->rx_buffer_id == 0)
		{
			recved_buf = p485->rx_dma_bufferA;
			p485->rx_buffer_id = 1;
		}
		else
		{
			recved_buf = p485->rx_dma_bufferB;
			p485->rx_buffer_id = 0;
		}
		start_receive_DMA(p485);

		p485->command_sizes[(p485->command_in & LIB485_COMMAND_BUFFER_MASK)] = Size;
		++p485->command_in;

		// copy to kfifo
		kfifo_put(&p485->rx_ring_fifo, recved_buf, Size);

		// mark command ends

	}
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	Serial485* p485_ = (Serial485*)(huart->AdvancedInit.AdvFeatureInit);
	if(p485_ == p485)
	{
		p485->busy = 0;
		set_direction_serial485(p485, DIR_RX); //switch back to listen mode
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// restart DMA receiving when error occures
	// restart DMA
	Serial485* p485_ = (Serial485*)(huart->AdvancedInit.AdvFeatureInit);
	if(p485 == p485_)
	{
		p485->busy = 0;
		start_receive_DMA(p485);
	}
}


void test_485_blockmode(Serial485 *p485)
{
	uint16_t lenRead = 0;
	set_direction_serial485(p485, DIR_TX);
	strcpy(p485->rx_dma_bufferA, "1234567890");
	HAL_UART_Transmit(p485->cfg.pSerial, p485->rx_dma_bufferA, 10, HAL_MAX_DELAY);
	set_direction_serial485(p485, DIR_RX);
	while(1)
	{
		set_direction_serial485(p485, DIR_RX);
		HAL_UARTEx_ReceiveToIdle(p485->cfg.pSerial, p485->rx_dma_bufferA, sizeof(p485->rx_dma_bufferA), &lenRead, HAL_MAX_DELAY);
		if(lenRead > 0)
		{
			set_direction_serial485(p485, DIR_TX);
			HAL_UART_Transmit(p485->cfg.pSerial, p485->rx_dma_bufferA, lenRead, HAL_MAX_DELAY);
		}
	}

}

