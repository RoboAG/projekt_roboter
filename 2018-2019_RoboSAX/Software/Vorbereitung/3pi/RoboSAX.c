/* Version 2.4.1
   RoboSAXgreen
*/

#include "3pi_neu.h"
#include <Pololu3pi/Pololu3pi.h>

#define WB 0
#define G 1
#define R 2
#define S 3

#define uart0_baudrate 57600
#define uart0_buffer_size 64
uint8_t uart0_buffer[uart0_buffer_size];
uint8_t uart0_buffer_position = 0;

unsigned int linesensors[5];

uint8_t team;
uint8_t last_color = WB;
uint8_t color_saved;

void uart0_init(void) {
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

void send_int(int data) {
    if(data < 0) {
        uart0_send('-');
        send_int(-data);
        return;
    } else if(data >= 10) {
        send_int(data / 10);
    }
    uart0_send('0' + data % 10);
}

void send_string(char data[]) {
    string_from_ram(uart0_send, data);
}

int infrawerte[2];

void inframessen(void) {
    set_analog_mode(MODE_8_BIT);
    infrawerte[1] = analog_read(6);
    infrawerte[0] = analog_read(7);
}

/* int infraauswerten(int marceldavis)
 * Diese Funktion wurde entfernt, weil sie total sinnlos war*/

void init_leds(void) {
    PORTB &= ~_BV(2);
    DDRB  &= ~_BV(2);
}
int read_leds(void) {
    return(PINB & _BV(2));
}

uint8_t get_color(/*uint8_t sensor,*/ uint16_t brightness) {
    /*const int[3][5] = {
        {,,},
        {,,}, // R G 600
        {,,}, // R G 600
        {,,}, // R G 450
        {,,}  // R G 100
    }
    if(brightness < 600) return WB;
    if(brightness < 700) return G;
    if(brightness < 1000) return R;
*/
    return S;
}

void testSensoren(void) {
     while(1) {
        inframessen();
        read_line_sensors(linesensors, IR_EMITTERS_ON);
        // uint8_t color_m = get_color(linesensors[3]);
        // uint8_t color_r = get_color(linesensors[4]);

        clear();
        lcd_goto_xy(0, 0);
        print_unsigned_long(linesensors[1]);
        lcd_goto_xy(5, 0);
        print_unsigned_long(linesensors[2]);
        lcd_goto_xy(0, 1);
        print_unsigned_long(linesensors[3]);
        lcd_goto_xy(5, 1);
        print_unsigned_long(linesensors[4]);

        delay(100);
    }
}

void testIRSensoren(void) {
     while(1) {
        inframessen();
        // uint8_t color_m = get_color(linesensors[3]);
        // uint8_t color_r = get_color(linesensors[4]);

        clear();
        lcd_goto_xy(0, 0);
        print_unsigned_long(infrawerte[0]);
        lcd_goto_xy(0, 1);
        print_unsigned_long(infrawerte[1]);

        delay(100);
    }
}

void motoransteuerung(void) {
    inframessen();
    int dreh = 375;

    if(infrawerte[0] < 45) {
        set_motors(100,40); //enger Kreis nach rechts
        //send_string("drehen_r");
    } else {
        read_line_sensors(linesensors, IR_EMITTERS_ON);
        uint8_t color_m = get_color(linesensors[3]);
        uint8_t color_r = get_color(linesensors[4]);

        lcd_goto_xy(0,0);
        print_unsigned_long(color_m);
        lcd_goto_xy(5,0);
        print_unsigned_long(color_r);

        if(color_m == color_r && color_r != S) {
            last_color = color_r;
        }

        if(color_r == S && (last_color == team || last_color == WB)) {
            set_motors(20, 20); // langsamer
            delay(500);         // warten bis LED und Sensor an gleicher Stelle
            if(read_leds()) {
                send_string("\nLED an, Knopf wird gedrückt.\n");
                green_led(1);
                delay(100);
                set_motors(50, -50);  // rechts drehen
                delay(dreh);
                set_motors(70, 70);   // rammen
                delay(600);
                set_motors(-50, -50); // zurück fahren
                delay(350);
                set_motors(-50, 50);  // links drehen
                delay(dreh);
            } else {
                send_string("\nLED aus, Knopf wird nicht gedrückt.\n");
                green_led(0);
            }
        } else {
            if(infrawerte[1] >= 80) {
                set_motors(-50,50);
                delay(dreh);
                // send_string("drehen_l\n");
            } else if(infrawerte[0] < 160 && infrawerte[0] >= 130) {
                set_motors(50,50);
                // send_string("gut\n");
            } else if(infrawerte[0] >= 160) {
                set_motors(40,50);
                // send_string("zu nah\n");
            } else if(infrawerte[0] < 130 && infrawerte[0] >= 45) {
                set_motors(50,40);
                //send_string("zu weit\n");
            } else {
                set_motors(0,0);
                // send_string("?????\n");
            }
        }
    }
}

int main(void) {
    set_motors(0,0);
    uart0_init();
    init_leds();
    pololu_3pi_init(2000);

    while(!(button_is_pressed(BUTTON_A) || button_is_pressed(BUTTON_C)));

    // TEST
    testIRSensoren();
    //testSensoren();
    return 0;

    if(button_is_pressed(BUTTON_A)) {
        team = G;
        green_led(1);
    } else {
        team = R;
        red_led(1);
    }
    delay(2000);

    // zur Wand fahren
    inframessen();
    while(infrawerte[0] < 80 || infrawerte[1] < 80) {
        delay(10);
        inframessen();
        set_motors(100,100);
    }
    green_led(1);
    set_motors(-50,-50);
    delay(200);
    set_motors(-50,50);
    delay(375);

    while(1) {
        motoransteuerung();
        green_led(read_leds());
        red_led(!read_leds());
        delay(10);
    }
    return 0;
}
