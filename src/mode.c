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
	wall_cntrol_gain.Kp = 0.5;
	wall_cntrol_gain.Kd = 0.4;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
	wait_time(100);

//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);
//	make_pass(0, 0, 1, 0);
//	out_put_pass(pass);
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);

	make_pass(0, 0, 1, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
	wait_time(10);
}

void mode_2(void) {
	wall_cntrol_gain.Kp = 0.5;
	wall_cntrol_gain.Kd = 0.4;
	para_mode();
	read_all_walldatas();
	start_SEN(1);
	wait_time(30);
	make_pass(x.goal, y.goal, 4, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1300,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
	wait_time(100);

//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);
//	make_pass(0, 0, 1, 0);
//	output_Walldate(&walldate_adachi);
//	out_put_pass(pass);

	make_pass(0, 0, 1, 0);
	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
			nomal_oblique.accel, nomal_oblique.vel_max, 1);
	fan_off();
	wait_time(10);

}

void mode_3(void) { //253.558
//	rotation_gain.Kp = 0.67;
//	rotation_gain.Ki = 0.0025;
	start_SEN(0);
	fan_on();
	set_straight(253.558 - 20.0, 7000.0, 1200.0, 0.0, 1200.0);
	wall_control_flag = 0;
	log_start();
	wait_straight();
//	slalom_right90(nomal_run.accel, nomal_run.vel_search);
	turn_right_v90(1200);
	set_straight(253.558 + 20.0, 7000.0, 1200.0, 1200.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	fan_off();
	while (SWITCH == 1) {
		moter_flag = 0;
	}
}

void mode_4(void) {
	rotation_gain.Kp = 0.50;
	rotation_gain.Ki = 0.001;

	oblique_Front_gain = 0;
	oblique_Side_gain = 0;
	start_SEN(0);
	fan_on();
	set_straight(253.558 - 20.0, 7000.0, 1200.0, 0.0, 1200.0);
	wall_control_flag = 0;
	log_start();
	wait_straight();
//	slalom_right90(nomal_run.accel, nomal_run.vel_search);
	turn_left_45_out(1200.0);
	set_straight(180.0, 7000.0, 1200.0, 1200.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	fan_off();
	while (SWITCH == 1) {
		moter_flag = 0;
	}
	log_output();
}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search
	start_SEN(0);
	wall_cntrol_gain.Kp = 0.1;
	wall_cntrol_gain.Kd = 0.1;
//	fan_on();
	wait_time(100);
	set_straight(142.0 + 180.0 * 6.0 + 90.0, 7000.0, 600.0, 0.0, 0.0);
	log_start();
	wait_straight();
//	fan_off();
	set_rotation(183.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	log_flag=0;
	wait_time(50);
	back_100();
//	wall_cntrol_gain.Kp = 0.1;
//	wall_cntrol_gain.Kd = 0.1;
	rotation_deviation.cumulative = 0.0;
//	fan_on();
	wait_time(100);
	set_straight(142.0 + 180.0 * 6.0 + 90.0, 7000.0, 600.0, 0.0, 0.0);
	wait_straight();
//	fan_off();
	while (SWITCH == 1) {
		moter_flag = 0;
	}
	log_output();

}

void mode_6(void) {
	uint8_t flag;
	uint16_t test;
	uint8_t dire, x_x, y_y;
//	wall_cntrol_gain.Kp = 0.5;
//	wall_cntrol_gain.Kd = 0.1;
//	para_mode();
//	read_all_walldatas();
//	start_SEN(1);
//	wait_time(30);
//	make_pass(x.goal, y.goal, 4, 1);
//	move_pass_oblique(nomal_run.accel, nomal_run.vel_max, 1200,
//			nomal_oblique.accel, nomal_oblique.vel_max, 1);
//	fan_off();
//	wait_time(100);

//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);
//	make_pass(0, 0, 1, 0);
//	out_put_pass(pass);
//	myprintf("x:%d,y:%d,dire:%d\n", x.now, y.now, direction);

//	make_pass(0, 0, 1, 0);
//	move_pass_big_turn(7000.0, 2200.0, 1000.0);
//	wait_time(10);

//	x.now=2;
//	y.now=3;
//	direction=3;
	read_all_walldatas();
	adachi_map(7, 7, 4, walldate_real);
	flag = how_to_move_search_known(3, 2, 15);
	myprintf("%d\n", flag);
	output_Walldate(&walldate_real);
	output_Walldate(&walldate_checked);
	//out_put_pass(pass);
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
