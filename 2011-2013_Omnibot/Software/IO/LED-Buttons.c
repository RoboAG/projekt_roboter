#include <LED-Buttons.h>

//*********************************<Buttons>************************************

int8_t buttons_get(uint8_t mask) {
  return ((~PIND) & BUTTONS_ALL) & mask;
}


//*********************************<Leds>***************************************

int8_t leds_get(uint8_t mask) {
  return ((PORTD) & LEDS_ALL) & mask;
}

void leds_set(uint8_t mask) {
  PORTD|= mask & LEDS_ALL;
}

void leds_clear(uint8_t mask) {
  PORTD&= ~(mask &  LEDS_ALL);
}

void leds_toggle(uint8_t mask) {
  PORTD^= mask & LEDS_ALL;
}


