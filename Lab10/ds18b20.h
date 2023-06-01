#ifndef DS18B20_H_
#define DS18B20_H_

#include "iwire.h"

uint8_t DS18B20_init(void);
uint8_t DS18B20_start(void);
int16_t DS18B20_read(void);

#endif

