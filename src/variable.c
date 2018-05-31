/*
 * variable.c
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#include "variable.h"
#include "iodefine.h"
#include "stdint.h"

volatile int g_count;
volatile unsigned short Batt;
volatile float Battery;
char flag,gyro_flag=0;
volatile float angle;
int16_t encoder1,encoder2;
uint8_t  gyro_r;


