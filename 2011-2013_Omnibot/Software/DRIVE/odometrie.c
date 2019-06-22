#include <odometrie.h>


//*********************************<Odometrie>************************************
void odo_timer_low(void) {

int16_t enc_s1,enc_s2,enc_s3;
enc_s3=encoder_get_speed(3)/ENCODER_ABRUF_HZ;
enc_s2=encoder_get_speed(2)/ENCODER_ABRUF_HZ;
enc_s1=encoder_get_speed(1)/ENCODER_ABRUF_HZ;

float xt1,yt1,xt2,yt2,xt3,yt3;

float ca=cos(odo.A*pi_div_180)
   ,sa=sin(odo.A*pi_div_180);
float ca60=(sa*sin60-ca*cos60)
   ,sa60=(sa*cos60+ca*sin60);

xt3=-(float)enc_s3*ca;
yt3=-(float)enc_s3*sa;
xt2= (float)enc_s2*ca60;
yt2=-(float)enc_s2*sa60;
xt1= (float)enc_s1*ca60;
yt1= (float)enc_s1*sa60;

/*    standartout('O');
      standartout(':');
      int16tostr(standartout,(int16_t)(xt1*10),5);
      standartout(':');
      int16tostr(standartout,(int16_t)(xt2*10),5);
      standartout(':');
      int16tostr(standartout,(int16_t)(xt3*10),5);
      standartout('/');
      int16tostr(standartout,(int16_t)(odo.A),4);
      standartout(':');
      int16tostr(standartout,(int16_t)(odo.X*100),6);
      standartout(':');
      int16tostr(standartout,(int16_t)(odo.Y*100),6);
      standartout('\n');
      standartout('\r');
//!!!???!!!Kleine Winkel => ..t..~0???!!!???*/
int temp=0;
if ((xt3==xt2)&&(xt2==xt1)){odo.X+=xt3*enc_to_m;temp++;}
else{odo.X+=((xt3+xt2+xt1)/3)*enc_to_m;}
//abweichungen einbeziehen?
if ((xt3==xt2)&&(yt2==yt1)){odo.Y+=yt2*enc_to_m;temp++;}
else{odo.Y+=((yt3+yt2+yt1)/3)*enc_to_m;}
//abweichungen einbeziehen?

if (temp==2)return ;//lineare Bewegung

//kreisbahn: + radius(Rückgabe)=> radius +: rechts von Vektor -: links von Vektor
float enc_d;
enc_d=enc_s1+enc_s2+enc_s3;
odo.A+=enc_d*enc_to_grad;
if (odo.A>180)odo.A-=360;
if (odo.A<-180)odo.A+=360;
}


float odo_point(float *X, float *Y) {
*X=odo.X;
*Y=odo.Y;
return odo.A;

}
