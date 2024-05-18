
#include <stdio.h>

#include "debug.h"
#include "tusb_app.h"
#include "tusb.h"

#if !defined(PICO_BOARD) && !defined(PICO_BOARD)
#include "system_ch32v30x.h"
#endif

int main(void) {
#if !defined(PICO_BOARD) && !defined(PICO_BOARD)
	// CH32V307 only
	SystemCoreClockUpdate();
#endif
	Delay_Init();

	USART_Printf_Init(115200);
	Delay_Ms(10);

	iprintf("USBD Udisk\r\n");

	tusb_app_init();

	while (1) {
		tud_task();
	}
}

