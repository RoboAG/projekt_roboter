#include <encoder.h>


//*********************************<Encoder>************************************
int16_t encoder_get(uint8_t nr) {
  int16_t erg = 0;
  uint8_t mSREG = SREG;

  cli();

  if (nr == 1) {
    erg = encoders.abs_l;
  } else if (nr == 2) {
    erg = encoders.abs_r;
  } else if (nr == 3) {
    erg = encoders.abs_h;
  }

  SREG = mSREG;

  return erg;
}

int16_t encoder_get_speed(uint8_t nr) {
  int16_t erg = 0;
  uint8_t mSREG = SREG;

  cli();

  if (nr == 1) {
    erg = encoders.vel_l;
  } else if (nr == 2) {
    erg = encoders.vel_r;
  } else if (nr == 3) {
    erg = encoders.vel_h;
  }

  SREG = mSREG;

  return erg;
}

void encoders_timer_low(void) {
  cli();
  encoders.vel_l=(encoders.abs_l-encoders.tmp_l)*ENCODER_ABRUF_HZ;
  encoders.vel_r=(encoders.abs_r-encoders.tmp_r)*ENCODER_ABRUF_HZ;
  encoders.vel_h=(encoders.abs_h-encoders.tmp_h)*ENCODER_ABRUF_HZ;
  encoders.tmp_l=encoders.abs_l;
  encoders.tmp_r=encoders.abs_r;
  encoders.tmp_h=encoders.abs_h;
  sei();
/*
  int16tostr(standartout,encoder_get_speed(1),6);
  flashtostr(standartout,PSTR(":"));
  int16tostr(standartout,encoder_get_speed(2),6);
  flashtostr(standartout,PSTR(":"));
  int16tostr(standartout,encoder_get_speed(3),6);
  flashtostr(standartout,PSTR("\\\\\\"));
  int16tostr(standartout,encoder_get(1),6);
  flashtostr(standartout,PSTR(":"));
  int16tostr(standartout,encoder_get(2),6);
  flashtostr(standartout,PSTR(":"));
  int16tostr(standartout,encoder_get(3),6);
  flashtostr(standartout,PSTR("\n\r"));
*/
}

float encoder_vector(float *X, float *Y) {
/*
Inverse zu motors_vektor->vektor aus encoder auslesen
X->encoder.vel
Y->0
Xt->'reales' X
Yt->'reales' Y

enc 3:
Xt:-X;Yt:-Y;
=>Xt:i-X;Yt:0;
enc 2:
Xt:X*cos( 60°)+Y*sin( 60°);Yt:-X*sin( 60°)+Y*cos( 60°);
=>Xt:X/cos( 60°);Yt:-X/sin( 60°);
enc 1:
Xt:X*cos(-60°)+Y*sin(-60°);Yt:-X*sin(-60°)+Y*cos(-60°);
=>Xt:X/cos(-60°);Yt:-X/sin(-60°);
*/
int16_t enc_s1,enc_s2,enc_s3;
//*
enc_s3=encoder_get_speed(3);
enc_s2=encoder_get_speed(2);
enc_s1=encoder_get_speed(1);/*/
enc_s3=encoder_get(3);
enc_s2=encoder_get(2);
enc_s1=encoder_get(1);//*/

float xt1,yt1,xt2,yt2,xt3,yt3;

xt3=-(float)enc_s3;
yt3=0;
xt2= (float)enc_s2/cos60;
yt2=-(float)enc_s2/sin60;
xt1= (float)enc_s1/cos60;
yt1= (float)enc_s1/sin60;

int temp=0;
if ((xt3==xt2)&&(xt2==xt1)){*X=xt3;temp++;}
else{*X=(xt3+xt2+xt1)/3;}
//abweichungen einbeziehen?
if (            (yt2==yt1)){*Y=yt2;temp++;}
else{*Y=(yt2+yt1)/2;}
//abweichungen einbeziehen?

if (temp==2)return 0;//gültiger linearer vektor => Radius = 0

//kreisbahn: + radius(Rückgabe)=> radius +: rechts von Vektor -: links von Vektor
float enc_d;
enc_d=enc_s1+enc_s2+enc_s3;
return enc_d;//...

}

void encoders_reset(void) {
  uint8_t mSREG = SREG;

  cli();

  encoders.abs_l = 0;
  encoders.abs_r = 0;
  encoders.abs_h = 0;

  encoders.vel_l = 0;
  encoders.vel_r = 0;
  encoders.vel_h = 0;

  SREG = mSREG;
}

