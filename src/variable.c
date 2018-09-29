/*
 * variable.c
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#include "variable.h"
#include "iodefine.h"
#include "stdint.h"

gain_t rotation_gain = { 0.62, 0.010, 0.0 };
//0.56, 0.015:安定感あり・段差弱し
//0.62,0.009:段差対策・安定感薄し
gain_t run_gain = { 0.8, 0.4, 0.0 }; //p,i,d
gain_t wall_cntrol_gain = { 0.046, 0.0, 0.0 }; //0.041
float oblique_Front_gain = 0.6;//0.6
float oblique_Side_gain = 0.01; //0.1

normal_para_t nomal_run = { 600.0, 2200.0, 20.0, 7000.0 }; //search,max,min,accel
normal_para_t nomal_rotation = { 600.0, 600.0, 0.0, 2000.0 }; //deg/sec //400.0, 400.0, 0.0, 500.0
slarom_para_t slarom_500 = { 7000.0, 6000.0, { 15.0, 35.0 }, { 15.0, 22.0 } };
slarom_para_t slarom_600 = { 12500.0, 6000.0, { 21.0, 42.0 }, { 21.0, 36.0 } };

sensor_t SEN_R = { 0, 1370, 600, 0, 0, 3272, 2944 }; //now,reference,threshold,diff
sensor_t SEN_L = { 0, 1785, 574, 0, 0, 3368, 3100 }; //oblique_reference,oblique_threshold
//3368
sensor_t SEN_RF = { 0, 3282, 728, 0, 0, 224, 190 };
sensor_t SEN_LF = { 0, 3320, 680, 0, 0, 359, 330 };
sensor_t SEN_F = { 0, 0, 0, 0 };

float wallcontrol_value;

volatile int16_t i;

volatile float diameter = 23.40; //タイヤ径 50
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

SENLOG_t SEN_L_log, SEN_R_log, SEN_RF_log, SEN_LF_log;
uint8_t wall_control_flag = 0;

int test1, test2;

uint8_t direction = 0, direction_pass = 0;
XY_t x, y;

walldate_t walldate_real, walldate_checked, walldate_adachi;
uint16_t step_map[16][16];
uint8_t pass[255];
uint8_t pass_compression[255];
uint8_t pass_big[255];
uint8_t pass_oblique[255];
uint8_t moter_flag = 0;

float test_float;

uint8_t failsafe_flag = 0;

volatile float Log[LogMax], log2[LogMax], log3[LogMax];

uint8_t SEN_check_flag = 0;
float failsafe_accel;

uint16_t led_count = 0;
uint8_t wall_control_oblique_flag = 0;

