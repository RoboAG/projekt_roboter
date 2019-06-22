#include <speed.h>

//*********************************<Speed>************************************
void motors_speed_PID_reset(void) {
  uint8_t mSREG = SREG;

  cli();
  PID.iel=0;
  PID.ier=0;
  PID.ieh=0;


  SREG = mSREG;
}

void motors_speed_stop(void) {
  uint8_t mSREG = SREG;

  cli();

  Speed.sl = 0;
  Speed.sr = 0;
  Speed.sh = 0;


  Speed.max_nr = 0;

  SREG = mSREG;
  motors_set(0,0,0);
}

void motors_speed_set(int8_t sl, int8_t sr, int8_t sh) {
    standartout('I');
    standartout('L');
    int16tostr(standartout,sl,6);
    standartout('R');
    int16tostr(standartout,sr,6);
    standartout('H');
    int16tostr(standartout,sh,6);
    standartout('\n');
    standartout('\r');

  int16_t max=0;
  int8_t maxnr=0;
  if (max<abs(sl)){max=abs(sl);maxnr=1;}
  if (max<abs(sr)){max=abs(sr);maxnr=2;}
  if (max<abs(sh)){max=abs(sh);maxnr=3;}

  //float scale=1;
  if (max>100){/*
    scale=100/(float)max;
    sl=(int8_t)(sl*scale);
    sr=(int8_t)(sr*scale);
    sh=(int8_t)(sh*scale);*/
    sl=((sl*100)/max);
    sr=((sr*100)/max);
    sh=((sh*100)/max);
  }

  if (sl >  100) { sl =  100;}
  if (sl < -100) { sl = -100;}
  if (sr >  100) { sr =  100;}
  if (sr < -100) { sr = -100;}
  if (sh >  100) { sh =  100;}
  if (sh < -100) { sh = -100;}

  uint8_t mSREG = SREG;

  cli();

  Speed.sl = sl;
  Speed.sr = sr;
  Speed.sh = sh;


  Speed.max_nr = maxnr;


  SREG = mSREG;
    standartout('O');
    standartout('L');
    int16tostr(standartout,sl,6);
    standartout('R');
    int16tostr(standartout,sr,6);
    standartout('H');
    int16tostr(standartout,sh,6);
    standartout('M');
    int16tostr(standartout,max,6);
    standartout('\n');
    standartout('\r');
}


/*void motor_speed_set(uint8_t nr, int8_t m) {
  if (m >  100) { m =  100;}
  if (m < -100) { m = -100;}
  if ((nr < 1) || (nr > 3)) { return;}

  if (nr == 1) {
    Speed.sl = m;
  } else if (nr == 2) {
    Speed.sr = m;
  } else if (nr == 3) {
    Speed.sh = m;
  }
}
*/
int8_t motor_speed_get(uint8_t nr) {
  if ((nr < 1) || (nr > 3)) { return 0;}

  if (nr == 1) {
    return Speed.sl;
  } else if (nr == 2) {
    return Speed.sr;
  } else if (nr == 3) {
    return Speed.sh;
  }

  return 0;
}

void motors_speed_set_max(uint8_t max) {
  if (max<0)    {max=0;}
  if (max>100)    {max=100;}
  Speed.max = max;
}

uint8_t motors_speed_get_max(void) {
  return Speed.max;
}


