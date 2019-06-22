#include <init.h>

void Init_Hardware(void) {
  // PORTA - Display
  //   Pin 0: Databit 4
  //   Pin 1: Databit 5
  //   Pin 2: Databit 6
  //   Pin 3: Databit 7
  //   Pin 4: Enable
  //   Pin 5: RS
  //   Pin 6: Read/Write
  //   Pin 7: Frei
    // durch system.h angesteuert und gesetzt

  // PORTB - Motortreiber
  //   Pin 0: Frei
  //   Pin 1: Frei
  //   Pin 2: Motor3 DDR
  //   Pin 3: Motor2 DDR
  //   Pin 4: Motor1 DDR
  //   Pin 5: Motor1 PWM (OC1A)
  //   Pin 6: Motor2 PWM (OC1B)
  //   Pin 7: Motor3 PWM (OC1C)
  DDRB  = 0xFC;
  PORTB = 0x00;

  // PORTC - Bumper
  //   Pin 0: Frei (Taster3)
  //   Pin 1: Bumper Hinten Links
  //   Pin 2: Bumper Hinten Rechts
  //   Pin 3: Bumper Rechts Rechts
  //   Pin 4: Bumper Rechts Links
  //   Pin 5: Bumper Tennisball
  //   Pin 6: Bumper Links  Links
  //   Pin 7: Bumper Links  Rechts
  DDRC  = 0x00;
  PORTC = 0xFE;

  // PORTD - TWI, XBEE, Taster
  //   Pin 0: TWI SCL
  //   Pin 1: TWI SDA
  //   Pin 2: XBEE TX (RXD1)
  //   Pin 3: XBEE RX (TXD1)
  //   Pin 4: Taster 1 Led
  //   Pin 5: Taster 1 Taster
  //   Pin 6: Taster 2 Taster
  //   Pin 7: Taster 2 Led
  DDRD  = 0x99;
  PORTD = 0x60;

  // PORTE - UART, Encoder
  //   Pin 0: UART TX (RXD0)
  //   Pin 1: UART RX (TXD0)
  //   Pin 2: Encoder 1
  //   Pin 3: Encoder 2
  //   Pin 4: Encoder 3
  //   Pin 5: Encoder 1 (INT5)
  //   Pin 6: Encoder 2 (INT6)
  //   Pin 7: Encoder 3 (INT7)
  DDRE  = 0x01;
  PORTE = 0x00;

  // PORTF - Analoge Sensoren
  //   Pin 0: Frei
  //   Pin 1: Frei
  //   Pin 2: Frei
  //   Pin 3: Frei
  //   Pin 4: Frei
  //   Pin 5: Frei
  //   Pin 6: Frei
  //   Pin 7: Frei
  DDRF  = 0x00;
  PORTF = 0x00;

  // PORTG - Frei
  //   Pin 0: Frei
  //   Pin 1: Frei
  //   Pin 2: Frei
  DDRG  = 0x00;
  PORTG = 0x00;

  // Timer1 - Motoren
  motors_init();
  // Speed
  motors_speed_init();
  // Encoder
  encoders_init();
  // Timer2 - Systick
  time_init();
  //
  datakom_init();

}



void time_init(void) {
  // Timer 2 - Systick
  TCCR2A = _BV(WGM21);  // CTC bis OCR2A; Kein Ausgang
  TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);   // Prescaler = 1024
  OCR2A  = 155;         // Top bei 155 ==> 100 Hz
  TIMSK2 = _BV(OCIE2A); // Interrupt für Comparematch A
}


void motors_init(void) {

  // Timer 1 - Motoren
  motors_stop();
  motors_set_max(MOTORS_INIT_MAX);

  ICR1H = 0x00;
  ICR1L =  100;  // Max. 100%

  TCCR1A = 0x00; // phasen- und frequenzrichtiger PWM bis ICR1
  TCCR1B = _BV(CS12) | _BV(WGM13); // ... PWM bis ICR1; Prescaler 256

  TCNT1H = 0x00;
  TCNT1L = 0x00;

  OCR1AH = 0x00;
  OCR1AL = 0x00;

  OCR1BH = 0x00;
  OCR1BL = 0x00;

  OCR1CH = 0x00;
  OCR1CL = 0x00;

  mdelay(10);
  TCCR1A|= _BV(COM1A1) | _BV(COM1B1) | _BV(COM1C1); // Ausgänge an
}

void motors_speed_init(void) {

  motors_speed_stop();
  motors_speed_set_max(MOTORS_SPEED_INIT_MAX);
  motors_speed_PID_reset();

}

void encoders_init(void) {

  encoders_reset();

  EIMSK=0;//Interupt auf Pins 5,6,7 (Port E) deaktivieren
  EICRB=_BV(ISC70)|_BV(ISC60)|_BV(ISC50);//logische Änderung für Pins 5,6,7 löst Interupt aus
  EIFR=_BV(INTF7)|_BV(INTF6)|_BV(INTF5);//Interupt auf Pin 5,6,7 zurücksetzen
  EIMSK=_BV(INT7)|_BV(INT6)|_BV(INT5);//Interupt auf Pins 5,6,7 (Port E) aktiviert

}

void datakom_init(void) {

  // Beide Ein/Ausgänge aktivieren
  setdatainp(1,1);
  setdataout(1,1,-1);

}
