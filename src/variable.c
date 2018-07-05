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
char flag, gyro_flag = 0;
volatile float angle;
int16_t encoder_R, encoder_L;
uint8_t gyro_r;
sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF;
float log[LogMax];
uint16_t log_index;
speed_t speed_R,speed_L;
speed_t distance_R,distance_L;
float diameter=23.15;//タイヤ径
float tread=48.0;//トレッド幅
float accel=2000;



