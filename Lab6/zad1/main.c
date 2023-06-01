#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "lcd.h"
#include "spi.h"

#define MAX_VAL 20

volatile uint16_t value; //wartosc z przetwornika
volatile uint8_t value_change; //czy mamy nowa wartosc?
uint16_t values[MAX_VAL]; //tablica przechowywujaca osatanie 20 wynikow, bufor kolowy
volatile uint8_t ind=0; //indeks do tablicy wskazujacy na nastepna wartosc
volatile uint8_t full=0; //czy mamy komplet danych w tablicy


ISR(TIMER1_COMPA_vect) {
	uint8_t tmp_val; //tymczasowa zmienna do odbierania danych
	SS_ENABLE();//wybieramy cs - komunikacja z przetwornikiem
	SPI_MasterTransmit(0x01); //Start
	tmp_val = SPI_MasterTransmit(0xa0); //SGL=1, MSBF=1, reszta=0
	value = ((tmp_val & 0x0f)<<8);
	tmp_val = SPI_MasterTransmit(0x00);
	value |= tmp_val;
	SS_DISABLE();
	ind%=MAX_VAL; //reszta z dzielenia, iterator
	values[ind++] = value;
	if(ind==MAX_VAL) full=1;
	value_change=1;
}
int main(void){
	double voltage;
	double voltage_mean = 0;
	
	lcdinit();
	lcd_write_text_P(PSTR("SPI A/D conv 2.1"));
	lcd_set_xy(0,1);
	lcd_write_text_P(PSTR("PSM.LABE.WF.PW"));
	SPI_MasterInit();
	_delay_ms(1000);
	TCCR1B = (1<<WGM12) | (1<<CS11) | (1<<CS10);
	OCR1A=14999;
	TIMSK=(1<<OCIE1A);
	sei();
	
	
	while(1) {
		if (value_change) {
			voltage = 5*value/4096.0;
			value_change=0;
			
			lcd_set_xy(0,0);
			printf("%d: %.3f V     ", value, voltage);
			voltage = 0;
			
			for (register uint8_t i = 0; i < MAX_VAL; i++) {
				voltage += 5*values[i]/4096.0;
			}
			if(full) 
				voltage_mean = voltage/MAX_VAL;
			else
				voltage_mean = voltage/ind;
			lcd_set_xy(0,1);
			printf("Mean: %.3f V     ", voltage_mean);
		}
	}
	
	return 0;
}
