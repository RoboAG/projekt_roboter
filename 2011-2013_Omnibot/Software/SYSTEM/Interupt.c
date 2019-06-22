#include <Interupt.h>


ISR(INT5_vect){
uint8_t blub=PINE;
   if (((blub)^((blub)>>3))& 0x04)encoders.abs_l++;
    else encoders.abs_l--;
}

ISR(INT6_vect){
uint8_t blub=PINE;
   if (((blub)^((blub)>>3))& 0x08)encoders.abs_h--;
    else encoders.abs_h++;
}

ISR(INT7_vect){
uint8_t blub=PINE;
   if (((blub)^((blub)>>3))& 0x10)encoders.abs_r++;
    else encoders.abs_r--;
}



ISR(TIMER2_COMPA_vect){
  static volatile uint8_t running = 0;
  // Bit 0: Motor   	Low Level
  // Bit 1: Encoder 	Low Level
  // Bit 2: Speed		Low Level
  // Bit 3: Odometrie 	Low Level
  // Bit 7: Errorhandling
  static uint8_t encoders_count = 0;
  static uint8_t odo_count = 0;
  static uint8_t speed_count = 0;

  time.ticks++;

  // Motoren Low-Level
  if ((running & 0x01) == 0x00) {
    running|= 0x01;
    sei();
    motors_timer_low();
    cli();
    running&= ~0x01;
  } else {
//    error.Timer_Overrun_Motors = 1;
  }

  // Encoder Low Level
  encoders_count++;
  if (encoders_count >= 10) {
    if ((running & 0x02) == 0x00) {
      encoders_count = 0;
      running|= 0x02;
      sei();
      encoders_timer_low();
      cli();
      running&= ~0x02;
    } else {
//      error.Timer_Overrun_Encoders = 1;
    }
  }

  // Speed Low Level
  speed_count++;
  if (speed_count >= 10) {
    if ((running & 0x04) == 0x00) {
      speed_count = 0;
      running|= 0x04;
      sei();
      motors_speed_timer_low();
      cli();
      running&= ~0x04;
    } else {
//      error.Timer_Overrun_Speed = 1;
    }
  }

  // Odometrie Low Level
  odo_count++;
  if (odo_count >= 10) {
    if ((running & 0x08) == 0x00) {
      odo_count = 0;
      running|= 0x08;
      sei();
      odo_timer_low();
      cli();
      running&= ~0x08;
    } else {
//      error.Timer_Overrun_Odo = 1;
    }
  }

  // Errorhandling
//  if ((uint16_t) error != 0) {
//    if ((running & 0x80) == 0x00) {
//      error_handling();
//    }
//  }

}

