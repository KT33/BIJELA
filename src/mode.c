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

void mode_0(void) {
	set_rotation(180, nomal_rotation.accel, nomal_rotation.vel_search);
	wait_rotation();
	LEFTFRONT = 1;
	for (i = 0; i < LogMax; i++) {
		myprintf("%.2f\n", log[i]);
	}

//	set_straight(-180, nomal_run.accel, 400.0, nomal_run.vel_min,
//			nomal_run.vel_min);
//	wait_straight();
//	LEFTFRONT = 1;
//	for (i = 0; i < LogMax; i++) {
//		myprintf("%.2f\n", log[i]);
//	}

//	translation_parameter.run_flag =0;
//	duty.left=10;

}

void mode_1(void) {
//	rotation_gain.Ki = 0.0;
//	rotation_gain.Kp=0.0;
//	run_gain.Ki=0.0;
//	run_gain.Kp=0.1;
	rotation_deviation.cumulative = 0;
	rotation_real.dis = 0.0;
	rotation_real.velocity = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.run_flag = 1;
	test_flag = 1;
	while (1) {
		myprintf("%d\n",duty.left);
	}
}

void mode_2(void) {
//	rotation_gain.Ki = 0.005;
	rotation_deviation.cumulative = 0;
	rotation_real.dis = 0.0;
	rotation_real.velocity = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.run_flag = 1;

	test_flag = 1;
	while (1) {
		myprintf("%d\n", duty.left);
	}
}

void mode_3(void) {
	rotation_gain.Ki = 0.1;
	rotation_deviation.cumulative = 0;
	rotation_real.dis = 0.0;
	rotation_real.velocity = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.run_flag = 1;
	test_flag = 1;
	while (1) {
		myprintf("%d\n", duty.left);
	}
}

void mode_4(void) {
	rotation_gain.Kp = 1.8;
	rotation_real.dis = 0.0;
	rotation_real.velocity = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.run_flag = 1;
	test_flag = 1;
	while (1) {
		myprintf("%6.2f\n", rotation_real.velocity);
	}
}

void mode_5(void) {

}

void mode_6(void) {

}

void mode_7(void) {
	Moter_L_BACK = 0;
	Moter_L_FRONT = 0;
	Moter_R_BACK = 0;
	Moter_R_FRONT = 0;
	while (1)
		;
}

void go_mode(uint8_t mode) {
	mode_flag = mode_flag | 0x80;
	Battery_Check();
	wait_time(1000);
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
	mode_flag = mode_flag & 0x7f;
}

