#include <Bumpers.h>



//*********************************<Bumpers>************************************
int8_t bumpers_get(uint8_t mask) {
  return ((~PINC) & 0xFE) & mask;
}


