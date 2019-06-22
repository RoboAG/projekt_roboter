#include <Pololu3pi/Pololu3pi.h>

#define relV 70
#define Vmax 100

#define WB 0
#define G 1
#define R 2
#define S 3

int infrawerte[2];
int lostLine = 0;

int abs(int v) {
    return v < 0 ? -v : v;
}

int gruen = 0;

void inframessen(void) {
    set_analog_mode(MODE_8_BIT);
    infrawerte[1] = analog_read(6);
    infrawerte[0] = analog_read(7);
}

int getSensorStates(void){
    unsigned int values[5] = {0,0,0,0,0};
    int answer = 0;

    read_line_sensors(values, IR_EMITTERS_ON);

    for(int i = 0; i < 5; i++) {
        lcd_goto_xy(i, 1);

        if(values[i] > 670) {
            answer = answer | (1 << i);
            print("1");
        } else print("0");
    }

    return answer;
}

int poww(int a, int b) {
    int res = 1;
    for(int i = 0; i < b; i++) res *= a;
    return res;
}

#define MEASURECNT 5


void testLinie(void){

    long value;
    unsigned int values[5] = {0, 0, 0, 0, 0};

    while(1){
        value = getSensorStates();
        read_line_sensors(values, IR_EMITTERS_ON); //Nur für wert [4], da [0] immer 1 ist und [1] bis [3] immer überschrieben werden
        values[1] = value & 2;
        values[2] = value & 4;
        values[3] = value & 8;

        if(values[2] && values[1] == 0 && values[3] == 0)
            set_motors(70, 70);
        //else if(values[0] && values[1] && values[2])
        //set_motors(70,70);
        else if(values[2] && values[1] && values[3] == 0)
            set_motors(65, 70);
        else if(values[2] && values[1] == 0 && values[3])
            set_motors(70, 65);
        else if(values[2] == 0 && values[1])
            set_motors(30, 70);
        else if(values[2] == 0 && values[3])
            set_motors(70, 30);
        else if(values[1] == 0 && values[1] == 0 && values[1] == 0)
            set_motors(-32, -30);

        inframessen();
        if((infrawerte[0] > 100 || infrawerte[1] > 100)) {
                    // crash
            if( ((values[4] < 1300) && (gruen == 1)) || ((values[4] > 1300) && (gruen == 0))){
                set_motors(relV, relV);
                delay(500);

                set_motors(-relV, -relV);
                delay(1000);
                set_motors(relV, -relV);
                delay(600);
            } else {
                set_motors(-relV, -relV);
                delay(1000);
                set_motors(relV, -relV);
                //if(gruen) delay(700);
                //else delay(1300);
                delay(800);
            }
        }
        delay(30);
    }
}
//main//////////////////////////////////

int main (void) {

    pololu_3pi_init(2000);
    set_motors(0,0);
    unsigned char button = wait_for_button_press(BUTTON_A | BUTTON_C);
    unsigned int values[5] = {0, 0, 0, 0, 0};

    if(button == BUTTON_A){
        gruen = 1;
        print("gruen");
    } else {
        print("rot");
    }
    print("Press B!");
    wait_for_button_press(BUTTON_B);

    clear();
    testLinie();
}
