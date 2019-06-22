#ifndef _INCLUDE_H_
#define _INCLUDE_H_

//*********************************<Import Definitionen>************************

// 2014 10 25 - peter: veraltet siehe "keplerboard.ini"
#define F_CPU  16000000  // Taktrate in Hz

#define UART0            // UART0 einschalten
#define UART0_BAUD 57600 // Übertragungsrate in Baud
#define UART0_TX 256     // Sendepuffergröße
#define UART0_RX 256     // Empfangspuffergröße

#define UART1            // UART1 einschalten
#define UART1_BAUD 57600 // Übertragungsrate in Baud
#define UART1_TX 256     // Sendepuffergröße
#define UART1_RX 256     // Empfangspuffergröße

#define DISPLAY          // Display einschalten
#define DISPLAY_PORTA    // Port an dem das Display angeschlossen ist

#define DELAY            // Delay einschalten

//*********************************<Eingebundene Dateien>***********************
// Standard Dateien
#include <inttypes.h>
#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

// Meine Eigenen Dateien
// 2014 10 25 - peter: veraltet siehe "keplerboard.ini"
//#include "system/source/system.h"
#include "keplerboard.h"

#include "IO/Datakom.h"//Datenaustausch



//*********************************<Math>***************************************
#define pi                3.141592653589793238462643
#define cos60             0.5
#define sin60             0.866025404
#define abs(x)            ((x<0)?(-x):x)


#endif
