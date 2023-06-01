#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcd.h"
#include "iwire.h"
#include "ds18b20.h"
#include "ff.h"
#include "rs232.h"

#define MAX_BUF 80

#define SWITCH PD7
#define SWITCH_DIR DDRD
#define SWITCH_PIN PIND
#define SWITCH_PORT PORTD

FATFS fs;
FIL fil;
FRESULT fresult;

char buf[MAX_BUF];
volatile double temp;

char prefix[]="data";
char suffix[]=".txt";

ISR(TIMER1_COMPA_vect)
{
	int16_t temperature;
	static uint32_t num=0;
	uint16_t bytes;
	uint16_t slen;
	
	DS18B20_start();
	while(!OW_check);
	temperature = DS18B20_read();
	temp = ((double) temperature)*0.0625;
	
	lcd_set_xy(0,0);
	printf("T=%5.1f \337C", temp);
	
	num++;
	sprintf(buf, "%ld \t %5.1f \r\n", num, temp);
	slen= strlen(buf);
	fresult = f_write(&fil, buf, slen, &bytes);
	if(fresult!=FR_OK) stop("Error writing file");
	if (bytes!=slen) stop("Cannot write complete data");
	f_sync(&fil);
}

void stop(char *s)
{
	lcd_clear();
	lcd_set_xy(0,0);
	printf("STOP!");
	lcd_set_xy(0,1);
	printf("%s",s);
	while(1);
}

int main(void)
{
	uint16_t num2=0;
	uint8_t position=0;
	
	SWITCH_DIR&= ~(1<<SWITCH);
	SWITCH_PORT|=(1<<SWITCH);
	
	lcdinit();
	printf("Logger temp");
	
	DS18B20_init();
	_delay_ms(1000);
	
	DS18B20_start();
	while (!OW_check);
	DS18B20_read();
	
	lcd_clear();
	lcd_set_xy(0,0);
	
	fresult = f_mount(&fs, "", 0);
	if(fresult!=FR_OK) stop("mount error");
	else printf ("mounted");
	_delay_ms(1000);
	
	lcd_clear();
	lcd_set_xy(0,0);
	
	while(1)
	{
		strcpy(buf, prefix);
		position = strlen(buf);
		sprintf(&buf[position], "%d", num2);
		position= strlen(buf);
		strcpy(&buf[position], suffix);
		num2++;
		
		if(f_stat(buf, NULL)==FR_NO_FILE)
		{
			if(f_open(&fil, buf, FA_WRITE | FA_CREATE_ALWAYS)==FR_OK) 
			{
				printf("file created");
				lcd_set_xy(0,1);
				printf("%s",buf);
				break;
			}
			else stop ("Error creat. file");
			
		}
		
	}
	
	_delay_ms(1000);
	
	lcd_clear();
	_delay_ms(1000);
	
	cli();
	TIMSK |= (1<<OCIE1A);
	OCR1A=62499;
	TCCR1B = (1<<WGM12) | (1<<CS12);
	sei();
	while(1)
	{
		if(!(SWITCH_PIN & (1<<SWITCH)))
		{
			TIMSK&=!(1<<OCIE1A);
			TCCR1B=0;
			_delay_ms(1000);
			f_close(&fil);
			stop("OK File closed");
		}
	}
	return 0;
}
