#include <Pololu3pi/Pololu3pi.h>

#define relV 50
#define Vmax 80

#define WB 0
#define G 1
#define R 2
#define S 3

int infrawerte[2];
int lostLine = 0;

int abs(int v) {
    return v < 0 ? -v : v;
}

void inframessen(void) {
    set_analog_mode(MODE_8_BIT);
    infrawerte[1] = analog_read(6);
    infrawerte[0] = analog_read(7);
}

uint8_t get_color(uint8_t sensor, uint16_t brightness) {
    const int refValues[5][4] = {
        // B G R S
        {0,0,0,0},             //                                 rr
        {390, 480, 860, 1200}, // R 900 G 600 B 400 W 400 S 1200  r
        {250, 350, 610, 1200}, // R 650 G 450 B 280 W 280 S 1200  m
        {390, 550, 900, 1200}, // R 950 G 600 B 400 W 400 S 1200  l
        {0,0,0,0}              //                                 ll
    };

    int col = WB, diff, mindiff = 2000;
    for(int i = 0; i < 4; i++) {
        diff = abs(refValues[sensor][i] - brightness);
        if(mindiff > diff) {
            mindiff = diff;
            col = i;
        }
    }

    return col;
}

void testSensoren(void) {
    unsigned int linesensors[5];

     while(1) {
        read_line_sensors(linesensors, IR_EMITTERS_ON);
        for(int i = 1; i < 4; i++) linesensors[i] = get_color(i, linesensors[i]);

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

int getSensorStates(void){
    unsigned int values[5];
    int answer = 0;

    read_line_sensors(values, IR_EMITTERS_ON);

    for(int i = 0; i < 5; i++) {
        lcd_goto_xy(i, 1);

        if(values[i] > 670) {
            answer = answer | (1 << i);
            print("1");
        }
        else print("0");
    }

    return answer;
}

int poww(int a, int b) {
    int res = 1;
    for(int i = 0; i < b; i++) res *= a;
    return res;
}

#define MEASURECNT 5

void fahre_linie(void) {
    long pr, in = 0, fa = 7, fb = 0.3, value;
    unsigned int values[5] = {0, 0, 0, 0, 0};

    while(1) {
        inframessen();
        read_line_sensors(values, IR_EMITTERS_ON);
        value = getSensorStates();

        // lcd_goto_xy(0, 0);
        // print_long(values);

        for(int i = 0; i < 5; i++)
            if(value & 1 << i) lostLine = 0;

        if(lostLine < 8) {
            pr = 0;

            for(int i=0; i<5; i++)
                if(value & 1 << i)
                    switch(i) {
                        case 0: pr -= 5; break;
                        case 1: pr -= 2; break;
                        case 2: pr += 0; break;
                        case 3: pr += 2; break;
                        case 4: pr += 5; break;
                    }

            in += pr;
            if(pr == 0) in = 0;

            int vl = (int)(relV + pr*fa + in*fb);
            if(vl > Vmax) vl = Vmax;
            if(vl < 10)   vl = 10;

            int vr = (int)(relV - pr*fa - in*fb);
            if(vr > Vmax) vr = Vmax;
            if(vr < 10)   vr = 10;

            set_motors(vl,vr);

            //print velocities
            //clear();
            //lcd_goto_xy(0,1);
            //print_long(vl);
            //print(" ");
            //print_long(vr);

            if(infrawerte[0] > 130 || infrawerte[1] > 130) {
                // crash
                set_motors(relV, relV);
                delay(1000);
                set_motors(-relV, -relV);
                delay(1000);
                set_motors(relV, -relV);
                delay(800);
            }

            delay(50);
        } else {
            set_motors(-relV, relV);
            read_line_sensors(values, IR_EMITTERS_ON);

            while(lostLine) {
                for(int i = 0; i < 5; i++)
                    if(values[i] == S) lostLine = 0;
            }
        }
    }
}

//main//////////////////////////////////

int main (void) {

    pololu_3pi_init(2000);
    set_motors(0,0);

    print("Press B!");
    wait_for_button_press(BUTTON_B);
    clear();

    fahre_linie();
}
