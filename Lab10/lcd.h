#ifndef __LCD_H
#define __LCD_H

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/pgmspace.h>

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

void lcd_write_instr(uint8_t data); //funkcja wysyłająca pojedynczą instrukcję do wyświetlacza↪→//WRITE_COMMAND

void lcd_clear(void); //funkcja realizująca instrukcję "czyść wyświetlacz"

void lcd_write_data(uint8_t data); //funkcja wysyłająca pojedynczą daną dowyświetlacza↪→

void lcd_set_xy(uint8_t x, uint8_t y); //funkcja ustawiająca kursor napozycji xy↪→

void lcd_write_text_xy(uint8_t x, uint8_t y, char *text); //funkcjawyświetlająca tekst znajdujący sie w w pamięci danych(!!!) począwszy odpozycji xy

void lcd_write_text_P(const char *text); 
#endif

