#ifndef _MOTORS_H_
#define _MOTORS_H_

#include <SYSTEM/include.h>
#include <DRIVE/speed.h>//wegen motors_stop

//*********************************<Motoren>************************************
#define MOTORS_LOW_STEP 5
#define MOTORS_INIT_MAX 50

#define motors_ddr_h(x) (x ? (PORTB|=  _BV(4)) : (PORTB&= ~_BV(4)))
#define motors_ddr_r(x) (x ? (PORTB&= ~_BV(3)) : (PORTB|=  _BV(3)))
#define motors_ddr_l(x) (x ? (PORTB|=  _BV(2)) : (PORTB&= ~_BV(2)))

struct sMotors {
  int8_t  ml_s, mr_s, mh_s; // Supposed motorspeed in percent (scaled by max_speed)
  int16_t ml_c, mr_c, mh_c; // Current  motorspeed in abs values
  uint8_t max;              // maximum  motorspeed in abs value
};
volatile struct sMotors motors;

void motors_stop(void);
void motors_set(int8_t ml, int8_t mr, int8_t mh);
void motor_set(uint8_t nr, int8_t m);
int8_t motor_get(uint8_t nr);
void motors_set_max(uint8_t max);
uint8_t motors_get_max(void);
void motors_timer_low(void);


#endif
