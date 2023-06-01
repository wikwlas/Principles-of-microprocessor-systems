#include <avr/io.h>
#include "longdelay.h"

#define MOTOR PORTC
#define MOTOR_DIR DDRC

#define BUTTONS PIND
#define BUTTONS_DIR DDRD
#define ON_BUTTON PD0
#define DIR_BUTTON PD1
#define FASTER_BUTTON PD2
#define SLOWER_BUTTON PD3

#define BUTTONS_PULLUP PORTD

int main(void){
	MOTOR_DIR |= 0x0f;
	BUTTONS_DIR &= ~(0x0f);
	BUTTONS_PULLUP = 0x0f;
	
	uint8_t stan = 0;
	uint8_t on = 0;
	uint8_t dir = 0;
	uint8_t faster = 0;
	uint8_t slower = 0;
	uint16_t my_delay = 5;
	
	while(1){
		if (!(BUTTONS & (1<<ON_BUTTON))) {
			on ^= 1; // toggle, przelaczanie stanu na przeciwny
			_delay_ms(10);
		}
		if (!(BUTTONS & (1<<DIR_BUTTON))) {
				dir ^= 1; // toggle, przelaczanie stanu na przeciwny
				_delay_ms(10);
			}
		if (!(BUTTONS & (1<<FASTER_BUTTON))) {
				if (my_delay > 3) my_delay--;
				_delay_ms(10);
			}
		if (!(BUTTONS & (1<<SLOWER_BUTTON))) {
				if (my_delay < 51) my_delay++;
				_delay_ms(10);
			}
			
		if (on) {
			MOTOR = (1<<stan);
			if (dir) {
				stan++;
			} else {
				stan--;
			}
			stan %= 4;
			longdelay(my_delay);
		} else {
			MOTOR = 0;
		}
		
	}
	
	return 0;
}
