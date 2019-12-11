#include "main.h"
#include "Lib485.h"
#include "commands.h"
#include "Delay.h"
struct Serial485 _serial485;
struct Serial485 *p485 = &_serial485;
#define OWN 0
#define GS232 1
#define COMPATIBILITY GS232
#ifdef RW485_ON_CH395
#include "CH395.h"
#endif
//#define COMPATIBILITY OWN
// Compliance with CRC16 MODBUS procotols
uint CRC16(char *s, int len) // chr_termin excluded
{
    unsigned int CRC_reg = 0xffff; // initialize 16-bit CRC register
    while (len-- && (*s > 0))
    {
        CRC_reg = CRC_reg ^ *s;
        s++;
        for (int i = 0; i < 8; ++i)
        {
            if (CRC_reg & 0x1) // shift 1
            {
                CRC_reg >>= 1;
                CRC_reg ^= CRC_POLY;
            }
            else // shift 0
                CRC_reg >>= 1;
        }
    }
    return CRC_reg;
}
void begin_serial485(struct Serial485 *p485, UART_HandleTypeDef *pSerial, PIN_typedef pin_RW, int timer_ms)
{
    int i;
    p485->state_RW = RE;
    p485->timeout_tx = 0;
    p485->idx_rx = 0;
    p485->idx_command = 0;
    *(p485->command) = 0;
    p485->argc = 0;
    p485->argv[0] = p485->argv0;
    p485->is_command_ready = false;
    p485->timer_ms = timer_ms;
    p485->timeout_clear_rx = TIMEOUT_RX;
    p485->n_available = 0;
    p485->pSerial = pSerial;
    p485->pin_RW = pin_RW;
    HAL_GPIO_WritePin(pin_RW.group, pin_RW.pin, RE); // toggle receive mode
}

void set_direction485(struct Serial485 *p485, uint8_t direction)
{
	//RE: receive; DE: write
#ifdef RW485_ON_CH395
	uint8_t v = CH395ReadGPIOAddr(GPIO_IN_REG);
	switch(direction)
	{
	case GPIO_PIN_SET:
		v |= (1<<RW485_ON_CH395);
		break;
	case GPIO_PIN_RESET:
		v &= ~(1 << RW485_ON_CH395);
	}
	CH395WriteGPIOAddr(GPIO_OUT_REG, v);
#else
	HAL_GPIO_WritePin(p485->pin_RW.group, p485->pin_RW.pin, direction);
#endif
}

void send_serial485(struct Serial485 *p485, const char *str)
{
    int len_content;
    set_direction485(p485, DE);
    p485->state_RW = DE;
    len_content = strlen(str);
    HAL_UART_Transmit(p485->pSerial, str, len_content, HAL_MAX_DELAY);
    Delay_us(20);
    set_direction485(p485, RE);
    p485->state_RW = RE;
}

//void onReceived_serial485(struct Serial485 *p485)
//{
//	p485->timeout_clear_rx = TIMEOUT_RX; // reset rx clear timer
//	// if c is a deliminator, we have a complete command
//	char c = *(p485->rx_buffer);
//	if (strchr(DELIM_485, c) != NULL) // c is a deliminator
//	{
//		if (p485->idx_command > 0) // parse the command if the command buffer is not empty
//		{
//			p485->command[p485->idx_command] = 0;
//			// eliminate the trailling deliminating characters
//			parse_command(p485);
//			// execute command if a command has been parsed
//			// Do not execute command in an interrupr service function
//			// execute it in the main loop
//			if (p485->is_command_ready)
//			{
//				if (execute_command(p485->argc, (char **)(p485->argv)))
//				{
//					send_serial485(p485, "\r"); // succeeded
//				}
//				else
//				{
//					send_serial485(p485, "?>\r"); // bad command
//				}
//				// clean up the command flags
//				p485->is_command_ready = false;
//				p485->idx_command = 0;
//				p485->argc = 0;
//			}
//		} // discard the deliminator if the command is null
//	}
//	else
//	{
//		p485->command[(p485->idx_command)++] = c;
//	}
//}

// call this function at each interval
// switches DE to RE when transmission finished
void handle_serial485(struct Serial485 *p485)
{
        // countdown rx clear timer
	char c;
	if(HAL_OK == HAL_UART_Receive(p485->pSerial, &c, 1, 0))
	{
		p485->timeout_clear_rx = TIMEOUT_RX; // reset rx clear timer
		if (strchr(DELIM_485, c) != NULL) // c is a deliminator
		{
			if (p485->idx_command > 0) // parse the command if the command buffer is not empty
			{
				p485->command[p485->idx_command] = 0;
				// eliminate the trailling deliminating characters
				parse_command(p485);
				// execute command if a command has been parsed
				// Do not execute command in an interrupr service function
				// execute it in the main loop
				if (p485->is_command_ready)
				{
					if (execute_command(p485->argc, (char **)(p485->argv)))
					{
						send_serial485(p485, "\r"); // succeeded
					}
					else
					{
						send_serial485(p485, "?>\r"); // bad command
					}
					// clean up the command flags
					p485->is_command_ready = false;
					p485->idx_command = 0;
					p485->argc = 0;
				}
			} // discard the deliminator if the command is null
		}
		else
		{
			p485->command[(p485->idx_command)++] = c;
		}
	}
	else
	{
        if (p485->timeout_clear_rx > 0)
        {
            p485->timeout_clear_rx -= p485->timer_ms;
            if (p485->timeout_clear_rx <= 0)
            {
                p485->idx_command = 0; // clear the rx buffer
                p485->command[0] = 0;
            }
        }
	}
}

void parse_command(struct Serial485 *p485)
{
    char *command = p485->command;
    char *tok;
    p485->argc = 0;
#if COMPATIBILITY == GS232
    p485->argv[0][0] = command[0];
    if (p485->idx_command > 1 &&
        command[0] != 'M' && command[1] >= '0' && command[1] <= '9') // handle cases like O2
    {
        p485->argv[0][1] = command[1];
        p485->argv[0][2] = 0;
        command+=2;
    }
    else
    {
        p485->argv[0][1] = 0;
        command++;
    }
    p485->argc = 1;
    tok = strtok(command, DELIM_COMM_485);
    while (tok)
    {
        p485->argv[(p485->argc)++] = tok;
        tok = strtok(NULL, DELIM_COMM_485);
    }
#elif COMPATIBILITY == OWN
    tok = strtok(command, DELIM_COMM_485);
    while (tok)
    {
        p485->argv[(p485->argc)++] = tok;
        tok = strtok(NULL, DELIM_COMM_485);
    }
#endif
    p485->is_active = true;
    p485->is_command_ready = true;
}

bool execute_command(int argc, char** argv)
{
    int i;
    bool res = false;
    // search in command list
    for (i = 0; i < N_COMM; ++i)
    {
        if (strcmp(argv[0], commands[i]) == 0) // if any of the listed commands matches
        {
            res = (*command_calls[i])(argc, argv);
            break;
        }
    }
    return res;
}
