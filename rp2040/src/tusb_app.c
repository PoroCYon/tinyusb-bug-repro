
#include "tusb.h"


void tusb_app_init(void) {
	// init device stack on configured roothub port
	tud_init(BOARD_TUD_RHPORT);
}

