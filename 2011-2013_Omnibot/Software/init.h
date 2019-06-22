#ifndef _INIT_H_
#define _INIT_H_

#include <SYSTEM/include.h>
#include <DRIVE/encoder.h>
#include <DRIVE/motors.h>
#include <DRIVE/speed.h>
#include <SYSTEM/Zeit.h>

void Init_Hardware(void);
void time_init(void) ;
void motors_init(void);
void encoders_init(void);
void motors_speed_init(void);
void datakom_init(void) ;

#endif
