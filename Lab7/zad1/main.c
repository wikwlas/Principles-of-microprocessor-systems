#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define SERVO_DDR DDRD
#define SERVO PD5

#define WAVE 39999
#define MID 2999
#define MIN 1799
#define MAX 4199

int main(void){
	
	SERVO_DDR |= (1<<SERVO);
	
	//Timer
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<CS11);
	
	ICR1 = WAVE;
	
	OCR1A = MID;
	_delay_ms(2000);
	

	while(1){
		OCR1A = MIN;
		_delay_ms(1000);
		OCR1A = MID;
		_delay_ms(1000);
		OCR1A = MAX;
		_delay_ms(1000);
		OCR1A = MID;
		_delay_ms(1000);
	}
	
	return 0;
}
