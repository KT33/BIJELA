/*
 * variable.c
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#include "variable.h"
#include "iodefine.h"
#include "stdint.h"

volatile float diameter = 23.15; //タイヤ径
volatile float tread = 48.0; //トレッド幅


volatile int g_count;
volatile unsigned short Batt;
volatile float Battery;
char flag, gyro_flag = 0;
volatile float angle;
int16_t encoder_R, encoder_L;
uint8_t gyro_r;
sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF;

int8_t log_flag=0,log_counter=0,log_how_often=0;
uint16_t log_index=0;

volatile float log[LogMax];

volatile int speacer_i;
volatile float triangle;
//volatile uint8_t run_flag = 0; //0:停止 1:加速 2:等速 3:減速


run_t translation_ideal,rotation_ideal;
trapezoid_t translation_parameter,rotation_parameter;
normal_para_t nomal_run={450.0,1000.0,200.0,1000.0};
run_t *pp;




