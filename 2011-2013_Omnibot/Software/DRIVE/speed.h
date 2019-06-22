#ifndef _SPEED_H_
#define _SPEED_H_

#include <SYSTEM/include.h>
#include <DRIVE/encoder.h>
#include <DRIVE/motors.h>

//*********************************<Speed>************************************
#define MOTORS_SPEED_INIT_MAX 100
#define PID_P 0.1
#define PID_I 0.01
#define PID_D 0.00

struct sSpeed {
  int8_t  sl, sr, sh; // Supposed motorspeed in percent (scaled by max_speed)
  int8_t max;
  int8_t max_nr;
};
volatile struct sSpeed Speed;
struct sPID {
  float  iel,ier,ieh; // Supposed motorspeed in percent (scaled by max_speed)
};
volatile struct sPID PID;

void motors_speed_PID_reset(void);
void motors_speed_stop(void);
void motors_speed_set(int8_t sl, int8_t sr, int8_t sh);
//void motor_speed_set(uint8_t nr, int8_t m) ;
int8_t motor_speed_get(uint8_t nr);
void motors_speed_set_max(uint8_t max);
uint8_t motors_speed_get_max(void);
void motors_speed_timer_low(void);

//*********************************<Drive>************************************
void motors_vector(float X, float Y);


#endif
