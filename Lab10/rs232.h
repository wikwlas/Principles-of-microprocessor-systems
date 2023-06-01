#ifndef _RS32_H_
#define _RS32_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define BAUD 1000000
#define MY_UBRR (( F_CPU / (16* BAUD))-1)
#define MAX_BUF 80


extern FILE *serout;
extern uint8_t bufor[MAX_BUF];
extern uint8_t buf_OUT[MAX_BUF];
extern volatile uint8_t decode;
extern volatile uint8_t transmission;

void USART_init(void);
void USART_send(char* tekst);
void ack(uint8_t status);
void send_double(double d);
void reset_buffer(void);

#endif
