/*
 * other.h
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#ifndef OTHER_H_
#define OTHER_H_

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

#endif /* OTHER_H_ */
