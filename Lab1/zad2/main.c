#include <avr/io.h>
#include "longdelay.h"

#define BLINK_LED PC6
#define BUTTON PD0
#define BUTTONDIR DDRD
#define BUTTONPULLUP PORTD
#define BUTTONPIN PIND
#define LED PC7
#define LEDDIR DDRC
#define LEDPORT PORTC


int main(void){
	LEDDIR = (1<<LED)|(1<<BLINK_LED);
	LEDPORT = (1<<LED)|(1<<BLINK_LED);
	
	BUTTONDIR &=~(1<<BUTTON);
	BUTTONPULLUP |= (1<<BUTTON);
	while(1){
		if (!(BUTTONPIN & (1<<BUTTON))) {
			LEDPORT&=~(1<<LED);  //zapal diode led
		} else {
			LEDPORT|=(1<<LED); //zgas diode led
		}

	
	
		LEDPORT ^= (1<<BLINK_LED);
		longdelay(500);
	}
	return 0;
}