void motors_speed_timer_low(void) {
int8_t ml,mr,mh;
int16_t enc_basis=0;

if (!Speed.max_nr)return;
     enc_basis=abs(encoder_get_speed(Speed.max_nr));
    //PID Regler

//ist-Wert: encoder_get_speed(x) soll wert:(Speed.sx*enc_basis)/100
//*
//float P=0.1,i=0.01,d=0;
float er,el,eh;
el=(((float)(Speed.sl*enc_basis)/100.)-encoder_get_speed(1));
er=(((float)(Speed.sr*enc_basis)/100.)-encoder_get_speed(2));
eh=(((float)(Speed.sh*enc_basis)/100.)-encoder_get_speed(3));

PID.iel+=el;
PID.ier+=er;
PID.ieh+=eh;


ml=Speed.sl+(int8_t)(PID_P*el+PID_I*PID.iel);
mr=Speed.sr+(int8_t)(PID_P*er+PID_I*PID.ier);
mh=Speed.sh+(int8_t)(PID_P*eh+PID_I*PID.ieh);

int8_t max=0;
  if (max<abs(ml)){max=abs(ml);}
  if (max<abs(mr)){max=abs(mr);}
  if (max<abs(mh)){max=abs(mh);}

  //float scale=1;
  if (max>100){/*
    scale=1;//100/(float)max;
    ml=(int8_t)(ml*scale);
    mr=(int8_t)(mr*scale);
    mh=(int8_t)(mh*scale);*/
    ml=((ml*100)/max);
    mr=((mr*100)/max);
    mh=((mh*100)/max);
  }

  lcdclr();
    lcdxy(0,0);
int16tostr(lcdout, encoder_get_speed(1),6);
    lcdxy(8,0);
int16tostr(lcdout, encoder_get_speed(2),6);
    lcdxy(4,1);
int16tostr(lcdout, encoder_get_speed(3),6);

/*/




    if     ((encoder_get_speed(1)*Speed.sl)/100<enc_basis)
        if    ((encoder_get_speed(1)*Speed.sl)/100>-enc_basis)    ml=   0;
        else                                                    ml=-100;
    else                                                        ml= 100;
    if     ((encoder_get_speed(2)*Speed.sr)/100<enc_basis)
        if    ((encoder_get_speed(2)*Speed.sr)/100>-enc_basis)    mr=   0;
        else                                                    mr=-100;
    else                                                        mr= 100;
    if     ((encoder_get_speed(2)*Speed.sh)/100<enc_basis)
        if    ((encoder_get_speed(2)*Speed.sh)/100>-enc_basis)    mh=   0;
        else                                                    mh=-100;
    else                                                        mh= 100;

//* /

    standartout('M');
    standartout('L');
    int16tostr(standartout,ml,6);
    standartout('R');
    int16tostr(standartout,mr,6);
    standartout('H');
    int16tostr(standartout,mh,6);
    standartout('E');
    standartout('M');
    int16tostr(standartout,enc_basis,6);
    standartout('L');
    int16tostr(standartout,encoder_get_speed(1),6);
    standartout('R');
    int16tostr(standartout,encoder_get_speed(2),6);
    standartout('H');
    int16tostr(standartout,encoder_get_speed(3),6);
    standartout('\n');
    standartout('\r');
*/

    motors_set(ml,mr,mh);

}




