#include <Zeit.h>

//*********************************<Zeit>***************************************
uint16_t time_get(void) {
  uint8_t mSREG = SREG;

  cli();

  uint16_t erg = time.ticks;

  SREG = mSREG;

  return erg;
}

void time_set(uint16_t ticks) {
  uint8_t mSREG = SREG;
  cli();

  time.ticks = ticks;

  SREG = mSREG;
}

uint8_t time_get_min(uint16_t ticks) {
  return (ticks / 6000);
}

uint8_t time_get_sec(uint16_t ticks) {
  return ((ticks / 100) % 60);
}

uint16_t time_get_milli(uint16_t ticks) {
  return ((ticks % 100) * 10);
 }
