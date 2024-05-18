/********************************** (C) COPYRIGHT  *******************************
* File Name          : debug.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : This file contains all the functions prototypes for UART
*                      Printf , Delay functions.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include "ch32v30x.h"

#include "debug.h"

#define AAA __attribute__((__no_sanitize_address__))
#define UGH __attribute__((__no_sanitize_address__, __used__, __noinline__))

AAA static void uart_init_dbg(void) {
	RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1;
	GPIOA->CFGHR &= ~(0xf<<(4*1));
	GPIOA->CFGHR |= (GPIO_Speed_10MHz | GPIO_Mode_AF_PP)<<(4*1);
	USART1->CTLR1 = USART_WordLength_8b | USART_Parity_No | USART_Mode_Tx;
	USART1->CTLR2 = USART_StopBits_1;
	USART1->CTLR3 = USART_HardwareFlowControl_None;
	USART1->BRR = (((144*1000*1000) + (115200)/2) / (115200));
	USART1->CTLR1 |= 1<<13;//USART_UE;
}

AAA static void uart_writebuf(const void* src_, size_t t) {
	const uint8_t* src = src_;
	for (size_t i = 0; i < t; ++i) {
		while (!(USART1->STATR & USART_FLAG_TC))
			;
		USART1->DATAR = src[i];
	}
}
AAA static void uart_writestr(const void* src_) {
	for (const uint8_t* src = src_; *src; ++src) {
		while (!(USART1->STATR & USART_FLAG_TC))
			;
		USART1->DATAR = *src;
	}
}
AAA static char nyb2hex(uint8_t v) {
	if (v >= 0xa) return v + 'a' - 0xa;
	else return v + '0';
}
AAA static void uart_writehex(uint32_t v) {
	char buf[8];
	for (size_t i = 0; i < 8; ++i) {
		buf[7^i] = nyb2hex((v >> 4*i) & 0xf);
	}
	uart_writebuf(buf, 8);
}

static uint8_t  p_us = 0;
static uint16_t p_ms = 0;

/*********************************************************************
 * @fn      Delay_Init
 *
 * @brief   Initializes Delay Funcation.
 *
 * @return  none
 */
AAA void Delay_Init(void) {
	p_us = SystemCoreClock / 8000000;
	p_ms = (uint16_t)p_us * 1000;
}

/*********************************************************************
 * @fn      Delay_Us
 *
 * @brief   Microsecond Delay Time.
 *
 * @param   n - Microsecond number.
 *
 * @return  None
 */
AAA void Delay_Us(uint32_t n) {
	SysTick->SR &= ~(1 << 0);
	uint32_t i = (uint32_t)n * p_us;

	SysTick->CMP = i;
	SysTick->CTLR |= (1 << 4) | (1 << 5) | (1 << 0);

	while((SysTick->SR & (1 << 0)) != (1 << 0))
		;
	SysTick->CTLR &= ~(1 << 0);
}

/*********************************************************************
 * @fn      Delay_Ms
 *
 * @brief   Millisecond Delay Time.
 *
 * @param   n - Millisecond number.
 *
 * @return  None
 */
AAA void Delay_Ms(uint32_t n) {
	SysTick->SR &= ~(1 << 0);
	uint32_t i = (uint32_t)n * p_ms;

	SysTick->CMP = i;
	SysTick->CTLR |= (1 << 4) | (1 << 5) | (1 << 0);

	while((SysTick->SR & (1 << 0)) != (1 << 0))
		;
	SysTick->CTLR &= ~(1 << 0);
}

/*********************************************************************
 * @fn      USART_Printf_Init
 *
 * @brief   Initializes the USARTx peripheral.
 *
 * @param   baudrate - USART communication baud rate.
 *
 * @return  None
 */
AAA void USART_Printf_Init(uint32_t baudrate) {
	uart_init_dbg();
}

/*********************************************************************
 * @fn      _write
 *
 * @brief   Support Printf Function
 *
 * @param   *buf - UART send Data.
 *          size - Data length
 *
 * @return  size: Data length
 */
AAA __attribute__((used)) int _write(int fd, char *buf, int size) {
	uart_writebuf(buf, size);
	return size;
}

/*********************************************************************
 * @fn      _sbrk
 *
 * @brief   Change the spatial position of data segment.
 *
 * @return  size: Data length
 */
AAA void *_sbrk(ptrdiff_t incr) {
	extern char _end[];
	extern char _heap_end[];
	static char *curbrk = _end;

	if ((curbrk + incr < _end) || (curbrk + incr > _heap_end))
	return NULL - 1;

	curbrk += incr;
	return curbrk - incr;
}

__attribute__((__used__, __noinline__))
AAA void hardfault_impl(void) {
	uint32_t v_mepc, v_mcause, v_mtval;

	uart_init_dbg();

	v_mepc = __get_MEPC();
	v_mcause = __get_MCAUSE();
	v_mtval = __get_MTVAL();

	uart_writestr("hardfault\r\n");

	uart_writestr("mepc: ");
	uart_writehex(v_mepc);
	uart_writestr("\r\nmcause: ");
	uart_writehex(v_mcause);
	uart_writestr("\r\nmtval: ");
	uart_writehex(v_mtval);
	uart_writestr("\r\n");

	while(1);
}

__attribute__((/*__section__(".highcode"),*/ __naked__))
//__attribute__((/*interrupt("WCH-Interrupt-fast")*/))
AAA void HardFault_Handler(void) {
	asm volatile("call hardfault_impl; mret");
}
