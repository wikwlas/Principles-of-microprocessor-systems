#include "ds18b20.h"

uint8_t DS18B20_init(void) {
	OW_reset();
	return OW_reset();
}

uint8_t DS18B20_start(void) {
	if(OW_reset()) return 1;
	OW_send(0xCC);
	OW_send(0x44);
	return 0;
}

int16_t DS18B20_read(void) {
	int16_t temperature;
	OW_reset();
	OW_send(0xCC);
	OW_send(0xBE);
	temperature = OW_recv();
	temperature |= ((int16_t)OW_recv())<<8;
	return temperature;
}
