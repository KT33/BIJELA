/*
 * variable.h
 *
 *  Created on: 2018/04/08
 *      Author: takao keisuke
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include "stdint.h"

#define LogMax 2000


typedef struct {
	int now;//今の値
	int reference;//真ん中にいるときのセンサー値
	int threshold;//閾値
	int diff;//差分
} sensor_t;

typedef struct{
	float accel;//加速度・各速度
	float velocity;//速度・各速度
	float dis;//距離・角度
} run_t;

typedef struct{
	float Kp;
	float Ki;
	float Kd;
} gain_t;

typedef struct{
	float i_distance;
	float accel;
//	float min_vel;
	float max_vel;
	float strat_vel;
	float end_vel;
	float triangle;
	int8_t back_rightturn_flag;
	float acceldistance;
	float deacceldistance;
	int8_t run_flag;
} trapezoid_t;

typedef struct{
	float vel_search;
	float vel_max;
	float vel_min;
	float accel;
} normal_para_t;

typedef struct{
	int left;
	int right;
} duty_t;

typedef struct{
	float now;//現在の偏差
	float cumulative;//偏差の累積値
	float difference;//偏差の差分
} deviation_t;//偏差

typedef struct{
	int before_1ms;
	int before_2ms;
	int before_3ms;
	int before_4ms;
	int before_5ms;
	int now;
}SENLOG_t;

typedef struct{
	uint8_t now;
	uint8_t goal;
	uint8_t pass;
}XY_t;

typedef struct{
	uint16_t row[17];
	uint16_t column[17];
}walldate_t;

typedef struct{
	float in_offset;
	float out_offset;
}sla_t;

typedef struct{
	float accel;
	float max_vel;
	sla_t left;
	sla_t right;
}slarom_para_t;

extern volatile int16_t i;
extern volatile int g_count;
extern volatile unsigned short Batt;
extern volatile float Battery;
extern char flag,gyro_flag;
extern volatile float angle;
//extern uint8_t  gyro_r;
extern int16_t encoder_L,encoder_R;
extern sensor_t SEN_R, SEN_RF, SEN_L, SEN_LF,SEN_F;
//#define LogMax 200

extern int8_t log_flag,log_counter,log_how_often;
extern uint16_t log_index;
extern volatile float diameter;//タイヤ径
extern volatile float tread;//トレッド幅
extern volatile int speacer_i;
extern volatile float triangle;
extern volatile float velocity;
//extern volatile uint8_t run_flag;

extern run_t left_real,right_real;
extern run_t translation_ideal,rotation_ideal;
extern trapezoid_t translation_parameter,rotation_parameter;
extern normal_para_t nomal_run;
extern gain_t run_gain;
extern duty_t duty;
extern uint8_t mode_flag;
extern float mode_select_dis;
extern deviation_t run_right_deviation;
extern deviation_t run_left_deviation;
extern int8_t test_flag;
extern run_t rotation_real;
extern gain_t rotation_gain;
extern deviation_t rotation_deviation;
extern normal_para_t nomal_rotation;
extern SENLOG_t SEN_L_log,SEN_R_log;
extern uint8_t wall_control_flag;
extern gain_t wall_cntrol_gain;
extern int test1,test2;
extern uint8_t direction,direction_pass;
extern XY_t x,y;
extern float wallcontrol_value;
extern walldate_t walldate_real,walldate_checked,walldate_adachi;
extern uint16_t step_map[16][16];
extern uint8_t pass[255];
extern uint8_t pass_compression[255];
extern uint8_t moter_flag;
extern float test_float;
extern uint8_t failsafe_flag;

extern volatile float Log[LogMax],log2[LogMax],log3[LogMax];
extern uint8_t SEN_check_flag;
extern float failsafe_accel;

extern slarom_para_t slarom_500;
extern slarom_para_t slarom_600;

extern uint8_t pass_big[255];

extern uint16_t led_count;

#define UI_LED1 PORT2.PODR.BIT.B7
#define UI_LED2 PORT5.PODR.BIT.B4
#define UI_LED3 PORT5.PODR.BIT.B5
#define RIGHTWING PORTC.PODR.BIT.B5
#define LEFTEING PORT0.PODR.BIT.B5
#define LEFTFRONT PORTA.PODR.BIT.B0
#define CENTERFRONT PORTA.PODR.BIT.B6
#define RIGHTFRONT PORTC.PODR.BIT.B3
#define SWITCH PORT3.PIDR.BIT.B1
#define CHATT 10000
#define SENLED_L PORT4.PODR.BIT.B3
#define SENLED_LF PORT4.PODR.BIT.B1
#define SENLED_R PORTB.PODR.BIT.B7
#define SENLED_RF PORTC.PODR.BIT.B2
#define FAN PORT4.PODR.BIT.B6
#define Moter_L_BACK PORTA.PODR.BIT.B3
#define Moter_L_FRONT PORTA.PODR.BIT.B4
#define Moter_Stby PORTB.PODR.BIT.B0
#define Moter_R_FRONT PORTB.PODR.BIT.B5
#define Moter_R_BACK PORTB.PODR.BIT.B6
#define North 0
#define West 1
#define South 2
#define East 3


#endif /* VARIABLE_H_ */
