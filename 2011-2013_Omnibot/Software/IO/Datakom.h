#ifndef _DATAKOM_H_
#define _DATAKOM_H_

#include <SYSTEM/include.h>

//*********************************<Datenaustausch>************************************

//define 0x01 -> an 0x00 -> aus
//#define  Read_XBee  (0x01)
//#define  Read_Kabel (0x01)
//#define  Send_XBee  (0x01)
//#define  Send_Kabel (0x01)

#define  standartout dataout//serout0

#define  standartinp datainp//serinp0
#define  standartstat datastat//serstat0

uint8_t Read_XBee ;
uint8_t Read_Kabel;
uint8_t Send_XBee ;
uint8_t Send_Kabel;
uint8_t Send_LCD  ;

void dataout(uint8_t send_data);
uint8_t datastat(void);
uint8_t datainp(void);

void setdatainp(int8_t  XBee,int8_t  Kabel);
void setdataout(int8_t  XBee,int8_t  Kabel,int8_t LCD);

void getdatainp(int8_t *XBee,int8_t *Kabel);
void getdataout(int8_t *XBee,int8_t *Kabel,int8_t *LCD);

#endif
