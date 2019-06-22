#include <Remote-control.h>

//*********************************<Remote_Control>***************************** 03.06.2014
void remote_bumpers(void) {
  uint8_t bumpers;
  uint8_t disp_out = 0xFF;

  flashtostr(standartout, PSTR("b - Bumpers:\r\n"));
  flashtostr(lcdout, PSTR("Bumpers "));

  bumpers = bumpers_get(BUMPERS_ALL);

  if (bumpers & BUMPERS_HINTEN_LINKS) {
    flashtostr(standartout, PSTR("  hinten links (Bit 1)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("hili 1"));
    }
  }

  if (bumpers & BUMPERS_HINTEN_RECHTS) {
    flashtostr(standartout, PSTR("  hinten rechts (Bit 2)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("hire 2"));
    }
  }

  if (bumpers & BUMPERS_RECHTS_RECHTS) {
    flashtostr(standartout, PSTR("  rechts rechts (Bit 3)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("rere 3"));
    }
  }

  if (bumpers & BUMPERS_RECHTS_LINKS) {
    flashtostr(standartout, PSTR("  rechts links (Bit 4)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("reli 4"));
    }
  }

  if (bumpers & BUMPERS_LINKS_LINKS) {
    flashtostr(standartout, PSTR("  links links (Bit 6)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("lili 6"));
    }
  }

  if (bumpers & BUMPERS_LINKS_RECHTS) {
    flashtostr(standartout, PSTR("  links rechts (Bit 7)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("lire 7"));
    }
  }

  bumpers = bumpers_get(BUMPERS_BALL);
  if (bumpers & BUMPERS_BALL) {
    flashtostr(standartout, PSTR("  Ball (Bit 5)\r\n"));
    if (disp_out) {
      disp_out = 0x00;
      flashtostr(lcdout, PSTR("ball 5"));
    }
  }

  if (disp_out) {
    flashtostr(standartout, PSTR("  Nichts gedrueckt\r\n"));
    flashtostr(lcdout, PSTR("nichts"));
  }

}

void remote_zeit(void) {
  uint16_t ticks;

  flashtostr(standartout, PSTR("z - Zeit:\r\n"));
  flashtostr(lcdout, PSTR("Zeit "));

  ticks = time_get();


  flashtostr(standartout, PSTR("  "));
  uint16tostr(standartout,time_get_min(ticks)  ,2);
  standartout(':');
  uint16tostr(standartout,time_get_sec(ticks)  ,2);
  standartout(',');
  uint16tostr(standartout,time_get_milli(ticks),3);
  flashtostr(standartout, PSTR("\r\n"));

  uint16tostr(lcdout,time_get_min(ticks),2);
  lcdout(':');
  uint16tostr(lcdout,time_get_sec(ticks),2);
  flashtostr(lcdout, PSTR("   "));
}

void remote_motors(void) {
  flashtostr(standartout, PSTR("motor:\r\n"));
  flashtostr(lcdout, PSTR("motor         "));

  flashtostr(standartout, PSTR("  ml= "));
  int16tostr(standartout,motor_get(1),4);
  flashtostr(standartout, PSTR("\n\r  mr= "));
  int16tostr(standartout,motor_get(2),4);
  flashtostr(standartout, PSTR("\n\r  mh= "));
  int16tostr(standartout,motor_get(3),4);
  flashtostr(standartout, PSTR("\r\n"));
}

void remote_power(uint8_t increment) {

  uint8_t power;
  power = motors_get_max();

  if (increment) {
    if (power < 255 - 10) {power+= 10;} else {power = 255;}
  } else {
    if (power >       10) {power-= 10;} else {power =   0;}
  }
  motors_set_max(power);

  flashtostr(standartout, PSTR("power:\r\n"));
  flashtostr(lcdout, PSTR("power "));

  flashtostr(standartout, PSTR("  max= "));
  uint16tostr(standartout,power,3);
  flashtostr(standartout, PSTR("\n\r"));

  uint16tostr(lcdout,power,3);
  flashtostr(lcdout, PSTR("     "));
}

void remote_idle(void) {
static  uint16_t ticks;

  lcdxy(0,2);
  int16tostr(lcdout,encoder_get(1),6);
  flashtostr(lcdout,PSTR("  "));
  int16tostr(lcdout,encoder_get(2),6);
  flashtostr(lcdout,PSTR("  "));

  lcdxy(0,3);
  flashtostr(lcdout,PSTR("     "));
  int16tostr(lcdout,encoder_get(3),6);
  flashtostr(lcdout,PSTR("     "));

  if (buttons_get(BUTTONS_STOP)) {
    leds_toggle(LEDS_THE);
  }
  if (buttons_get(BUTTONS_THE)) {
    lcdclr();
    leds_clear(LEDS_THE);
    leds_set(LEDS_STOP);
    uint8_t temp=0;
    while(temp<15){
        mdelay(500);
        leds_toggle(LEDS_ALL);
        lcdxy(5,1);
        flashtostr(lcdout,PSTR("Shutdown"));
        mdelay(500);
        leds_toggle(LEDS_ALL);
        lcdclr();
        temp++;
    }
    leds_clear(LEDS_ALL);
    while (1);
  }
  if (ticks>10) {
    leds_toggle(LEDS_THE);
    ticks=0;
  }
  else{
    ticks++;
  }


  mdelay(50);
}


void remote_controller(void) {
  uint8_t cmd;
  uint8_t cmd_r=0;
  int16_t pow=0;
  int8_t x=0;
  int8_t multipl=1;
  uint8_t xpoint=0;
  int8_t y=0;
  uint8_t bumpers;
  static uint8_t Status_out=0;
  static uint8_t go=0;

  while(1) {
    standartout('V');
    int16tostr(standartout,x,6);
    standartout('/');
    int16tostr(standartout,y,6);
    standartout('\n');
    standartout('\r');
    while (standartstat() == 0) {
       if (Status_out){
            float X,Y,A;
            A=odo_point(&X, &Y);
            X*=100;
            Y*=100;
//          A*=100;
            standartout('X');
            standartout(':');
            int16tostr(standartout,(int16_t)X,6);
            standartout(' ');
            standartout(' ');
            standartout('Y');
            standartout(':');
            int16tostr(standartout,(int16_t)Y,6);
            standartout(' ');
            standartout(' ');
            standartout('A');
            standartout(':');
            int16tostr(standartout,(int16_t)A,6);
            standartout('Z');
            standartout(':');
            int16tostr(standartout,time_get_sec(time_get()),4);
            standartout('\n');
            standartout('\r');
        }
      remote_idle();
      bumpers = bumpers_get(BUMPERS_ALL);
      if (bumpers){motors_stop();}
    }

    cmd = standartinp();
    lcdxy(0,1);
    xpoint=0;
    lcdout(cmd);
    lcdout(32);
     switch (cmd) {
      case 'b':
      case 'B': return;  ; break;

      case 'y':
      case 'Y': cmd_r=1; break;

      case 'x':
      case 'X': cmd_r=2; break;

      case 'l':
      case 'L': cmd_r=3; break;

      case 'v':
      case 'V': cmd_r=4; break;

      case 'h':
      case 'H': cmd_r=5; break;

      case 's':
      case 'S': cmd_r=0;Status_out=!Status_out;break;

      case 'g':
      case 'G': cmd_r=6;go=!go;break;

      case ' ': motors_speed_stop();return; break;

      default : cmd_r=0; flashtostr(lcdout, PSTR("Error         "));
    }
    multipl=1;
    pow=0;
    if ((cmd_r==1)||(cmd_r==2)){
        while(cmd!=';') {
    int16tostr(standartout,pow,6);
    standartout(',');
            while (standartstat() == 0) {
              remote_idle();
              bumpers = bumpers_get(BUMPERS_ALL);
              if (bumpers){motors_stop();}
            }

            cmd = standartinp();
            //lcdxy(++xpoint,1);
            lcdout(cmd);
            if (cmd!=';')pow*=10;
            switch (cmd) {
              case '9': pow++;
              case '8': pow++;
              case '7': pow++;
              case '6': pow++;
              case '5': pow++;
              case '4': pow++;
              case '3': pow++;
              case '2': pow++;
              case '1': pow++;
              case '0': break;
              case '-': multipl=-multipl;break;
              case ';': break;
              case ' ': motors_speed_stop();return; break;
             default : flashtostr(lcdout, PSTR("Error         "));
            }

        }
        if(pow>100)pow=100;
        int16tostr(standartout,pow,6);
        standartout('\n');
        standartout('\r');
        switch (cmd_r) {
            case 1: y=pow*multipl;break;
            case 2: x=pow*multipl;break;
            default : flashtostr(lcdout, PSTR("Error         "));
        }
      }

    if ((cmd_r==1)||(cmd_r==2)||(cmd_r=6)){
        if (go)motors_vector((float)x/100,(float)y/100);
      }
    }
}

void remote_control(void) {
  uint8_t cmd;

  while(1) {
    while (standartstat() == 0) {
      remote_idle();
    }

    cmd = standartinp();
    lcdxy(0,1);
    lcdout(cmd);
    lcdout(32);

    switch (cmd) {
      case 'b':
      case 'B': remote_bumpers()   ; break;


      case 'z':
      case 'Z': remote_zeit()      ; break;

      case 'c':
      case 'C': remote_controller(); break;


      case '*': remote_power(0xFF) ; break;
      case '/': remote_power(0x00) ; break;

      case '+': motors_set(  100,  100,  100); remote_motors(); break;
      case '-': motors_set( -100, -100, -100); remote_motors(); break;

      case '7': motors_set(    0,  100, -100); remote_motors(); break;
      case '3': motors_set(    0, -100,  100); remote_motors(); break;
      case '8': motors_set( -100,  100,    0); remote_motors(); break;
      case '2': motors_set(  100, -100,    0); remote_motors(); break;
      case '9': motors_set( -100,    0,  100); remote_motors(); break;
      case '1': motors_set(  100,    0, -100); remote_motors(); break;

      case '4': motors_set(  100,  100, -100); remote_motors(); break;
      case '6': motors_set( -100, -100,  100); remote_motors(); break;

      case '5': motors_set(    0,    0,    0); remote_motors(); break;
      case '0': motors_set(0,0,0); remote_motors(); break;
      case ' ': motors_stop();     remote_motors(); break;


      default : flashtostr(lcdout, PSTR("Error         "));
    }


  }
}

