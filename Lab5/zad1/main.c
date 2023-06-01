#include <avr/io.h>
#include <avr/interrupt.h>

#define BAUD 1000000

#define MY_UBRR (( F_CPU / (16 * BAUD))-1 )


volatile uint8_t znak = 0;

volatile char *pbuf;

ISR(USART_RXC_vect) {
	znak = UDR;
}

ISR(USART_TXC_vect) {
	if(*pbuf) {
		UDR =*(pbuf++);
	}
}

void USART_init(void) {
	UBRRH = (uint8_t) (MY_UBRR >> 8);
	UBRRL = (uint8_t) (MY_UBRR & 0xff);
	UCSRB = (1<<RXCIE) | (1<<TXCIE) | (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0);
}

void USART_send(char *tekst) {
	UDR=*tekst;
	pbuf= ++tekst;
}

int main(void){
	USART_init(); //inicjalizacja portu szeregowego
	sei(); //wlaczenie przerwan globalnie
	
	USART_send("Hello\r\n");
	
	while(1){
		if (znak=='x') {
			znak=0;
			USART_send("OK\r\n");
		}
	}
	
	return 0;
}
