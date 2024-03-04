#include "main.h"
#include "Lib485.h"
#include "commands.h"
#include <string.h>

#define RE 1
#define DE 0

Serial485 _serial485;
Serial485 *p485 = &_serial485;



static void start_receive_DMA(Serial485 *p485)
{
    p485->rx_head = 0;
    p485->rx_old_pos = 0;
    // start IDLE interrupt
//    __HAL_UART_ENABLE_IT(p485->cfg.pSerial, UART_IT_IDLE);
    // start DMA
    HAL_UARTEx_ReceiveToIdle_DMA(p485->cfg.pSerial, p485->rx_buffer, sizeof(p485->rx_buffer));
}

void begin_serial485(Serial485 *p485, const Serial485_cfg_t* pCfg)
{
	memcpy(&p485->cfg, pCfg, sizeof(Serial485_cfg_t));
    p485->state_RW = RE;
    *(p485->command) = 0;
    p485->len_unprocessed_command = 0;
    HAL_GPIO_WritePin(pCfg->pin_RW.group, pCfg->pin_RW.pin, RE); // toggle receive mode

    // TRICK: store Serial485 pointer in AdvFeatureInit(uint32_t) member
	// for later iterruption use
	p485->cfg.pSerial->AdvancedInit.AdvFeatureInit = (uint32_t)(p485);

    start_receive_DMA(p485);
    // when DMA interrupts, parse command and restart receiving


}

void set_direction_serial485(Serial485 *p485, uint8_t direction)
{
	//RE: receive; DE: write
	p485->state_RW = direction;
	HAL_GPIO_WritePin(p485->cfg.pin_RW.group, p485->cfg.pin_RW.pin, direction);
}

void send_serial485(Serial485 *p485, const char *buffer_send, int16_t len)
{


	if(len < 0)
		len = strnlen(buffer_send, sizeof(p485->tx_buffer));
	if(len == 0) return;
	strncpy(p485->tx_buffer, buffer_send, len);
	while(!p485->busy);
	p485->busy = true;
	set_direction_serial485(p485, DE);
    HAL_UART_Transmit_DMA(p485->cfg.pSerial, p485->tx_buffer, len);
    // when DMA interrupts, clear busy flags
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
	Serial485* p485 = (Serial485*)(huart->AdvancedInit.AdvFeatureInit);
	uint16_t *phead = &(p485->rx_head); // reserved for circular DMA mode
	uint16_t *pold_pos = &(p485->rx_old_pos);
	uint16_t len_command;
  /* check if idle signal received, under non-circular DMA (normal mode)
   * enters here either HC/TC or IDLE
   * case 1: Size == old_pos, means idle received, process data from [header, Size), move head to Size
   * case 2: Size == RX_BUFFER_LEN, means buffer is full (TC), must process, and IDLE int will not be triggered instead
   */
  if(
		  (Size == *pold_pos) // case 1
		  || (Size == sizeof(p485->rx_buffer)) // case 2
		  )
  {
	  if(p485->len_unprocessed_command == 0) // do not overwrite unprocessed command
	  {

		  len_command =  Size - *phead;
		  strncpy(p485->command, p485->rx_buffer + *phead, len_command);
		  p485->command[len_command] = 0;

		  // process command
		  p485->len_unprocessed_command = len_command;
	  }

	  // restart DMA
	  start_receive_DMA(p485);
  }
  else // HC event, do nothing
	  *pold_pos = Size;

}


void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	// restart DMA receiving when error occures
	// restart DMA
	Serial485* p485 = (Serial485*)(huart->AdvancedInit.AdvFeatureInit);
	start_receive_DMA(p485);
}

