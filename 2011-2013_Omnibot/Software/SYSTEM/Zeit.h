#ifndef _ZEIT_H_
#define _ZEIT_H_

#include <SYSTEM/include.h>
#include <DRIVE/motors.h>

//*********************************<Zeit>***************************************
struct sTime {
  uint16_t ticks;
};

volatile struct sTime time;

uint16_t time_get(void);
void time_set(uint16_t ticks);
uint8_t time_get_min(uint16_t ticks);
uint8_t time_get_sec(uint16_t ticks) ;
uint16_t time_get_milli(uint16_t ticks) ;


#endif
