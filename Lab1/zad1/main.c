#include <avr/io.h>
#include <utils/delay.h>

#define BUTTON PD0
#define BUTTONDIR DDRD
#define BUTTONPULLUP PORTD
#define BUTTONPIN PIND
#define LED PC7
#define LEDDIR DDRC
#define LEDPORT PORTC

int main(void){
	
	// do zadania 1
	/*DDRC = 0xff; //225 //(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4)|(1<<5) //0b11111111 //ustawiamy na wyjscie
	PORTC = 0x55;*/
	
	
	//do zadania 2
	LEDDIR = (1<<LED); //pin na wyjscie
	LEDPORT = (1<<LED); //pin na stan wysoki
	
	BUTTONDIR &=~(1<<BUTTON); //ustawiamy 0 na miejscu button zeby to bylo wejsciem
	BUTTONPULLUP |= (1<<BUTTON); //zawsze soe pisze
	while(1){
		if (!(BUTTONPIN & (1<<BUTTON))) {
			LEDPORT&=~(1<<LED);  //zapal diode led
		} else {
			LEDPORT|=(1<<LED); //zgas diode led
		}
	}
	
	return 0;
}
