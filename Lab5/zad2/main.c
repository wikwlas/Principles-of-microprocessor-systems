#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include "rs232.h"

#define LED_PORT PORTC
#define LED_PORT_DIR DDRC
#define SWITCH_DIR DDRC
#define SWITCH_PORT PORTC
#define SWITCH_PORT_INPUT PINC
#define SWITCH PC1
#define LED PC0

volatile uint8_t led_status=0;
volatile uint8_t led_status_changed=0;

ISR(TIMER1_COMPA_vect) {
	if(!SWITCH_PORT_INPUT & (1<<SWITCH)) {
		led_status_changed = 1;
		led_status ^= 1;
	}
}

int main(void){
	LED_PORT_DIR = (1 << LED);
	LED_PORT = (1 << LED);
	
	SWITCH_DIR &= ~(1 << SWITCH);
	SWITCH_PORT |= (1 << SWITCH);
	
	USART_init();
	
	TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);
	OCR1A = 24999;
	
	TIMSK = (1<<OCIE1A);
	sei();
	
	USART_send("Witaj\r\n");
	
	while(1) {
		if(led_status_changed) {
			led_status_changed=0;
			if(led_status){
				LED_PORT &= ~(1 << LED);
				USART_send("LED:1\r\n");
			} else {
				LED_PORT |= (1 << LED);
				USART_send("LED:0\r\n");
			}
		}
		if(decode) {
			if(strncmp((void *)bufor, "help", 4)==0) {
				ack(0);
				USART_send("Available commands:\r\n");
				USART_send("help - this list\r\n");
				USART_send("on - enable led\r\n");
				USART_send("off - disable led\r\n");
			}
			else if(strncmp((void *)bufor, "off", 3)==0) {
				led_status=0;
				led_status_changed=1;
				ack(0);
			}
			else if (strncmp((void *)bufor, "on", 2)==0) {
				led_status=1;
				led_status_changed=1;
				ack(0);
			} else {
				ack(-1);
				USART_send("Unknown command: \r\n");
				USART_send((void *)bufor);
				USART_send("\r\n");
			}
			
			reset_buffer();
		}
	}
	return 0;
}
