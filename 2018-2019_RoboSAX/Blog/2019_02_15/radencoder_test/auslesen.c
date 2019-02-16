#include "keplerboard.h"

void uart_1_init(void) {
    SREG  &= ~_BV(7);    //disable interrupts
    UCSR1A = 0;
    UCSR1B = 0b10011000;
    UCSR1C = 0b00001110;
    UBRR1H = 0;
    UBRR1L = 16;
    SREG  |= _BV(7);     //enable interrupts
}

void uart_1_send_blocking(uint8_t c) {
    while(!(UCSR1A & _BV(5)));
    UDR1 = c;
}

void send_int(int data) {
    if(data < 0) {
        uart_1_send_blocking('-');
        send_int(-data);
        return;
    } else if(data >= 10) {
        send_int(data / 10);
    }
    uart_1_send_blocking('0' + data % 10);
}

volatile int pos_left;
volatile int pos_right;

void pos_init(void) {
    pos_left  = 0;
    pos_right = 0;
    DDRE  &= ~0x0f;
    SREG  &= ~_BV(7);    //disable interrupts
    EIMSK |=  _BV(4) | _BV(6);
    EICRB  = (EICRB & ~(_BV(1) | _BV(5))) | _BV(0) | _BV(4);
    SREG  |= _BV(7);     //enable interrupts
}

ISR(INT4_vect) {
    uint8_t pine = PINE;
    if(pine & _BV(4))
        PORTC |= _BV(1);
    else
        PORTC &= ~_BV(1);
    if(pine & _BV(5))
        PORTC |= _BV(5);
    else
        PORTC &= ~_BV(5);


    if (pine & _BV(4)) {
        if (pine & _BV(5))
            pos_left--;
        else
            pos_left++;
    } else {
        if (pine & _BV(5))
            pos_left++;
        else
            pos_left--;
    }
}

ISR(INT6_vect) {
    uint8_t pine = PINE;
    if (pine & _BV(6)) {
        if (pine & _BV(7))
            pos_left--;
        else
            pos_left++;
    } else {
        if (pine & _BV(7))
            pos_left++;
        else
            pos_left--;
    }
}

int main(void) {
    uart_1_init();
    pos_init();
    DDRC  = _BV(1) | _BV(3) | _BV(5);
    PORTC = _BV(0) | _BV(2) | _BV(3) | _BV(4);
    while(1) {
        while(!(PINC & _BV(2)));
        send_int(pos_left);
        uart_1_send_blocking(' ');
        send_int(pos_right);
        uart_1_send_blocking('\n');
        while(PINC & _BV(2));
    }
}
