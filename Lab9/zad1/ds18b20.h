#ifndef _DS18B20_H
#define _DS18B20_H

#include "i1wire.h"

uint8_t DS18B20_init(void);
uint8_t DS18B20_start(void);
int16_t DS18B20_read(void);

#endif
