#include "rs232.h"

uint8_t bufor[MAX_BUF];
uint8_t buf_out[MAX_BUF];

volatile uint8_t indeks;
volatile uint8_t *psend;
volatile uint8_t decode=0;
volatile uint8_t transmission=0;

static int rs232_print(char c, FILE *stream);

static FILE ser = FDEV_SETUP_STREAM(rs232_print, NULL, _FDEV_SETUP_WRITE);

FILE *serout = &ser;

static int rs232_print(char c, FILE *stream) {
	while(transmission);
	if (c == '\n')
		rs232_print('\r', stream);
	while(!(UCSRA&(1<<UDRE)));
	UDR=c;
	return 0;
}

ISR(USART_RXC_vect) {
	uint8_t znak;
	znak = UDR;
	if (indeks > (MAX_BUF-2))
		indeks=0;
	if (decode == 0){
		if ((znak=='\r') || (znak=='\n')) {
			if (indeks>0){
				bufor[indeks]=0;
				indeks=0;
				decode=1;
			}
		}
		else {
			bufor[indeks]=znak;
			indeks++;
		}
	}
}

ISR(USART_UDRE_vect) {
	if(*psend) {
		UDR =*(psend++);
	} else {
		UCSRB &= ~(1<<UDRIE);
		transmission=0;
	}
}

void USART_init(void) {
	UBRRH = (uint8_t) (MY_UBRR >> 8);
	UBRRL = (uint8_t) (MY_UBRR & 0xff);
	UCSRB = (1<<RXCIE) | (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void USART_send(char *tekst) {
	while(transmission);
	transmission=1;
	psend=(uint8_t *)tekst;
	UCSRB |= (1<<UDRIE);
}

void ack(uint8_t status) {
	if (status = 0)
		USART_send("\r\nOK\r\n");
	else
		USART_send("\r\nERROR\r\n");
}

void send_double(double d) {
	sprintf((char*)buf_out, "%g\r\n", d);
	USART_send((char*)buf_out);
}

void reset_buffer(void) {
	memset((void *)bufor, 0, MAX_BUF);
	indeks=0;
	decode=0;
}
