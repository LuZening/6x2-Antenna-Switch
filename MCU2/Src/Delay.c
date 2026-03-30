/*
 * Delay.c
 *
 *  Created on: Sep 21, 2019
 *      Author: Zening
 */

#include "Delay.h"

void Delay_us(unsigned int n) {
	volatile int t = n * SysCLK_Freq / 8; // 72: MHz
	while(--t) {__NOP();};
}