//*********************************<Other>************************************
void motors_vector(float X, float Y) {
/*
motor Xt->Power

motor 3:
sin (alpha+180°)=Xt:-X;Yt:-Y
motor 2:
sin (alpha+60°)=Xt:X*cos( 60°)+Y*sin( 60°);Yt:-X*sin( 60°)+Y*cos( 60°);
motor 1:
sin (alpha-60°)=Xt:X*cos(-60°)+Y*sin(-60°);Yt:-X*sin(-60°)+Y*cos(-60°);
*/
    float motor_power_1,motor_power_2,motor_power_3;
    float help,relativ=1;
//Every Vektor To Einheitskreis(sicherheitshalber)
/*
    x²+Y²=1 => X*=1/sqrt(X²+Y²)
*/
/*    float scale=1/sqrt(X*X+Y*Y);
    X*=scale;
    Y*=scale;
*/
    if((X!=0)||(Y!=0)){
        if(fabs(X)>fabs(Y))    {Y/=fabs(X);X/=fabs(X);}
        else                    {X/=fabs(Y);Y/=fabs(Y);}
/*                                                //{Winkel}=Grunddrehung(Roboter->Absulutes Bezugssystem?)
        motor_power_1=X*cos( pi/3)+Y*sin( pi/3);    //X*cos({Winkel}+pi/3)+Y*sin({Winkel}+pi/3);
        motor_power_2=X*cos(-pi/3)+Y*sin(-pi/3);    //X*cos({Winkel}-pi/3)+Y*sin({Winkel}-pi/3);
        motor_power_3=-X;                            //X*cos({Winkel}+pi  )+Y*sin({Winkel}+pi  );
*/
        motor_power_1= X*cos60+Y*sin60;
        motor_power_2= X*cos60-Y*sin60;
        motor_power_3=-X;


        /*if (fabs(motor_power_1)>1)*/{relativ=(1/fabs(motor_power_1));}
        /*if (fabs(motor_power_2)>1)*/{   help=(1/fabs(motor_power_2));if (help<relativ)relativ=help;}
        /*if (fabs(motor_power_3)>1)*/{   help=(1/fabs(motor_power_3));if (help<relativ)relativ=help;}


        motor_power_1*=relativ;
        motor_power_2*=relativ;
        motor_power_3*=relativ;


        motors_speed_set((int8_t)(motor_power_1*100),(int8_t)(motor_power_2*100),(int8_t)(motor_power_3*100));
        //motors_set((int8_t)(motor_power_1*100),(int8_t)(motor_power_2*100),(int8_t)(motor_power_3*100));
    }
    else
        motors_speed_stop();


}


void fahre_winkel(int8_t rich, int8_t speed) {
#define WINKEL_VOLL 255
#define WINKEL_FEIN 100
#define SPEED_VOLL 255
    uint8_t grob_rich=((uint16_t)rich*6)/WINKEL_VOLL;
    uint8_t fein_rich=((uint32_t)rich*6*WINKEL_FEIN)%WINKEL_VOLL;
/*rich            grob_rich
 =WINKEL_VOLL     0 --> motors_set(- speed/SPEED_VOLL,  speed/SPEED_VOLL,               0  )
 =WINKEL_VOLL  /6 1 --> motors_set(               0  ,  speed/SPEED_VOLL,- speed/SPEED_VOLL)
 =WINKEL_VOLL*1/3 2 --> motors_set(  speed/SPEED_VOLL,               0  ,- speed/SPEED_VOLL)
 =WINKEL_VOLL  /2 3 --> motors_set(  speed/SPEED_VOLL,- speed/SPEED_VOLL,               0  )
 =WINKEL_VOLL*2/3 4 --> motors_set(               0  ,- speed/SPEED_VOLL,  speed/SPEED_VOLL)
 =WINKEL_VOLL*5/6 5 --> motors_set(- speed/SPEED_VOLL,               0  ,  speed/SPEED_VOLL)*/
//cos 30°*speed// cos
    switch (grob_rich) {
      case 0:motors_set(- (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL,
                          (((uint16_t)speed*100)                        )/SPEED_VOLL,
                        - (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL);break;
      case 1:motors_set(  (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL,
                          (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL,
                        - (((uint16_t)speed*100)                        )/SPEED_VOLL);break;
      case 2:motors_set(  (((uint16_t)speed*100)                        )/SPEED_VOLL,
                        - (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL,
                        - (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL);break;
      case 3:motors_set(  (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL,
                        - (((uint16_t)speed*100)                        )/SPEED_VOLL,
                          (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL);break;
      case 4:motors_set(- (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL,
                        - (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL,
                          (((uint16_t)speed*100)                        )/SPEED_VOLL);break;
      case 5:motors_set(- (((uint16_t)speed*100)                        )/SPEED_VOLL,
                          (((uint16_t)speed*100)*(            fein_rich))/SPEED_VOLL,
                          (((uint16_t)speed*100)*(WINKEL_FEIN-fein_rich))/SPEED_VOLL);break;
    }



}

