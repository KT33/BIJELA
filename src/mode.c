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

	right_hand(nomal_run.accel,nomal_run.vel_search);

//	set_straight(180, nomal_run.accel, 300.0, 0.0, 0.0);
//	wait_straight();
//	set_rotation(180, nomal_rotation.accel, nomal_rotation.vel_search);
//	wait_rotation();
//	LEFTFRONT = 1;
//	for (i = 0; i < LogMax; i++) {
//		myprintf("%.2f\n", log[i]);
//	}

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
	set_straight(141.4, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
	wait_straight();
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search, nomal_run.vel_search, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,0.0);
	wait_rotation();
	wait_time(100);
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
	wait_straight();
	set_straight(90.0, nomal_run.accel, nomal_run.vel_search, nomal_run.vel_search, 0.0);
	wait_straight();

//	while (1) {
//		myprintf("%.2f\n",translation_ideal.velocity);
//	}
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
	set_rotation(180, 700, 350, 0.0);
	wait_rotation();
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

