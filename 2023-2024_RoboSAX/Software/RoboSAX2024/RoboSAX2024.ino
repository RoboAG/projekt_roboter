#define ANALOG2PERCENT 0.0978

#include <MeMCore.h>
#include <MeDrive.h>
#include <NEPODefs.h>
#include <Adafruit_TCS34725.h>
#include <MePort.h>
#include <MeRGBLed.h>
#include <MeLineFollower.h>


using namespace std;

MeLineFollower __meLineFollower2(PORT_2);
MeDrive _meDrive(M1, M2);
MeDCMotor leftMotor(M1);
MeDCMotor rightMotor(M2);
MeRGBLed _meRgbLed(7, 2);
MePort farbsensorPort(PORT_1);
Adafruit_TCS34725 farbsensor(TCS34725_INTEGRATIONTIME_60MS, TCS34725_GAIN_16X);
uint16_t r_w, g_w, b_w, c_w;


typedef enum {
  COL_RED,
  COL_GREEN,
  COL_BLUE,
  COL_WHITE
}
Color;

int Farbe(int r, int g, int b);

void setup() {
  Serial.begin(9600);
  farbsensor.begin();

  while (analogRead(A7) > 512){
    _meRgbLed.setColor(255,255,255);  //weiß
    _meRgbLed.show();
  }

  farbsensor.getRawData(&r_w, &g_w, &b_w, &c_w);
  Serial.print("r=");
  Serial.print(r_w);
  Serial.print(" g=");
  Serial.print(g_w);
  Serial.print(" b=");
  Serial.print(b_w);
}

void blinken() {

  while (analogRead(A7) > 512){
     _meRgbLed.setColor(255, 255, 255); // weiß
     _meRgbLed.show();
     _meRgbLed.setColor(255,255,0); //gelb
     _meRgbLed.show();
     _meRgbLed.setColor(0,0,255); //blau
     _meRgbLed.show();
     _meRgbLed.setColor(255,0,0); //rot
     _meRgbLed.show();
  }
}
void forward(){
  leftMotor.run(-150);
  rightMotor.run(150);

  delay(250);

  leftMotor.stop();
  rightMotor.stop();
}

void right(){
  leftMotor.run(-150);
  rightMotor.stop();

  delay(150);
  _meRgbLed.setColor(255,255,255); //weiß

  leftMotor.stop();
  rightMotor.stop();
}

void left(){
  rightMotor.run(-150);
  leftMotor.stop();

  delay(150);

  leftMotor.stop();
  rightMotor.stop();
}

void stop(){
  leftMotor.stop();
  rightMotor.stop();
}


int followLineBlack() {
    if (__meLineFollower2.readSensor1() && __meLineFollower2.readSensor2()) {
        _meDrive.steer(90, 90, 1); //vor
        _meRgbLed.setColor(0,255,0);  //grün
        _meRgbLed.show();
    }
    else if (!__meLineFollower2.readSensor1() && !__meLineFollower2.readSensor2()) {
        _meDrive.steer(10,100, 0); //links
        delay(200);
        _meRgbLed.setColor(255,255,0);  //gelb
        _meRgbLed.show();
      return 1;
    }
    else {
        if (!__meLineFollower2.readSensor1() ) {
            _meDrive.steer(10, 90, 1, 1); //links
            _meRgbLed.setColor(255,0,0);  //rot
            _meRgbLed.show();
        }
        else if (!__meLineFollower2.readSensor2() ) {
            _meDrive.steer(90, 10, 1, 1); //rechts
            _meRgbLed.setColor(0,0,255);  //blau
            _meRgbLed.show();
        }
    }
  return 0;
}
void followLineBlackProgramm() {
    if (__meLineFollower2.readSensor1() && __meLineFollower2.readSensor2()) {
        _meDrive.steer(90, 90, 0); //rueckwaerts
        _meRgbLed.setColor(0,255,0);  //grün
        _meRgbLed.show();
    }
    else if (!__meLineFollower2.readSensor1() && !__meLineFollower2.readSensor2()) {
        _meDrive.steer(90, 90, 1); //vor
        _meRgbLed.setColor(255,255,0);  //gelb
        _meRgbLed.show();
    }
    else {
        if (!__meLineFollower2.readSensor1() ) {
            _meDrive.steer(10, 90, 1, 1); //links
            _meRgbLed.setColor(255,0,0);  //rot
            _meRgbLed.show();
        }
        else if (!__meLineFollower2.readSensor2() ) {
            _meDrive.steer(90, 10, 1, 1); //rechts
            _meRgbLed.setColor(0,0,255);  //blau
            _meRgbLed.show();
        }
    }
}
void followBlue(Color curr_color){
  _meRgbLed.setColor(0,0,255); //blau
  forward();

  while (curr_color != COL_BLUE){
    right();
  }
  right();

  while (!__meLineFollower2.readSensor1() && !__meLineFollower2.readSensor2()){
    right();
    forward();

    while (curr_color != COL_BLUE) {
      left();
    }
  }
}

