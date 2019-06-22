#ifndef _ODOMETRIE_H_
#define _ODOMETRIE_H_

#include <SYSTEM/include.h>
#include <DRIVE/encoder.h>


//*********************************<Odometrie>************************************
//SCHÄTZUNG
#define enc_to_grad       0.29
#define enc_to_m          0.001
#define pi_div_180        0.017453
//SCHÄTZUNG
struct sOdometrie {
  float X,Y,A;
};

volatile struct sOdometrie odo;

float odo_point(float *X, float *Y);
void odo_timer_low(void);

#endif
