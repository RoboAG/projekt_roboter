#include <Datakom.h>

//*********************************<Datenaustausch>************************************

void dataout(uint8_t send_data){
  if (Send_XBee)  serout1(send_data);
  if (Send_Kabel) serout0(send_data);
  if (Send_LCD)   lcdout(send_data);
}

uint8_t datastat(void){
  if ((Read_XBee)&&(Read_Kabel))  return (serstat1()||serstat0());
  if (Read_XBee)  return serstat1();
  if (Read_Kabel) return serstat0();
  return 0;
}

/*
 * Datainp: serinp Ausgewählte Kanäle
 *  wenn beide: warte bis auf einem ein Zeichen gelesen wurde und serinpe diesen Kanal
 *  Priorität: Kabel
 *
 */
uint8_t datainp(void){
  if ((Read_XBee)&&(Read_Kabel)){
    while (!(serstat1()||serstat0())) nop();
    if (serstat0()) return serinp0();
    else return serinp1();
  }
  else{
    if (Read_Kabel) return serinp0();
    if (Read_XBee)  return serinp1();
  }
  return 0;
}
/*
 * Setdatainp: Kanäle zur Dateneingabe auswählen
 *  XBee : Steuert XBee  Kanal
 *  Kabel: Steuert Kabel Kanal
 *
 *  -1: Kanal NICHT zur Eingabe nutzen
 *   0: Kanal nicht verändern
 *   1: Kanal       ZUR Eingabe nutzen
 *
 */
void setdatainp(int8_t XBee,int8_t Kabel){
    if (Kabel) Read_Kabel=Kabel+1;
    if (XBee)  Read_XBee =XBee +1;
}
/*
 * Setdataout: Kanäle zur Datenausgabe auswählen
 *  siehe Setdatainp.
 * +LCD  : Ausgabe via LCD Display Kanal
 *
 */
void setdataout(int8_t XBee,int8_t Kabel,int8_t LCD){
    if (Kabel) Send_Kabel=Kabel+1;
    if (XBee)  Send_XBee =XBee +1;
    if (LCD)   Send_LCD  =LCD  +1;
}
/*
 * getdatainp: Auslesen welche Kanäle zur Dateneingabe genutzt werden
 *  XBee : XBee  Kanal
 *  Kabel: Kabel Kanal
 *
 *  -1: Kanal NICHT zur Eingabe genutzt
 *   1: Kanal       ZUR Eingabe genutzt
 *
 */
void getdatainp(int8_t *XBee,int8_t *Kabel){
    if (Read_Kabel) *Kabel=1;
    else *Kabel=-1;
    if (Read_XBee)  *XBee=1;
    else *XBee=-1;
}
/*
 * getdataout: Auslesen welche Kanäle zur Datenausgabe genutzt werden
 *  siehe getdatainp.
 * +LCD  : LCD Display Kanal
 *
 */
void getdataout(int8_t *XBee,int8_t *Kabel,int8_t *LCD){
    if (Send_Kabel) *Kabel=1;
    else *Kabel=-1;
    if (Send_XBee)  *XBee=1;
    else *XBee=-1;
    if (Send_LCD)   *LCD=1;
    else *LCD=-1;
}
