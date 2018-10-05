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
	adachi_search_run_known(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();

	adachi_search_run_known(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();
	wait_time(100);

//	ui_led_3bit(1);
//	make_pass(x.goal, y.goal, 4, 1);
//	ui_led_3bit(3);
//	move_pass_oblique(nomal_run.accel, 2200.0, 1000.0, nomal_run.accel, 2200);
////	adachi_search_run(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
//	write_all_walldatas();
//	wait_time(1000);

}

void mode_1(void) {

	read_all_walldatas();
	start_SEN(1);
	make_pass(x.goal, y.goal, 4, 1);
//	output_Walldate(&walldate_adachi);
	move_pass_oblique(7000.0, 2500.0, 1000.0, nomal_run.accel, 2200);
	adachi_search_run(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();
	wait_time(100);
}

void mode_2(void) {
	read_all_walldatas();
	start_SEN(0);
	adachi_search_run_known(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();

	adachi_search_run_known(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();
	wait_time(100);
//	adachi_search_run(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
//	write_all_walldatas();
//	wait_time(100);

}

void mode_3(void) {
	read_all_walldatas();
	start_SEN(1);
	make_pass(x.goal, y.goal, 4, 0);
//	output_Walldate(&walldate_adachi);
	move_pass_oblique(7000.0, 2500.0, 1000.0, nomal_run.accel, 2200);
	adachi_search_run(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();
	wait_time(100);

}

void mode_4(void) {

//	//127.28
	start_SEN(0);
	go_entrance(nomal_run.accel, 1000.0);
	set_straight(90.0, 6500.0, 1000.0, 1000.0, 1000.0);
	wait_straight();
	wall_control_oblique_flag = 2;
	turn_left_90_big(1000.0);
	set_straight(270.0, nomal_run.accel, 1000.0, 1000.0, 0.0);
	wait_straight();
//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
////	myprintf("%.3f\n", Log[0]);
//	log_output();
}

void mode_5(void) { //nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search
	start_SEN(0);
	adachi_search_run_known(x.goal, y.goal, 4, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();

	adachi_search_run_known(0, 0, 1, nomal_run.accel, nomal_run.vel_search, 1, 1);
	write_all_walldatas();
	wait_time(100);
}

void mode_6(void) {
	read_all_walldatas();
	start_SEN(1);
	make_pass(x.goal, y.goal, 4, 1);
//	output_Walldate(&walldate_adachi);
	move_pass_big_turn(nomal_run.accel + 200.0, nomal_run.vel_max + 500.0,
			1000.0);

//	uint8_t i;
//	moter_flag = 1;
//	log_start();
//	go_entrance(nomal_run.accel, nomal_run.vel_search);
//	for (i = 0; i < 1; i++) {
//		pass_180(nomal_run.accel, nomal_run.vel_search);
//	}
//	stop90(nomal_run.accel, nomal_run.vel_search);
//	while (SWITCH == 1) {
//		moter_flag = 0;
//	}
//	log_output();

}

void mode_7(void) {
	read_all_walldatas();
	make_pass(x.goal, y.goal, 4, 1);
	output_Walldate(&walldate_adachi);
	adachi_map(x.goal, y.goal, 4, walldate_real);
	output_Walldate(&walldate_real);
	output_Walldate(&walldate_checked);

//	output_SEN();
//
//	while (1) {
//		//	moter_flag = 1;
//		wait_time(1);
//		real_angle_control();
//		myprintf("%.4f\n", rotation_real.velocity);
//////		ui_led_3bit(i);
//////		i++;
//////		if(i>=8){
//////			i=0;
//	}
//////		wait_time(1);

////		myprintf("%.8f\n", rotation_real.velocity);
//////		moter_flag = 1;
//////		myprintf("%.3f\n", rotation_deviation.cumulative);
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
	x.now = 0;
	y.now = 0;
	direction = 0;
	if (failsafe_flag == 1) {
		set_straight(180.0, nomal_run.accel * 2, left_real.velocity,
				left_real.velocity, 0.0);
		wait_straight();
	}
	mode_flag = mode_flag & 0x7f;
	moter_flag = 0;
	failsafe_flag = 0;
	SEN_check_flag = 0;
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
