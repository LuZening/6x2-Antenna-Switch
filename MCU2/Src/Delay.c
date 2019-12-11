/*
 * Delay.c
 *
 *  Created on: Sep 21, 2019
 *      Author: Zening
 */

#include "Delay.h"

void Delay_us(unsigned int n) {
	unsigned int t = n * SysCLK_Freq /2; // 32: MHz
	while(--t){__NOP();};
}
