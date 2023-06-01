#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

//rzeczy do printfa
static int lcd_print(char c, FILE *stream);

static FILE mystdout= FDEV_SETUP_STREAM(lcd_print, NULL, _FDEV_SETUP_WRITE);

static int lcd_print(char c, FILE *stream)
{
	lcd_write_data(c);
	return 0;
}

void lcdinit(void)
{
	LCD_PORT_DIR=0xff; //ustawiamy linie portu wyswietlacza na wyjsciowy
	LCD_PORT=0x00; //ustawiamy stan niski na liniach portu wyswietlacza
	_delay_ms(50); //czekamy 50 ms 
	
	stdout=&mystdout;//przypisanie strumienia WEJSCIOWEGO  do zmiennej
	
	LCD_PORT &=~(1<<LCD_RS);//wybieramy rejestr instrukcji IR wpisanie zera na rejestr RS
	
	for(uint8_t i=0; i<3; i++)
	{
		LCD_PORT|= 1<<LCD_E;
		LCD_PORT = (LCD_PORT & 0x0f) | 0x30;
		_delay_us(1);
		LCD_PORT &= ~(1<<LCD_E);
		_delay_ms(5);
		
	}
	
	LCD_PORT|= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | 0x20;
		_delay_us(1);
		LCD_PORT &= ~(1<<LCD_E);
		_delay_us(100);
	
	lcd_write_instr(0x28); //ustawienie dwojki 
	
	lcd_write_instr(0x0c); //włacz lcd i wylacz wyswietlanie kursora
	
	lcd_write_instr(0x06);//włacz pisanie od lewej do prawej i inkrementacja adresu pamieci
	lcd_clear(); //zerowanie wyswietlacza	
	
}
	
void lcd_write_instr(uint8_t data)
{
	LCD_PORT &= ~(1<<LCD_RS); //wybieramy rejestr
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | (data & 0xf0);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | ((data & 0x0f)<<4);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);	
}
	

	void lcd_clear(void)
	{
		lcd_write_instr(0x01);
		_delay_ms(5);
	}
	

void lcd_write_data(uint8_t data)
{
	LCD_PORT |= (1<<LCD_RS); //wybieramy rejestr
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | (data & 0xf0);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);
	LCD_PORT |= 1<<LCD_E;
	LCD_PORT = (LCD_PORT & 0x0f) | ((data & 0x0f)<<4);
	_delay_us(1);
	LCD_PORT &= ~(1<<LCD_E);
	_delay_us(100);	
}

void lcd_set_xy(uint8_t x, uint8_t y )
{
	lcd_write_instr(0x80 | (0x40 * y +x)); //instrukcja do ustawienia adresu pamieci wyswietlacza
	//linia 1 jest od adresu 0x40
	//to mamy adres 0x40 * y +x
}

void lcd_write_text_xy(uint8_t x, uint8_t y, char *text)
{
	lcd_set_xy(x,y);
	while (*text!=0) //sprawdz czy znak rozny od zera jesli tak to 
	{
		lcd_write_data((uint8_t)*text); //znak wysłany na wyswietlacz
		text++; //na nastepny znak
	}
}

void lcd_write_text_P(const char *text)
{
	register uint8_t ch; //sugestia wyjorztstywania wewnetrznego rejestru do przechowywania 
	while ((ch= pgm_read_byte(text++))) //odczyt 
	{
		lcd_write_data(ch);
	}
}

