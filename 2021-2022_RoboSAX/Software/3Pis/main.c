#include<Pololu3pi/Pololu3pi.h>

#define sensor_max 2047
#define calibration_samples 512
#define calibration_period 4650
#define calibration_buckets 32
#define calibration_bucket_width (sensor_max / calibration_buckets)
#define calibration_split_percent 0.84
#define calibration_split_absolute calibration_split_percent * calibration_samples

#define speed 75
#define steering_factor (speed * (2.0 / 3.0))
#define time_factor (60.0 / speed)

void write_eeprom(unsigned int address, unsigned int data) {
    for(int i = 0; i < sizeof(unsigned int); i++) {
        while(EECR & _BV(1));
        while(SPMCSR & _BV(0));
        EEAR = address + i;
        EEDR = data >> (i * 8);
        EECR = 0b00000100;
        EECR = 0b00000110;
    }
}

unsigned int read_eeprom(unsigned int address) {
    unsigned int data = 0;
    for(int i = 0; i < sizeof(unsigned int); i++) {
        while(EECR & _BV(1));
        EEAR = address + i;
        EECR = 0b00000001;
        data |= EEDR << (i * 8);
    }
    return data;
}

unsigned int thresholds[5];

void store_thresholds(void) {
    for(int i = 0; i < 5; i++) {
        write_eeprom(i * sizeof(unsigned int), thresholds[i]);
    }
}

void load_thresholds(void) {
    for(int i = 0; i < 5; i++) {
        thresholds[i] = read_eeprom(i * sizeof(unsigned int));
    }
}

void print_thresholds(void) {
    for(int i = 0; i < 5; i++) {
        clear();
        print_long(i);
        lcd_goto_xy(0, 1);
        print_long(thresholds[i]);
        delay_ms(1000);
    }
}

float search_cuml_and_interpolate(unsigned short *buckets, float target) {
    unsigned int current_cuml_count = 0;
    unsigned int last_cuml_count = 0;
    unsigned int current_bucket_index = 0;
    while(current_cuml_count < target) {
        last_cuml_count = current_cuml_count;
        current_cuml_count += buckets[current_bucket_index];
        current_bucket_index++;
    }
    
    return current_bucket_index - 1 + (target - last_cuml_count) / (current_cuml_count - last_cuml_count);
}

void calibrate(void) {
    set_motors(30, -30);

    unsigned short buckets[5][calibration_buckets];
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < calibration_buckets; j++) {
            buckets[i][j] = 0;
        }
    }

    unsigned int sensor_values[5];
    for(int i = 0; i < calibration_samples; i++) {
        read_line_sensors(sensor_values, IR_EMITTERS_ON);
        for(int i = 0; i < 5; i++) {
            buckets[i][sensor_values[i] / calibration_bucket_width]++;
        }
        delay_ms(calibration_period / calibration_samples);
    }
    
    for(int i = 0; i < 5; i++) {
        thresholds[i] = (unsigned int) (search_cuml_and_interpolate(buckets[i], calibration_split_absolute) * calibration_bucket_width);
    }
    
    delay_ms(calibration_period % calibration_samples);
    set_motors(0, 0);
}

float line_pos(void) {
    unsigned int sensor_values[5];
    read_line_sensors(sensor_values, IR_EMITTERS_ON);
    
    //Ausgabe der normalisierten Sensorwerte
    /*clear();
    for(int i = 0; i < 5; i++) {
        lcd_goto_xy(i % 3 * 3, i / 3);
        int disp = (sensor_values[i]) / 100;
        if(disp < 10 && disp >= 0) {
            print_character(' ');
        }
        print_long(disp);
    }*/

    uint8_t is_line[5]; //verarbeitete Sensorwerte (>0: Linie, =0: keine Linie)
    for(int i = 0; i < 5; i++) {
        is_line[i] = (int) (sensor_values[i] > thresholds[i]);
    }
    
    //Berechnung der Position der Linie
    lcd_goto_xy(0, 1);
    int sum = 0;
    int num_detected = 0;
    for(int i = 0; i < 5; i++) {
        if(is_line[i]) {
            sum += i - 2;
            num_detected++;
            print_character('#');
        } else {
            print_character('_');
        }
    }

    //falls keine Linie erkannt: 1000 bedeutet Fehler
    if(num_detected == 0) {
        return 1000;
    } else {
        return (float) sum / num_detected;
    }
}

void obstacle(void) {
    set_motors(speed, -speed);
    delay_ms(250 * time_factor);
    unsigned int sensor_values[5] = {0, 0, 0, 0, 0};
    while(sensor_values[2] < thresholds[2]) {
        unsigned int dist_l = analog_read(6);
        if(dist_l > 60) {
            set_motors(speed, speed);
        } else {
            set_motors(speed * 2 / 3, speed);
        }
        read_line_sensors(sensor_values, IR_EMITTERS_ON);
    }
    set_motors(speed, speed / 3);
    delay_ms(400 * time_factor);
}

void task(void) {
    while(1) {
        clear();
        unsigned int dist_r = analog_read(7);
        print_long(dist_r);
        if(dist_r > 80) {
            obstacle();
        }
        
        float line = line_pos();
        //print_long((long) line);
        if(line == 1000) {
            set_motors(speed, speed);
            int filter = 0;
            for(int timer = 0; timer < 100; timer++) {
                delay_ms(10);
                line = line_pos();
                if(line != 1000) {
                    filter++;
                    if(filter >= 3) {
                        break;
                    }
                }
            }
            if(filter < 3) {
                set_motors(0, 0);
                filter = 0;
                do {
                    delay_ms(10);
                    line = line_pos();
                    if(line != 1000) {
                        filter++;
                    }
                } while(filter < 3);
            }
        }
        set_motors(speed + (int) (line * steering_factor), speed - (int) (line * steering_factor));
        delay_ms(10);
    }
}

int main(void) {
    set_motors(0, 0);
    set_analog_mode(MODE_8_BIT);
    pololu_3pi_init(sensor_max);

    load_thresholds();
    while(1) {
        unsigned char button = wait_for_button_release(wait_for_button_press(ANY_BUTTON));
        if(button == BUTTON_A) {
            calibrate();
            store_thresholds();
        } else if(button == BUTTON_B) {
            task();
        } else {
            print_thresholds();
        }
    }
    return 0;
}
