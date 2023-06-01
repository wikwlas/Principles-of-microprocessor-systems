#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "i1wire.h"
#include "ds18b20.h"
#include "rs232.h"

int main(void){
	int16_t temperature;
	lcdinit();
	DS18B20_init();
	
	
	
	
	char text[16];
	//Timer
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<CS11);
	
	USART_init();
	sei();

	_delay_ms(2000);
	
	while(1){
		if(decode) {
			if (strncmp((void* )bufor, "temp", 4) == 0) {
				DS18B20_start();
				while( ! OW_check);
				temperature = DS18B20_read();
				//lcd_set_xy(0, 0);
				sprintf(text, "\r\nT=%3d.%04d \337C", temperature>>4, (temperature & 0x0f)*625);
				//printf("T=%3d.%04d \337C     ", temperature>>4, (temperature & 0x0f)*625);
				USART_send(text);
				//lcd_set_xy(0, 1);
				//printf("T=%5.1f \337C       ", ((double)temperature)*0.0625);
			}
			else USART_send("\r\nUnknown command\r\n");
			decode = 0;
			indeks = 0;
			memset((void *)bufor, 0, MAX_BUF);
		}
	}
	
	return 0;
}
