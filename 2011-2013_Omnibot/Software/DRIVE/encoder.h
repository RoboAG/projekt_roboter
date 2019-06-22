#ifndef _ENCODER_H_
#define _ENCODER_H_

#include <SYSTEM/include.h>


//*********************************<Encoder>************************************
#define ENCODER_ABRUF_HZ 10
struct sEncoders {
  int16_t abs_l, abs_r, abs_h;
  int16_t tmp_l, tmp_r, tmp_h;
  int16_t vel_l, vel_r, vel_h;
};

volatile struct sEncoders encoders;

int16_t encoder_get(uint8_t nr);
int16_t encoder_get_speed(uint8_t nr);
float encoder_vector(float *X, float *Y);
void encoders_reset(void);
void encoders_timer_low(void);

#endif
