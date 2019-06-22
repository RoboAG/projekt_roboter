#ifndef _REMOTE-CONTROL_H_
#define _REMOTE-CONTROL_H_

#include <SYSTEM/include.h>
#include <DRIVE/motors.h>
#include <DRIVE/speed.h>
#include <DRIVE/odometrie.h>
#include <SYSTEM/Zeit.h>
#include <IO/Bumpers.h>
#include <IO/LED-Buttons.h>

//*********************************<Remote_Control>***************************** 03.06.2014
void remote_bumpers(void);

void remote_zeit(void);

void remote_motors(void);

void remote_power(uint8_t increment);

void remote_idle(void);

void remote_controller(void);

void remote_control(void);

#endif
