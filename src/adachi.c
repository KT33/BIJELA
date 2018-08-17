/*
 * adachi.c
 *
 *  Created on: 2018/08/13
 *      Author: takao keisuke
 */

#include "adachi.h"
#include "iodefine.h"
#include "variable.h"
#include "CMT.h"
#include "run.h"
#include "other.h"

void right_hand(float accel, float vel) {
	int i=0;
	go_entrance(accel, vel);
	coordinate();
	while (1) {
		if (SEN_L.now < SEN_L.threshold) {
			turn_left(accel, vel);
		} else if (SEN_F.now < SEN_F.threshold) {
			pass(accel, vel);
		} else if (SEN_R.now < SEN_R.threshold) {
			turn_right(accel, vel);
		} else {
			ketuate_right(accel, vel);
		}
		coordinate();
		i++;
		if(i==20){
			break;
		}
	}
}

void go_entrance(float accel, float vel) {
	set_straight(140.0, accel, vel, 0.0, vel);
	wait_straight();
}

void go_center(float accel, float vel) {
	set_straight(50.0, accel, vel, 0.0, 0.0);
	wait_straight();
}


void turn_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void turn_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void pass(float accel, float vel) {
	set_straight(180.0, accel, vel, vel, vel);
	wait_straight();
}

void turn_180(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(-180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void ketuate_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	back_100();
	wait_time(100);
	go_center(accel, vel);
	wait_time(100);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	back_100();
	wait_time(100);
	go_entrance(accel, vel);
}

void ketuate_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	back_100();
	wait_time(100);
	go_center(accel, vel);
	wait_time(100);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	back_100();
	wait_time(100);
	go_entrance(accel, vel);
}

void back_100(void) {
	set_straight(-60.0, 1000, 200, 0.0, 0.0);
	wait_straight();
}

