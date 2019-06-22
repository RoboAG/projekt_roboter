/*******************************************************************************
* Omniwheel                                                                    *
* =========                                                                    *
*                                                                              *
* Version: 1.0.0                                                               *
* Datum  : 03.06.14                                                            *
* Autoren: Leander Herr, Peter Weissig                                         *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
* D-SUB9                                                                       *
* ======                                                                       *
*                                                                              *
*   9er-Kabel  |   D-SUB9 Pin  |  Komponente  |  Steckplatz Robo  | MCU Pin    *
*  ------------+---------------+--------------+-------------------+--------    *
*     1 (rot)  |    1          |  XBEE TX     |    Servo 2        | PORTD.3    *
*     2        |        6      |  XBEE GND    |    Servo GND      | (GND)      *
*     3        |    2          |  XBEE VCC    |    Servo VCC      | (Vcc)      *
*     4        |        7      |  XBEE RX     |    Servo 1        | PORTD.2    *
*     5        |    3          |  Led 1 Anode |    Servo 0        | PORTD.4    *
*     6        |        8      |  Taster 1    |    Servo 3        | PORTD.5    *
*     7        |    4          |  Ta&Led GND  |    Servo GND      | (GND)      *
*     8        |        9      |  Taster 2    |    Servo 4        | PORTD.6    *
*     9        |    5          |  Led 2 Anode |    Servo 5        | PORTD.7    *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
* D-SUB15                                                                      *
* =======                                                                      *
*                                                                              *
*  15er-Kabel  |  D-SUB15 Pin  |  Komponente  |  Steckplatz Robo  | MCU Pin    *
*  ------------+---------------+--------------+-------------------+--------    *
*     1 (rot)  |    1          |  Display VSS |  Displayport  1   | (GND)      *
*     2        |        9      |  Display VDD |  Displayport  2   | (Vcc)      *
*     3        |    2          |  Display V0  |  Displayport  3   | (Poti)     *
*     4        |       10      |  Display RS  |  Displayport  4   | PORTA.5    *
*     5        |    3          |  Display R/W |  Displayport  5   | PORTA.6    *
*     6        |       11      |  Display E   |  Displayport  6   | PORTA.4    *
*     7        |    4          |  Display DB4 |  Displayport 11   | PORTA.0    *
*     8        |       12      |  Display DB5 |  Displayport 12   | PORTA.1    *
*     9        |    5          |  Display DB6 |  Displayport 13   | PORTA.2    *
*    10        |       13      |  Display DB7 |  Displayport 14   | PORTA.3    *
*    11        |    6          |  Display BLA |  Displayport 15   | (15 Ohm)   *
*    12        |       14      |  Display BLK |  Displayport 16   | (GND)      *
*    13        |    7          |              |                   |            *
*    14        |       15      |              |                   |            *
*    15        |    8          |              |                   |            *
*                                                                              *
*******************************************************************************/


// Meine Eigenen Dateien
#include <SYSTEM/include.h>
// 2014 10 25 - peter: veraltet siehe "keplerboard.ini"
//#include <system/source/system.c>

#include <DRIVE/motors.h>
#include <SYSTEM/Zeit.h>
#include <IO/Bumpers.h>
#include <IO/LED-Buttons.h>

#include <Remote-control.h>

#include <init.h>


//*********************************<Allgemeines>********************************
int main (void) {

  Init_System();
  Init_Hardware();
  sei();

  flashtostr(standartout, PSTR("\n\r"));
  flashtostr(standartout, PSTR("Omniwheel Roboter\n\r"));
  flashtostr(standartout, PSTR("=================\n\r\n\r"));

  flashtostr(lcdout, PSTR("Omniwheel - Xbee"));
  lcdxy(0,1);
  flashtostr(lcdout, PSTR("  Warte         "));

  remote_control();

  return (0);
}
