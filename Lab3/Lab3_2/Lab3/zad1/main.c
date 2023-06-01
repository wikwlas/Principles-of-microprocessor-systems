#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

const char charrr[] PROGMEM = "Im here";

int main(void){
	lcdinit();
	double pi = 3.1415;
	lcd_set_xy(0, 0);
	printf("Pi %.2f", pi);
	lcd_set_xy(0, 1);
	lcd_write_text_P(charrr);
	//lcd_write_text_xy(8, 1, pi);	
	
		
	while(1) {
		
	}
	
	return 0;
}
