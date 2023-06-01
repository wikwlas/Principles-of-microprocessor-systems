#include <avr/io.h>
#include <avr/interrupt.h>

#define INT0_pin PD2
volatile uint8_t count = 0;

ISR (INT0_vect) {
	count ++;
}

int main(void){
	
	DDRC = 0xff; //kierunek wyjsciowy dla diod LED
	PORTC = 0xff; //diody wylaczone
	
	DDRD &=~(1<<INT0_pin); //kierunek wejsciowy na INT0_pin
	PORTD |= (1<<INT0_pin); //wlaczamy rezystor podciagajacy (Pull-up) na INT0_pin
	
	
	MCUCR |= (1<<ISC01);
	GICR |= (1<<INT0);
	
	sei();
	while(1){
		PORTC = ~count;
	}
	
	return 0;
}
