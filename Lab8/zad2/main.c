#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "lcd.h"
#include "twi.h"
#include "eeprom.h"


#define MAXBUF 256


int main(void){
	uint8_t z;
	char s[] = "Hello World!";
	char buffer[MAXBUF];
	memset(buffer,0,MAXBUF);
	
	I2C_init();
	
	lcdinit();
	lcd_set_xy(0,0);
	printf("Hello");
	
	_delay_ms(2000);
	
	EE_write_byte(0, 'f');
	z = EE_read_byte(0);
	lcd_set_xy(0,1);
	printf("Byte: 0x%02x=%c", z, z);
	
	EE_write_multi(1, strlen(s), (uint8_t*)s);
	
	lcd_set_xy(0,0);
	
	EE_read_multi(1, strlen(s), (uint8_t*)buffer);
	
	printf("%s", buffer);
	while(1){
		
	}
	
	return 0;
}

