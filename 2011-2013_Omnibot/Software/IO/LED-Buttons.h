#ifndef _LED_BUTTONS_H_
#define _LED_BUTTONS_H_

#include <SYSTEM/include.h>

//*********************************<Buttons>************************************
#define BUTTONS_ALL   (0x60)
#define BUTTONS_STOP  (0x20)
#define BUTTONS_THE   (0x40)

int8_t buttons_get(uint8_t mask);

//*********************************<Leds>***************************************
#define LEDS_ALL  (0x90)
#define LEDS_STOP (0x10)
#define LEDS_THE  (0x80)

int8_t leds_get(uint8_t mask);

void leds_set(uint8_t mask);

void leds_clear(uint8_t mask);

void leds_toggle(uint8_t mask);

#endif
