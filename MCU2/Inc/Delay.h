/*
 * Delay.h
 *
 *  Created on: Sep 21, 2019
 *      Author: Zening
 */

#ifndef DELAY_H_
#define DELAY_H_

#define SysCLK_Freq 72 // MHz

#include <stm32f0xx_hal.h>

void Delay_us(unsigned int n);
#define Delay_ms(n) HAL_Delay(n)
//#define Delay_ms(n) HAL_Delay(n);
#endif /* DELAY_H_ */
