#ifndef _LCD_H
#define _LCD_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdio.h>

#ifndef LCD_PORT
#define LCD_PORT PORTA
#endif

#ifndef LCD_PORT_DIR
#define LCD_PORT_DIR DDRA
#endif

#ifndef LCD_E
#define LCD_E PA3
#endif

#ifndef LCD_RS
#define LCD_RS PA2
#endif

void lcdinit(void); //inicjalizacja wyswietlacza

void lcd_write_command(uint8_t data); //funkcja wysyłająca pojedynczą instrukcję do wyświetlacza

void lcd_clear(void); //funkcja realizująca instrukcję "czyść wyświetlacz"

void lcd_write_data(uint8_t data); //funkcja wysyłająca pojedynczą daną do wyświetlacza

void lcd_set_xy(uint8_t x, uint8_t y); //funkcja ustawiająca kursor na pozycji xy

void lcd_write_text_xy(uint8_t x, uint8_t y, char *text); //funkcja wyświetlająca tekst znajdujący sie w w pamięci danych(!!!) począwszy od pozycji xy.

void lcd_write_text_P(const char *text);

#endif
