//Dies ist die letzte stabile Version, die auch im RoboSAX verwendet wurde.
//Bitte Ausschnitte nur mit folgenden Kommentar verwenden:
/*
+----------------------------------------------------------------+
| Ausschnitte dieses Programms stammen aus dem Programm          |
| des RoboSAX 2018 der RobotikAG des                             |
| Johannes-Kepler-Gymnasium Chemnitz.                            |
| Das Programm des RoboSAX 2018 der RobotikAG des                |
| Johannes-Kepler-Gymnasium Chemnitz kann mit folgendem Link     |
| gefunden werden: <Link>                                        |
| Das Programm des RoboSAX 2018 der RobotikAG des                |
| Johannes-Kepler-Gymnasium Chemnitz wurde von Janek Neugebauer; |
| Leonhard Nagel; Felix Krüger; Christoph Männel programmiert    |
+----------------------------------------------------------------+
*/

#include "3pi_neu.h"
#include <Pololu3pi/Pololu3pi.h>

#define uart0_baudrate 57600
#define uart0_buffer_size 64
uint8_t uart0_buffer[uart0_buffer_size];
uint8_t uart0_buffer_position = 0;

unsigned int linesensors[5]; //Der Array für die Bodensensor-Werte

void uart0_init(void) {//Zeug für XBEE Nicht von uns Copy Paste
  uart0_buffer_position = 0;
  serial_set_baud_rate(uart0_baudrate);
  serial_receive_ring((char*) uart0_buffer, uart0_buffer_size);
}

uint8_t uart0_receive(void) {
  while(serial_get_received_bytes() == uart0_buffer_position);

  uint8_t result = uart0_buffer[uart0_buffer_position];
  ++uart0_buffer_position;
  if(uart0_buffer_position >= uart0_buffer_size) {uart0_buffer_position = 0;}

  return result;
}

void uart0_send(uint8_t data) {
  serial_send_blocking((char *) &data, 1);
}

void send_int(int data) { //sendet Zahl an XBEE Modul
    if(data < 0) {        //Fall für negative Zahlen
        uart0_send('-');
        send_int(-data);  //Betrag der negativen Zahl also positiv wird gesendet
        return;
    } else if(data >= 10) { //Zerhackt die Zahl und sendet sie
        send_int(data / 10);
    }
    uart0_send('0' + data % 10);
}

void send_string(char data[]) {//Sendet eine Zeichenkette
    string_from_ram(uart0_send, data);
}

int infrawerte[2]; //Array für Infrarotsensorwerte

void inframessen(void){//Auslesen der Infrarotsensoren
    set_analog_mode(MODE_8_BIT);
    infrawerte[1] = analog_read(6);
    infrawerte[0] = analog_read(7);
}

/* int infraauswerten(int marceldavis)
 * Diese Funktion wurde entfernt, weil sie total sinnlos war*/ //RIP infraauswerten()

void init_leds(void){//Legt den Port für den Diodensensor fest
    PORTB &= ~_BV(2);
    DDRB  &= ~_BV(2);
}
int read_leds(void){//Liest den Diodensensor aus
    return(PINB & _BV(2));
}

void motoransteuerung(void){//Motoransteuerung
    inframessen();//Auslesen der Infrarotsensoren
    int dreh = 375;//Legt Zeitwert für 90°-Drehen fest
    if(infrawerte[0] < 45) {
        set_motors(100,40); //enger Kreis nach rechts
        //send_string("drehen_r");
    }else{
    read_line_sensors(linesensors, IR_EMITTERS_ON);//initieren der Bodensensoren
    if(linesensors[4] > 1300) {
        set_motors(20, 20); //langsamer
        delay(500); //warten bis LED und Sensor an gleicher Stelle
        if(!read_leds()) {//Wenn kein Infrarotsignal erfasst wird
            send_string("\nLED aus, Knopf wird gedrückt.\n");
            green_led(0);
            delay(100);
            set_motors(50, -50); //rechts drehen
            delay(dreh);
            set_motors(70, 70); //rammen
            delay(600);
            set_motors(-50, -50); //zurück fahren
            delay(350);
            set_motors(-50, 50); //links drehen
            delay(dreh);
        } else {//Wenn ein Infrarotsignal erfasst wird
            send_string("\nLED an, Knopf wird nicht gedrückt.\n");
            green_led(1);
        }
    } else{
        if(infrawerte[1] >= 80){
            set_motors(-50,50);//90°-Drehen
            delay(dreh);
            //send_string("drehen_l\n");
        }else if(infrawerte[0] < 140 && infrawerte[0] >= 110){
            set_motors(100,100);//Vorwärts
            //send_string("gut\n");
        }else if(infrawerte[0] >= 140){
            set_motors(75,100);//Nach außen korrigieren
            //send_string("zu nah\n");
        }else if(infrawerte[0] < 110 && infrawerte[0] >= 45){
            set_motors(100,75);//Nach innen korrigieren
            //send_string("zu weit\n");
        }else {
            set_motors(0,0);//Etwas sehr komisches
            //send_string("?????\n");
        }
    }
    }
}

int main(void){//Starten des Roboters
    set_motors(0,0);//Zuerst Motoren auf 0 setzen, sonst könnten sie plötzlich losfahren
    uart0_init();//XBEE initieren
    init_leds();//Diodensensor initieren (Im Übrigen steht leds für LED-Sensor nicht für die Mehrzahl von LED
    pololu_3pi_init(2000);//3pi initieren
    delay(1000);//Kurz warten
    inframessen();//Das erste mal Infrarotentfernungswerte auslesen
    while(infrawerte[0] < 80 || infrawerte[1] < 80) {//Von der Mitte zur Wand fahren
        delay(10);//Nach jedem Zyklus kurz warten
        inframessen();//Infrarotinfernungswerte auslesen
        set_motors(100,100);//Motoren aktivieren
    }
    green_led(1);
    set_motors(-50,-50);//Etwas zurück fahren
    delay(200);
    set_motors(-50,50);//90° drehen
    delay(375);
    while(1){
        motoransteuerung();
        green_led(read_leds());
        red_led(!read_leds());
        delay(5);//Nach jedem Zyklus kurz warten
    }
    return 0;
}
