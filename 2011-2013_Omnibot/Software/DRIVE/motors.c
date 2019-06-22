#include <motors.h>


//*********************************<Motoren>************************************
void motors_stop(void) {

  motors_speed_stop();//'Speed' stoppen

  uint8_t mSREG = SREG;

  cli();

  motors.ml_s = 0;
  motors.mr_s = 0;
  motors.mh_s = 0;

  motors.ml_c = 0;
  motors.mr_c = 0;
  motors.mh_c = 0;

  SREG = mSREG;
}

void motors_set(int8_t ml, int8_t mr, int8_t mh) {
  if (ml >  100) { ml =  100;}
  if (ml < -100) { ml = -100;}
  if (mr >  100) { mr =  100;}
  if (mr < -100) { mr = -100;}
  if (mh >  100) { mh =  100;}
  if (mh < -100) { mh = -100;}

  uint8_t mSREG = SREG;

  cli();

  motors.ml_s = ml;
  motors.mr_s = mr;
  motors.mh_s = mh;

  SREG = mSREG;
}

void motor_set(uint8_t nr, int8_t m) {
  if (m >  100) { m =  100;}
  if (m < -100) { m = -100;}
  if ((nr < 1) || (nr > 3)) { return;}

  if (nr == 1) {
    motors.ml_s = m;
  } else if (nr == 2) {
    motors.mr_s = m;
  } else if (nr == 3) {
    motors.mh_s = m;
  }
}

int8_t motor_get(uint8_t nr) {
  if ((nr < 1) || (nr > 3)) { return 0;}

  if (nr == 1) {
    return motors.ml_s;
  } else if (nr == 2) {
    return motors.mr_s;
  } else if (nr == 3) {
    return motors.mh_s;
  }

  return 0;
}

void motors_set_max(uint8_t max) {
  if (max<0)	{max=0;}
  if (max>100)	{max=100;}
  motors.max = max;
}

uint8_t motors_get_max(void) {
  return motors.max;
}

void motors_timer_low(void) {
  int16_t m_s, m_c;

  m_s = ((int16_t) motors.ml_s * (int16_t) motors.max) / 100;
  m_c = motors.ml_c;
  if (m_s != m_c) {
    if (m_s - m_c > MOTORS_LOW_STEP) {
      m_c+= MOTORS_LOW_STEP;
    } else if (m_c - m_s > MOTORS_LOW_STEP) {
      m_c-= MOTORS_LOW_STEP;
    } else {
      m_c = m_s;
    }
    motors.ml_c = m_c;
  }

  if (m_c >= 0) {
    motors_ddr_l(1);
  } else {
    motors_ddr_l(0);
    m_c = -m_c;
  }
  cli();
  OCR1CH = 0x00;
  OCR1CL = m_c;
  sei();

  m_s = ((int16_t) motors.mr_s * (int16_t) motors.max) / 100;
  m_c = motors.mr_c;
  if (m_s != m_c) {
    if (m_s - m_c > MOTORS_LOW_STEP) {
      m_c+= MOTORS_LOW_STEP;
    } else if (m_c - m_s > MOTORS_LOW_STEP) {
      m_c-= MOTORS_LOW_STEP;
    } else {
      m_c = m_s;
    }
    motors.mr_c = m_c;
  }

  if (m_c >= 0) {
    motors_ddr_r(1);
  } else {
    motors_ddr_r(0);
    m_c = -m_c;
  }
  cli();
  OCR1BH = 0x00;
  OCR1BL = m_c;
  sei();

  m_s = ((int16_t) motors.mh_s * (int16_t) motors.max) / 100;
  m_c = motors.mh_c;
  if (m_s != m_c) {
    if (m_s - m_c > MOTORS_LOW_STEP) {
      m_c+= MOTORS_LOW_STEP;
    } else if (m_c - m_s > MOTORS_LOW_STEP) {
      m_c-= MOTORS_LOW_STEP;
    } else {
      m_c = m_s;
    }
    motors.mh_c = m_c;
  }

  if (m_c >= 0) {
    motors_ddr_h(1);
  } else {
    motors_ddr_h(0);
    m_c = -m_c;
  }
  cli();
  OCR1AH = 0x00;
  OCR1AL = m_c;
  sei();
}
