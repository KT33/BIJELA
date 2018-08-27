/*
 * other.h
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#ifndef OTHER_H_
#define OTHER_H_

#include "stdint.h"

void init_ALL(void);
void log_sampling(void);
void log_start(void);
void Battery_Check(void);
void wait_time(int);
void chattering(void);
void init_sci1(void);
void put1byte(char);
void putnbyte(char*,int);
void putnbyte(char*,int);
int myprintf(const char *, ...);

void LED_Setting(void);
void Clock_Settting(void);

void ui_led_3bit(uint8_t value);
void real_velocity_control(void);
void real_angle_control(void);
void log_output(void);

#endif /* OTHER_H_ */
