#include "spi.h"


void SPI_MasterInit(void) {
	DDRB|=(1<<SS)|(1<<MOSI)|(1<<SCK);
	PORTB|=(1<<SS);
	DDRB&=~(MISO);
	SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

uint8_t SPI_MasterTransmit(uint8_t data) {
	SPDR = data;
	while(! (SPSR & (1<<SPIF))); //SPIF=1 transmisja zakonczona
	return SPDR;
}
