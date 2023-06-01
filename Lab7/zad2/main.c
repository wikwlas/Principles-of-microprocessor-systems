#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_DDR DDRD
#define SERVO PD5

#define LED_DDR DDRB
#define LED PB3

#define WAVE 39999
#define MID 2999
#define MIN 999
#define MAX 4999

ISR(TIMER2_COMP_vect) {
	static uint8_t intensity = 1;
	static uint8_t dir = 0;
	if ((intensity==255) || (intensity==0))
		dir^=1;
	
	OCR0=intensity;
	
	if(dir) intensity--;
	else intensity++;
}
int main(void){
	
	SERVO_DDR |= (1<<SERVO);
	LED_DDR |= (1<<LED);
	//Timer
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<CS11);
	
	ICR1 = WAVE;
	TCCR0 |= (1<<WGM00)|(1<<WGM01);
	TCCR0 |= (1<<COM01)|(1<<COM00);
	TCCR0 |= (1<<CS01);
	OCR0 = 0;
	
	
	TCCR2 |= (1<<WGM21);
	TCCR2=(1<<CS20)|(1<<CS21)|(1<<CS22);
	TIMSK |= (1<<OCIE2);
	OCR2 = 249;
	
	sei();
	
	OCR1A = MIN;
	_delay_ms(2000);
	
	uint16_t i = MID;
	while(1){
		for (i = MIN; i < MAX; i++) {
			OCR1A = i;
			_delay_ms(1);
		}
		for (i = MAX; i > MIN; i--) {
			OCR1A = i;
			_delay_ms(1);
		}
	}
	
	return 0;
}
