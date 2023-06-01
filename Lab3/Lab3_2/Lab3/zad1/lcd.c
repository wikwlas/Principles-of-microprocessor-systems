#include "lcd.h"

static int lcd_print(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(lcd_print, NULL, _FDEV_SETUP_WRITE);

static int lcd_print(char c, FILE *stream) {
    lcd_write_data(c);
    return 0;
}

void lcdinit(void) {
    LCD_PORT_DIR = 0xff; //ustawiamy linie portu wyswietlacza na wyjsciowe
    LCD_PORT = 0x00; //ustawiamy stan niski na liniach portu wyswietlacza
    _delay_ms(50);

    stdout = &mystdout; //przypisujemy strumien wyjsciowy stdout

    LCD_PORT &= ~(1<<LCD_RS); //wybieramy rejestr instrukcji IR
    for (uint8_t i = 0; i < 3; i++) {
        LCD_PORT |= 1<<LCD_E;
        LCD_PORT = (LCD_PORT & 0x0f) | 0x30;
         _delay_us(1);
        LCD_PORT &= ~(1<<LCD_E);
        _delay_ms(5);
    }

    LCD_PORT |= 1<<LCD_E;
    LCD_PORT = (LCD_PORT & 0x0f) | 0x20;
    _delay_us(1);
    LCD_PORT &= ~(1<<LCD_E);
    _delay_us(100);
    
    lcd_write_command(0x28);
    lcd_write_command(0x0c);
    lcd_write_command(0x06);

    lcd_clear();

}

void lcd_write_command(uint8_t data) {
    LCD_PORT &= ~(1<<LCD_RS);
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


void lcd_clear() {
    lcd_write_command(1);
    _delay_ms(2);
}

void lcd_write_data(uint8_t data) {
    LCD_PORT |= (1<<LCD_RS);
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

void lcd_set_xy(uint8_t x, uint8_t y) {
    lcd_write_command(0x80 | (0x40*y + x)); //instrukcja do utawienia adresu pamieci wyswietlacza linia "1" jets od adresu 0x40 wiec mamy adres 0x40*y+x
}

void lcd_write_text_xy(uint8_t x, uint8_t y, char *text) {
    lcd_set_xy(x, y);
    while(*text != 0) { 
		//sprawdz czy znak rozny od 0, jesli tak to wyslij znak na wyswietlacz, zwieksz wskaznik o 1 - na nastepny znak
        lcd_write_data((uint8_t)*text);
        text++;
    }
}

void lcd_write_text_P(const char *text) {
	register uint8_t ch;
	while( (ch = pgm_read_byte(text++) )) {
		lcd_write_data(ch);
	}
}
