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

void mode_0(void) {
	Battery_Check();

	wait_time(1000);
	set_straight(180*2, nomal_run.accel, 600.0, nomal_run.vel_min,
			nomal_run.vel_min);
	wait_straight();
	LEFTFRONT = 1;
	for (i = 0; i < LogMax; i++) {
		myprintf("%.2f\n", log[i]);
	}
}

void mode_1(void) {
	Battery_Check();
	for (i = 0; i < 1; i++) {
		ui_led_3bit(1);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

void mode_2(void) {
	Battery_Check();
	for (i = 0; i < 2; i++) {
		ui_led_3bit(2);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

void mode_3(void) {
	Battery_Check();
	for (i = 0; i < 3; i++) {
		ui_led_3bit(3);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

void mode_4(void) {
	Battery_Check();
	for (i = 0; i < 4; i++) {
		ui_led_3bit(4);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}

}

void mode_5(void) {
	Battery_Check();
	for (i = 0; i < 5; i++) {
		ui_led_3bit(5);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

void mode_6(void) {
	Battery_Check();
	for (i = 0; i < 6; i++) {
		ui_led_3bit(6);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

void mode_7(void) {
	Battery_Check();
	for (i = 0; i < 7; i++) {
		ui_led_3bit(7);
		wait_time(300);
		ui_led_3bit(0);
		wait_time(300);
	}
}

