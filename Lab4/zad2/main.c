#include <avr/io.h>
#include <avr/interrupt.h>


#define SWITCH_PORT PORTC // rejestr port switcha
#define SWITCH_PORT_DIR DDRC // rejestr kierunku switcha
#define SWITCH_PORT_INPUT PINC // rejestr wejsciowy switcha
#define SWITCH PC6 // switch podlaczony do PD6

#define LED7SEG_DIGIT PORTC
#define LED7SEG_DIGIT_DIR DDRC
#define LED_SEGMENT PORTD
#define LED_SEGMENT_DIR DDRD
#define LED7SEG_SEGMENT PORTD
#define LED7SEG_SEGMENT_DIR DDRD

#define LED_PORT_DIR DDRA
#define LED_PORT PORTA
#define LED PA0
#define BUZZER PA1

volatile uint16_t liczba = 60;
volatile uint8_t t, s, d, j;


ISR(TIMER1_COMPA_vect) {
	if(!( SWITCH_PORT_INPUT & ( 1 << SWITCH) )) {
		liczba++;
	} else {
		liczba--;
	}
}

ISR(TIMER0_COMP_vect) {
	static uint8_t digit = 0; //statyczna zmienna pamieta
	LED7SEG_DIGIT |= 0x0f; //zgaszenie wszystkich cyfr
	switch(digit) {
		case 0: LED7SEG_SEGMENT=j; break;
		case 1: LED7SEG_SEGMENT=d; break;
		case 2: LED7SEG_SEGMENT=s; break;
		case 3: LED7SEG_SEGMENT=t; break;
	}
	LED7SEG_DIGIT &= ~(1<<digit); //wlaczamy kolejna cyfre
	digit++;
	digit %= 4; //aby zapetlaly sie 0-3
	
}

void led7seg(uint16_t liczba) {
	uint8_t digits_7seg[] = { 	~0x3f,//0
								~0x06, //1
								~0x5b, //2
								~0x4f, //3
								~0x66, //4
								~0x6d, //5
								~0x7d, //6
								~0x07, //7
								~0x7f, //8
								~0x6f, //9
	};
	j=digits_7seg[liczba % 10];
	liczba /= 10;
	d=digits_7seg[liczba % 10];
	liczba /= 10;
	s=digits_7seg[liczba % 10];
	liczba /= 10;
	t=digits_7seg[liczba % 10];
}


int main(void){
	LED_PORT_DIR |= (1 << LED) | (1 << BUZZER);
	LED_PORT = (1 << LED);
	
	LED7SEG_DIGIT_DIR = 0x0f;
	LED7SEG_DIGIT = 0x0f;
	
	SWITCH_PORT_DIR &= ~(1 << SWITCH);
	SWITCH_PORT |= (1 << SWITCH);
	
	LED_SEGMENT_DIR = 0xff;
	LED_SEGMENT = 0xff;
	
	TCCR0 |= (1 << WGM01) | (1 << CS02);
	OCR0 = 124;
	
	TCCR1B |= (1 << WGM12) | (1 << CS12);
	OCR1A = 62499;
	
	TIMSK |= (1 << OCIE0) | (1 << OCIE1A);
	OCR0 = 124;
	sei();
	while(1){
		led7seg(liczba);
		if (liczba == 0) {
			LED_PORT &= ~(1 << LED);
			LED_PORT |= (1 << BUZZER);
			TCCR1B &= ~(1 << CS12);
		}
	}
	
	return 0;
}
