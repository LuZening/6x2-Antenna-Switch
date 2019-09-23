/*
 * Delay.c
 *
 *  Created on: Sep 21, 2019
 *      Author: Zening
 */

#include "Delay.h"

void Delay_us(unsigned int n) {
	uint32_t temp;
	SysTick->LOAD = 8 * n; // SYSCLK Frequency MHz * us
	SysTick->VAL = 0X00; //清空计数器
	SysTick->CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
	do
	{
		temp = SysTick->CTRL; //读取当前倒计数值
	} while ((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
	SysTick->CTRL = 0x00; //关闭计数器
	SysTick->VAL = 0X00; //清空计数器
}n
