/*
 * mode.c
 *
 *  Created on: 2018/07/16
 *      Author: takao keisuke
 */

#include "mode.h"
#include "iodefine.h"
#include "CMT.h"
#include "run.h"
#include "SPI.h"
#include "variable.h"
#include "machine.h"
#include "other.h"
#include "Clock.h"
#include "speaker.h"
#include "stdint.h"
#include "walldate.h"
#include "pass.h"

void mode_0(void) {
//	uint8_t flag;
	moter_flag = 1;
	adachi_search_run(1, 0, nomal_run.accel, nomal_run.vel_search);
	wait_time(2000);
	adachi_search_run(0, 0, nomal_run.accel, nomal_run.vel_search);
	wait_time(2000);

//	nomal_rotation.accel=1000.0;
//	nomal_rotation.vel_search=700.0;
//	ui_led_3bit(1);
//	go_entrance(nomal_run.accel, nomal_run.vel_search);
//	ui_led_3bit(3);
//	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
//			nomal_run.vel_search);
//	wait_rotation();
//	ui_led_3bit(7);
//	set_straight(90.0, nomal_run.accel, nomal_run.vel_search,
//			nomal_run.vel_search, 0.0);
//	wait_straight();

}

void mode_1(void) {
//	uint16_t i, j;
	moter_flag = 1;
	rotation_gain.Kp = 0.35;
	rotation_gain.Ki = 0.035;

	nomal_rotation.accel = 7000.0;
	nomal_rotation.vel_search = 600.0;
	ui_led_3bit(1);
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	ui_led_3bit(3);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
			nomal_run.vel_search);
	wait_rotation();
	ui_led_3bit(7);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search,
			nomal_run.vel_search, 0.0);
	wait_straight();

}
void mode_2(void) {
	moter_flag = 1;
	rotation_gain.Kp = 0.38;
	rotation_gain.Ki = 0.035;
	nomal_rotation.accel = 7000.0;
	nomal_rotation.vel_search = 600.0;
	ui_led_3bit(1);
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	ui_led_3bit(3);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
			nomal_run.vel_search);
	wait_rotation();
	ui_led_3bit(7);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search,
			nomal_run.vel_search, 0.0);
	wait_straight();
}

void mode_3(void) {
	moter_flag = 1;
	rotation_gain.Kp = 0.35;
	rotation_gain.Ki = 0.03;
	nomal_rotation.accel = 7000.0;
	nomal_rotation.vel_search = 600.0;
	ui_led_3bit(1);
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	ui_led_3bit(3);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
			nomal_run.vel_search);
	wait_rotation();
	ui_led_3bit(7);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search,
			nomal_run.vel_search, 0.0);
	wait_straight();

}

void mode_4(void) {
	moter_flag = 1;
	go_center(nomal_run.accel, nomal_run.vel_search);
}

void mode_5(void) {
	moter_flag = 1;
	rotation_gain.Kp = 0.50;
	rotation_gain.Ki = 0.03; //36
	nomal_rotation.accel = 7000.0;
	nomal_rotation.vel_search = 600.0;
	set_rotation(-360.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	moter_flag = 0;
	while (SWITCH == 1) {

	}
	log_output();
}

void mode_6(void) {
	moter_flag = 1;
	rotation_gain.Kp = 0.52; //54
	rotation_gain.Ki = 0.027; //36
	nomal_rotation.accel = 7000.0;
	nomal_rotation.vel_search = 600.0;
	set_rotation(-360.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	moter_flag = 0;
	while (SWITCH == 1) {

	}
	log_output();
}

void mode_7(void) {
//	int i = 0;
	while (1) {
//		real_angle_control();
//	myprintf("%.2f\n", rotation_real.velocity);
//		ui_led_3bit(i);
//		i++;
//		if(i>=8){
//			i=0;
//		}
//		wait_time(1);
		AD_SEN();
		myprintf("L:%3d,LF:%3d,RF:%3d,R:%3d\n", SEN_L.now, SEN_LF.now,
				SEN_RF.now, SEN_R.now);
		wait_time(10);
//		myprintf("%.8f\n",rotation_real.velocity);

	}
}

void go_mode(uint8_t mode) {
	mode_flag = mode_flag | 0x80;
	Battery_Check();
	wait_time(1000);
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;
	translation_ideal.dis = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.dis = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.back_rightturn_flag = 0;
	rotation_deviation.now = 0.0;
	rotation_deviation.cumulative = 0.0;
	if (mode == 0) {
		mode_0();
	} else if (mode == 1) {
		for (i = 0; i < 1; i++) {
			ui_led_3bit(1);
			wait_time(300);
			ui_led_3bit(0);
			wait_time(300);
		}
		mode_1();
	} else if (mode == 2) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(2);
			wait_time(300);
			ui_led_3bit(0);
			wait_time(300);
		}
		mode_2();
	} else if (mode == 3) {
		for (i = 0; i < 3; i++) {
			ui_led_3bit(3);
			wait_time(300);
			ui_led_3bit(0);
			wait_time(300);
		}
		mode_3();
	} else if (mode == 4) {
		for (i = 0; i < 4; i++) {
			ui_led_3bit(4);
			wait_time(200);
			ui_led_3bit(0);
			wait_time(200);
		}
		mode_4();
	} else if (mode == 5) {
		for (i = 0; i < 5; i++) {
			ui_led_3bit(5);
			wait_time(200);
			ui_led_3bit(0);
			wait_time(200);
		}
		mode_5();
	} else if (mode == 6) {
		for (i = 0; i < 6; i++) {
			ui_led_3bit(6);
			wait_time(150);
			ui_led_3bit(0);
			wait_time(150);
		}
		mode_6();
	} else if (mode == 7) {
		for (i = 0; i < 7; i++) {
			ui_led_3bit(7);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_7();
	}
	ui_reset();
	wait_time(200);
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;
	duty.left = 0;
	duty.right = 0;
	duty_to_moter();
	mode_flag = mode_flag & 0x7f;
	moter_flag = 0;
}

//	walldate_real.column[0] = 65535;
//	walldate_real.column[1] = 1;
//	walldate_real.column[2] = 8;
//	walldate_real.column[3] = 4;
//	walldate_real.column[4] = 6;
//	walldate_real.column[5] = 2;
//	walldate_real.column[6] = 14;
//	walldate_real.row[1] = 44;
//	walldate_real.row[2] = 5;
//	walldate_real.row[3] = 30;
//	walldate_real.row[4] = 63;
//	walldate_real.row[5] = 0;
//	walldate_real.row[6] = 0;
//
//	walldate_adachi.column[0] = 65535;
//	walldate_adachi.column[1] = 1;
//	walldate_adachi.column[2] = 8;
//	walldate_adachi.column[3] = 4;
//	walldate_adachi.column[4] = 6;
//	walldate_adachi.column[5] = 2;
//	walldate_adachi.column[6] = 14;
//	walldate_adachi.row[1] = 44;
//	walldate_adachi.row[2] = 5;
//	walldate_adachi.row[3] = 30;
//	walldate_adachi.row[4] = 63;
//	walldate_adachi.row[5] = 0;
//	walldate_adachi.row[6] = 0;
