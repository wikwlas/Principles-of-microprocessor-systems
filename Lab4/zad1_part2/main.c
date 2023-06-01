#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PORT PORTC // port wyjsciowy do zapalania diody 
#define LED_PORT_DIR DDRC // rejestr kierunku potu diody
#define SWITCH_PORT PORTD // rejestr port switcha
#define SWITCH_PORT_DIR DDRD // rejestr kierunku switcha
#define SWITCH_PORT_INPUT PIND // rejestr wejsciowy switcha
#define LED PC0 // dioda LED podlaczona do PC0
#define SWITCH PD6 // switch podlaczony do PD6

volatile uint8_t status_led = 0; //stan wlaczenia led

ISR(TIMER1_COMPA_vect) {
	if(!( SWITCH_PORT_INPUT & ( 1 << SWITCH) )) {
		status_led ^= 1;
	}
}
int main(void){
	LED_PORT_DIR = 1 << LED; // stan wyjsiowy na diodzie led
	LED_PORT = 1 << LED; // wylaczenie swiecenia led
	
	SWITCH_PORT_DIR = 0; // stan wejsciowy na porcie switch
	SWITCH_PORT = 1 << SWITCH; //wlaczamy rezystor Pull-up
	
	TCCR1A = 0; // stan domyslny timera tryb NORMAl
	TIMSK = 1 << OCIE1A; //wlaczamy przerwanie compare match
	
	OCR1A = 24999; //wartosc porownawcza, zliczenie do wartosci +1 wygeneruje przerwanie compare-match
	TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // wlaczanie licznika poprzez wybor preskalera u nas 64
	sei(); // wlaczanie globalnej flagi przerwan
	
	
	while(1){
		if(status_led) LED_PORT &= ~(1 << LED); // zapalenie diody led
		else LED_PORT |= (1 << LED); // zgaszanie diody led
	}
	
	return 0;
}
