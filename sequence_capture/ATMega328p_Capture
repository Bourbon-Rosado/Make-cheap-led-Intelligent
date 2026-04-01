#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/16/BAUD)-1)

volatile bool flag = false;

void UART_init() {
    // Configurar baud rate
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    UBRR0L = (unsigned char)(UBRR_VALUE & 0xFF);

    // Habilitar transmisión
    UCSR0B = (1 << TXEN0);

    // Formato: 8 bits, 1 stop
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_sendChar() {
    while (!(UCSR0A & (1 << UDRE0))); // Esperar buffer vacío
    UDR0 = '\n';
}

ISR(INT0_vect){
  flag = true;
}

int main(void){

  UART_init();

  DDRB |= (1<<5);
  DDRD &= ~(0x04);
  
  EICRA |= (0x02);
  EIMSK |= (1<<INT0);

  sei();

  while(true){
    if(flag){
    flag = false;
    UART_sendChar();
    }
  }
}
