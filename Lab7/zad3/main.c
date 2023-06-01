#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rs232.h"

#define SERVO_DDR DDRD
#define SERVO PD5


#define WAVE 39999
#define MID 2999
#define MIN 999
#define MAX 4999
#define COEF (MAX-MIN)/90.0

#define BAUD 1000000



int main(void){
	uint16_t servo = MID;
	double angle = 0;
	SERVO_DDR |= (1<<SERVO);
	char text[16];
	//Timer
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	TCCR1A |= (1<<COM1A1);
	TCCR1B |= (1<<CS11);
	
	ICR1 = WAVE;
	
	USART_init();
	sei();
	
	OCR1A = MIN;
	_delay_ms(2000);
	
	while(1){
		
		if(decode) {
			if (strncmp((void* )bufor, "angle", 5) == 0) {
				angle=atof((const char *)bufor+6);
				if (angle > 90.0) angle = 90;
				if (angle < -90.0) angle = -90;
				servo=(uint16_t)(MID+angle*COEF);
				sprintf(text, "\r\nGET: %d\r\n", servo);
				USART_send(text);
				OCR1A=servo;
			}
			else USART_send("\r\nUnknown command\r\n");
			decode = 0;
			indeks = 0;
			memset((void *)bufor, 0, MAX_BUF);
		}
	}
	
	return 0;
}
