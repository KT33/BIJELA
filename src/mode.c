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
#include "dataflash.h"
#include "adachi.h"
#include "motion.h"

void mode_0(void) {
//	start_SEN();
//	uint8_t i;
	start_SEN(0);
	search_run_special(x.goal, y.goal, 4);
	write_all_walldatas();
}

void mode_1(void) {
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
	wait_time(500);

//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);
//	make_pass(0, 0, 1, 0);
//	out_put_pass(pass);
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);

	make_pass(0, 0, 1, 0);
	move_pass_big_turn(7000.0, 2200.0, 1000.0);
	wait_time(10);
}

void mode_2(void) {
	para_mode();
	read_all_walldatas();
	start_SEN(1);
//	fan_on();
	make_pass(x.goal, y.goal, 4, 0);
//	output_Walldate(&walldate_adachi);
//	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1000.0,
//			nomal_oblique.accel, nomal_oblique.vel_max, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1000,
			nomal_oblique.accel, nomal_oblique.vel_max, 0);
	fan_off();
	wait_time(2000);
	adachi_map(0, 0, 1, walldate_adachi);
	move_pass_big_turn(7000.0, 2200.0, 1000.0);
	wait_time(10);

}

void mode_3(void) { //253.558
	para_mode();
	read_all_walldatas();
	start_SEN(1);
//	fan_on();
	make_pass(x.goal, y.goal, 4, 0);
//	output_Walldate(&walldate_adachi);
//	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1000.0,
//			nomal_oblique.accel, nomal_oblique.vel_max, 0);
	move_pass_big_turn(nomal_run.accel, nomal_run.vel_max, 1000.0);
	wait_time(2000);
	adachi_search_run_known(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1,
			0);
	write_all_walldatas();
	fan_off();
	wait_time(10);
}

void mode_4(void) {
	start_SEN(0);
	fan_on();
	set_straight(140.0 + 90.0, 7000.0, 1300.0, 0.0, 1300.0);
	wait_straight();
	turn_right_45_in(1300.0);
	set_straight(253.558+20.0, 7000.0, 1300.0, 1300.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	fan_off();
	while(SWITCH==1){
		moter_flag=0;
	}
	myprintf("%.2f\n",Log[0]);

}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search
	start_SEN(0);
	fan_on();
	set_straight(140.0 + 90.0, 7000.0, 1300.0, 0.0, 1300.0);
	wait_straight();
	turn_right_135_in(1300.0);
	set_straight(253.558+20.0, 7000.0, 1300.0, 1300.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	fan_off();
	while(SWITCH==1){
		moter_flag=0;
	}
	myprintf("%.2f\n",Log[0]);
}


void mode_6(void) {
	read_all_walldatas();
	adachi_map_special(x.goal,y.goal, 4, walldate_real);
	output_Walldate(&walldate_real);
}

void mode_7(void) {
	output_SEN();

//	fan_on();

//	out_put_pass(pass);
//	while (SWITCH == 1) {
//
//	}
}

void go_mode(uint8_t mode) {
	mode_flag = mode_flag | 0x80;
	Battery_Check();
	failsafe_flag = 0;
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
	wallcontrol_value = 0.0;
	run_left_deviation.cumulative=0.0;
	run_right_deviation.cumulative=0.0;
	x.now = 0;
	y.now = 0;
	direction = 0;
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

	wait_time(200);
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;

	duty.left = 0;
	duty.right = 0;
	duty_to_moter();
	x.now = 0;
	y.now = 0;
	direction = 0;
	if (failsafe_flag == 1) {
		while (failsafe_counter < 1000) {
		}
	}
	mode_flag = mode_flag & 0x7f;
	moter_flag = 0;
	failsafe_flag = 0;
	SEN_check_flag = 0;
	Moter_Stby = 1;
	ui_reset();
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
