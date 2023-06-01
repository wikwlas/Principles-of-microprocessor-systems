#include "longdelay.h"

void longdelay(uint16_t n) {
	for (uint16_t i = 0; i < n; i++) {
		_delay_ms(1);
	}
}
