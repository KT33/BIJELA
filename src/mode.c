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
	wall_cntrol_gain.Kp = 0.1;
	wall_cntrol_gain.Kd = 0.1;
	nomal_run.vel_search = 600.0;
	start_SEN(0);
	search_run_special(x.goal, y.goal, 4);
	if (u_turn_counter == 100) {
		write_all_walldatas(255);
	} else {
		write_all_walldatas(0);
	}
}

void mode_1(void) {
	oblique_Front_gain = 0.3;
	oblique_Side_gain = 0.02;
	wall_cntrol_gain.Kp = 0.4;
	wall_cntrol_gain.Kd = 0.15;
	oblique_offset_front = 3.0;
	oblique_offset_side = 6.0;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
}

void mode_2(void) {
	oblique_Front_gain = 0.3;
	oblique_Side_gain = 0.02;
	wall_cntrol_gain.Kp = 0.4;
	wall_cntrol_gain.Kd = 0.15;
	oblique_offset_front = 3.0;
	oblique_offset_side = 6.0;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1300.0,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
}

void mode_3(void) { //253.558
	oblique_Front_gain = 0.3;
	oblique_Side_gain = 0.02;
	wall_cntrol_gain.Kp = 0.4;
	wall_cntrol_gain.Kd = 0.15;
	oblique_offset_front = 3.0;
	oblique_offset_side = 6.0;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1000.0,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
}

void mode_4(void) {
	oblique_Front_gain = 0.3;
	oblique_Side_gain = 0.02;
	wall_cntrol_gain.Kp = 0.2;
	wall_cntrol_gain.Kd = 0.1;
	oblique_offset_front = 3.0;
	oblique_offset_side = 6.0;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search
	oblique_Front_gain = 0.3;
	oblique_Side_gain = 0.02;
	wall_cntrol_gain.Kp = 0.4;
	wall_cntrol_gain.Kd = 0.15;
	oblique_offset_front = 3.0;
	oblique_offset_side = 6.0;
	para_mode();

	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 1);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
}

void mode_6(void) {
	read_all_walldatas();
	wall_cntrol_gain.Kp = 0.1;
	wall_cntrol_gain.Kd = 0.1;
	nomal_run.vel_search = 600.0;
	start_SEN(0);
	search_run_special(x.goal, y.goal, 4);
	if (u_turn_counter == 100) {
		write_all_walldatas(255);
	} else {
		write_all_walldatas(0);
	}

//	nomal_run.accel = 15000.0;
//	nomal_run.vel_max = 3500;
//	start_SEN(0);
//	fan_on();
//	set_straight(142.0 + 180.0*14.0, 15000.0, 3500.0, 0.0, 0.0);
////	wall_control_flag = 0;
//	log_start();
//	wait_straight();
////	slalom_right90(nomal_run.accel, nomal_run.vel_search);
////	turn_left_180_big(1200);
////	set_straight(180.0, 7000.0, 1200.0, 1200.0, 0.0);
////	wall_control_flag = 0;
////	wait_straight();
//	fan_off();
}

void mode_7(void) {
//	output_SEN();

	fan_on();

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
	run_left_deviation.cumulative = 0.0;
	run_right_deviation.cumulative = 0.0;
	x.now = 0;
	y.now = 0;
	direction = 0;
	if (mode == 0) {
		mode_0();
	} else if (mode == 1) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_1();
	} else if (mode == 2) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_2();
	} else if (mode == 3) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_3();
	} else if (mode == 4) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_4();
	} else if (mode == 5) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_5();
	} else if (mode == 6) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
			wait_time(100);
			ui_led_3bit(0);
			wait_time(100);
		}
		mode_6();
	} else if (mode == 7) {
		for (i = 0; i < 2; i++) {
			ui_led_3bit(mode);
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