int middle(Color curr_Color){
  int count = 0;

  while (count < 7){
    followLineBlack();
    farbCheck();
    Color curr_color = farbCheck();

    if (curr_color == COL_BLUE){
      count = count + 1;
      _meRgbLed.setColor(255,200,0);  //orange
      _meRgbLed.show();
      delay(100);
    }
    return count;
  }
}

Color farbCheck(){
  //farbsesor lesen
  uint16_t r, g, b, c;
  // farbsensor steht fuer die Adafruit_TCS3           4725-Objektvariable
  farbsensor.getRawData(&r, &g, &b, &c);
  Serial.print("r=");
  Serial.print(r);
  Serial.print(" g=");
  Serial.print(g);
  Serial.print(" b=");
  Serial.print(b);
  r = ((uint32_t) r << 8) / r_w;
  g = ((uint32_t) g << 8) / g_w;
  b = ((uint32_t) b << 8) / b_w;
  Serial.print("\tr=");
  Serial.print(r);
  Serial.print(" g=");
  Serial.print(g);
  Serial.print(" b=");
  Serial.println(b);

  // Messwerte stehen jetzt in den Variablen r, g, b und c
  Color curr_color = Farbe(r, g, b);
  if (curr_color == COL_RED) {
    if (r - 10 < g && r-10 <b){
      _meRgbLed.setColor(255,255,255); //weiß
      return COL_WHITE;
    }
    else{
      _meRgbLed.setColor(255,0,0); //rot
      return COL_RED;
    }
  }
  else if (curr_color == COL_GREEN) {
    if (g - 10 < b && g-10 <r){
      _meRgbLed.setColor(255,255,255); //weiß
      return COL_WHITE;
    }
    else{
      _meRgbLed.setColor(0,255,0); //gruen
      return COL_GREEN;
    }
  }
  else if (curr_color == COL_BLUE){
    if (b - 10 < g && b-10 <r){
      _meRgbLed.setColor(255,255,255); //weiß
      return COL_WHITE;
    }
    else{
      _meRgbLed.setColor(0,0,255); //blau
      return COL_BLUE;
    }
  }
    else {
      _meRgbLed.setColor(255,0,255); //lila
  }
    _meRgbLed.show();

  return curr_color;
}

int Farbe(uint16_t r, uint16_t g, uint16_t b) {
  if (r > g) {
    if (r > b) {
      return COL_RED;
    }
    else {
      return COL_BLUE;
    }
  }
  else if (g > b) {
    return COL_GREEN;
  }
  else {
    return COL_BLUE;
  }
}

int start(){
  if (!__meLineFollower2.readSensor1() || !__meLineFollower2.readSensor2()){
    return 1;
  }
  else if(!__meLineFollower2.readSensor1() && !__meLineFollower2.readSensor2()){
    return 1;
  }
  else{
    followLineBlack();
    return 2;
  }
}


void loop() {
  int angekommen = 0;

  while (angekommen == 0){
  angekommen = followLineBlack();
  }
  while (true){
    followLineBlackProgramm();
  }
}
