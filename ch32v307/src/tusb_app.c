
#include "ch32v30x.h"
#include "tusb.h"
#include "critical.h"

//--------------------------------------------------------------------+
// Forward USB interrupt events to TinyUSB IRQ Handler
//--------------------------------------------------------------------+

__attribute__((/*__interrupt__("WCH-Interrupt-fast"),*/ __naked__))
void USBHS_IRQHandler(void) {
	asm volatile ("call USBHS_IRQHandler_impl; mret");
}

__attribute__((__used__, __noinline__)) void USBHS_IRQHandler_impl(void) {
  tud_int_handler(0);
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(char* file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number,
     tex: iprintf("Wrong parameters value: file %s on line %d\r\n", file, line)
   */
  /* USER CODE END 6 */
	iprintf("[USB] Wrong parameters value: file %s on line %d\r\n", file, line);
}
#endif /* USE_FULL_ASSERT */

void tusb_app_init(void) {
	CRITICAL_SECTION({
		RCC_USBCLK48MConfig(RCC_USBCLK48MCLKSource_USBPHY);
		RCC_USBHSPLLCLKConfig(RCC_HSBHSPLLCLKSource_HSE);
		RCC_USBHSConfig(RCC_USBPLL_Div2);
		RCC_USBHSPLLCKREFCLKConfig(RCC_USBHSPLLCKREFCLK_4M);
		RCC_USBHSPHYPLLALIVEcmd(ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_USBHS, ENABLE);
	});

	Delay_Ms(2);

	// init device stack on configured roothub port
	tud_init(BOARD_TUD_RHPORT);
}

