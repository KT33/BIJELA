/*
 * variable.c
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#include "variable.h"
#include "iodefine.h"
#include "stdint.h"

gain_t rotation_gain = { 0.1, 0.01, 0.0 };
gain_t run_gain = { 0.2, 0.1, 0.0 }; //p,i,d
gain_t wall_cntrol_gain = { 0.01, 0.0, 0.0 };

normal_para_t nomal_run = { 450.0, 450.0, 20.0, 1000.0 }; //search,max,min,accel
normal_para_t nomal_rotation = { 10.0, 10.0, 0.0, 100.0 }; //deg/sec

sensor_t SEN_R = { 0, 1116, 626, 0 }; //now,reference,threshold,diff
sensor_t SEN_L = { 0, 1082, 450, 0 };
sensor_t SEN_RF = { 0, 0, 425, 0 };
sensor_t SEN_LF = { 0, 0, 377, 0 };
sensor_t SEN_F = { 0, 0, 0, 0 };

volatile int16_t i;

volatile float diameter = 23.15; //タイヤ径
volatile float tread = 48.0; //トレッド幅

volatile int g_count;
volatile unsigned short Batt;
volatile float Battery;
char flag, gyro_flag = 0;
volatile float angle;

int16_t encoder_R, encoder_L;
//uint8_t gyro_r;
//sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF, SEN_F;

int8_t log_flag = 0, log_counter = 0, log_how_often = 0;
uint16_t log_index = 0;
run_t left_real, right_real;
duty_t duty = { 0, 0 };

volatile float log[LogMax];

volatile int speacer_i;
volatile float triangle;
//volatile uint8_t run_flag = 0; //0:停止 1:加速 2:等速 3:減速

run_t translation_ideal, rotation_ideal;
trapezoid_t translation_parameter, rotation_parameter;

uint8_t mode_flag = 0; //MSBが0の時モード選択中,1でモード確定
float mode_select_dis = 0;

deviation_t run_right_deviation = { 0.0, 0.0, 0.0 };
deviation_t run_left_deviation = { 0.0, 0.0, 0.0 };

int8_t test_flag = 0;

run_t rotation_real;

deviation_t rotation_deviation = { 0.0, 0.0, 0.0 };

SENLOG_t SEN_L_log, SEN_R_log;
uint8_t wall_control_flag = 0;

int test1,test2;
