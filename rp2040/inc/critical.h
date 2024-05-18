
#ifndef CRITICAL_H_
#define CRITICAL_H_

#include <stdint.h>
#include <stdbool.h>

#include <hardware/sync.h>


static inline uint32_t enter_critical_section(void) {
	return save_and_disable_interrupts();
}
static inline void exit_critical_section(uint32_t dis) {
	restore_interrupts(dis);
}

#define CRITICAL_SECTION(...) do { \
		uint32_t __v = enter_critical_section(); \
		do{__VA_ARGS__;}while(0);\
		exit_critical_section(__v); \
	} while (0); \

#endif

