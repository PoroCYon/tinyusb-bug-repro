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

#include "debug.h"
#include <pico/stdlib.h>
#include "stdio_uart_dma.h"

#define AAA __attribute__((__no_sanitize_address__))
#define UGH __attribute__((__no_sanitize_address__, __used__, __noinline__))

/*********************************************************************
 * @fn      Delay_Init
 *
 * @brief   Initializes Delay Funcation.
 *
 * @return  none
 */
AAA void Delay_Init(void) {
	/*p_us = SystemCoreClock / 8000000;
	p_ms = (uint16_t)p_us * 1000;*/
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
	busy_wait_us_32(n);
	/*SysTick->SR &= ~(1 << 0);
	uint32_t i = (uint32_t)n * p_us;

	SysTick->CMP = i;
	SysTick->CTLR |= (1 << 4) | (1 << 5) | (1 << 0);

	while((SysTick->SR & (1 << 0)) != (1 << 0))
		;
	SysTick->CTLR &= ~(1 << 0);*/
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
	busy_wait_ms(n);
	/*SysTick->SR &= ~(1 << 0);
	uint32_t i = (uint32_t)n * p_ms;

	SysTick->CMP = i;
	SysTick->CTLR |= (1 << 4) | (1 << 5) | (1 << 0);

	while((SysTick->SR & (1 << 0)) != (1 << 0))
		;
	SysTick->CTLR &= ~(1 << 0);*/
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
	stdio_uart_dma_init();
}

