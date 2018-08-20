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

void mode_0(void) {
	uint8_t flag;
	x.goal = 7;
	y.goal = 7;
	right_hand(nomal_run.accel, nomal_run.vel_search);
	while (SWITCH == 1) {

	}
	adachi_map(2, 3, walldate_real);
	flag = how_to_move(direction, x.now, y.now);
	output_Walldate(&walldate_real);
	myprintf("\n");
	myprintf("%d\n", flag);
//	output_Walldate(&walldate_checked);
//	myprintf("\n");
//	output_Walldate(&walldate_adachi);
//	myprintf("\n");

}

void mode_1(void) {
	adachi_search_run(2, 3, nomal_run.accel, nomal_run.vel_search);
	while (SWITCH == 1) {

	}
	adachi_map(2, 3, walldate_real);
	flag = how_to_move(direction, x.now, y.now);
	output_Walldate(&walldate_real);
	myprintf("\n");
	myprintf("%d\n", flag);
	myprintf("\n");
	myprintf("getwall(%d,%d,East+%d)%4,walldate_real)=%d\n", x.now, y.now,
			direction,
			getWall(x.now, y.now, (West + direction) % 4, &walldate_real));
}
void mode_2(void) {
//	diameter = 23.75;
	right_real.dis = 0.0;
	set_straight(90, nomal_run.accel, 300.0, 0.0, 0.0);
	wait_straight();

//	while(1){
//		myprintf("%d,%d\n",test1,test2);
//	}
}

void mode_3(void) {
//	wall_control_flag=0;
	right_real.dis = 0.0;
	set_straight(180 * 3, nomal_run.accel, 300.0, 0.0, 0.0);
	wait_straight();

}

void mode_4(void) {

}

void mode_5(void) {
	wall_control_flag = 1;
	wall_cntrol_gain.Kp = 0.01;
	set_straight(180 * 3, nomal_run.accel, nomal_run.vel_max, 0, 0);
	wait_straight();
}

void mode_6(void) {
	adachi_map(7, 7, walldate_real);
	output_Walldate(&walldate_real);
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

		myprintf("L:%3d,LF:%3d,RF:%3d,R:%3d\n", SEN_L.now, SEN_LF.now,
				SEN_RF.now, SEN_R.now);
		wait_time(100);
	}
}

void go_mode(uint8_t mode) {
	mode_flag = mode_flag | 0x80;
	Battery_Check();
	wait_time(1000);
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;
	translation_ideal.dis = 0.0;
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
}

