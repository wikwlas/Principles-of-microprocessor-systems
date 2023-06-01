#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"
#include "twi.h"
#include "rtc.h"

ISR(INT0_vect) {
	struct datetime dt;
	rtc_get_date_time(&dt);
	lcd_set_xy(0,0);
	printf("Date: %02d/%02d/20%02d", dt.day, dt.month, dt.year);
	lcd_set_xy(0, 1);
	printf("Time: %02d:%02d:%02d", dt.hour, dt.min, dt.sec);
}
int main(void){
	lcdinit();
	struct datetime mydt={
		.sec = 0,
		.min = 35,
		.hour  = 13,
		.day = 18,
		.weekday = 02,
		.month = 04,
		.year = 23
	};
	I2C_init();
	rtc_set_date_time(&mydt);
	
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
	
	sei();
	
	
	while(1){
		
	}
	
	return 0;
}
