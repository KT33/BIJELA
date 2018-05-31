/*
 * variable.h
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "stdint.h"

#define UI_LED1 PORT2.PODR.BIT.B7
#define UI_LED2 PORT5.PODR.BIT.B4
#define UI_LED3 PORT5.PODR.BIT.B5
#define RIGHTWING PORTC.PODR.BIT.B5
#define LEFTEING PORT0.PODR.BIT.B5
#define LEFTFRONT PORTE.PODR.BIT.B0
#define CENTERFRONT PORTA.PODR.BIT.B6
#define RIGHTFRONT PORTC.PODR.BIT.B3
#define SWITCH PORT3.PIDR.BIT.B1
#define CHATT 10000
#define SENLED_L PORT4.PODR.BIT.B3
#define SENLED_LF PORT4.PODR.BIT.B1
#define SENLED_R PORTB.PODR.BIT.B7
#define SENLED_RF PORTC.PODR.BIT.B2
#define FAN PORT4.PODR.BIT.B6
#define Moter_L_IN1 PORTA.PODR.BIT.B3
#define Moter_L_IN2 PORTA.PODR.BIT.B4
#define Moter_Stby PORTB.PODR.BIT.B0
#define Moter_R_IN1 PORTB.PODR.BIT.B5
#define Moter_R_IN2 PORTB.PODR.BIT.B6

extern volatile int g_count;
extern volatile unsigned short Batt;
extern volatile float Battery;
extern char flag,gyro_flag;
extern volatile float angle;
extern uint8_t  gyro_r;
extern int16_t encoder1,encoder2;

#endif /* VARIABLE_H_ */
