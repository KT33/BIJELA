/*
 * pass.c
 *
 *  Created on: 2018/08/24
 *      Author: takao keisuke
 */

#include "pass.h"
#include "iodefine.h"
#include "run.h"
#include "adachi.h"
#include "walldate.h"
#include "other.h"

void make_pass(uint8_t goal_x, uint8_t goal_y) {
	uint8_t i = 0, j = 0, straight_count = 0;
	uint8_t flag;
	for (i = 0; i < 255; i++) {
		pass[i] = 0x0;
		pass_compression[i] = 0x0;
	}
	x.pass = x.now;
	y.pass = y.now;
	direction_pass = direction;
	adachi_map(goal_x, goal_y, walldate_adachi);
	coordinate_pass();
	i = 0;
	while (1) {

		if (x.pass == goal_x && y.pass == goal_y) {
			direction_pass += 2;
			pass[i] = 0xff;
			break;
		}

		flag = how_to_move(direction_pass, (int8_t) x.pass, (int8_t) y.pass,
				walldate_adachi);
		myprintf("flag:%d,%d,%d\n", flag, x.pass, y.pass);
		if (flag == 0) {
			pass[i] = 0;
		}
		if (flag == 1) {
			direction_pass += 1;
			pass[i] = 1;
		}
		if (flag == 3) {
			direction_pass += 3;
			pass[i] = 3;
		}
		if (flag == 2 || flag == 4) {
			direction_pass += 2;
			pass[i] = 2;
		}

		if (direction_pass > 3) {
			direction_pass -= 4;
		}
		coordinate_pass();
		i++;
	}

	j = 0;
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 0) {
			straight_count++;
		} else if (straight_count != 0) {
			pass_compression[j] = straight_count;
			straight_count = 0;
			j++;
			pass_compression[j] = pass[i] * 20;
			j++;
		} else {
			pass_compression[j] = pass[i] * 20;
			j++;
		}
	}
	if (straight_count != 0) {
		pass_compression[j] = straight_count;
		j++;
	}
	pass_compression[j] = 0xff;
}

void move_pass_compression(float accel, float vel) {
	uint8_t i = 0;
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 0) {
			pass_180(accel, vel);
		} else if (pass[i] == 1) {
			slalom_left90(accel, vel, nomal_rotation.accel,
					nomal_rotation.vel_search);
		} else if (pass[i] == 3) {
			slalom_right90(accel, vel, nomal_rotation.accel,
					nomal_rotation.vel_search);
		}
	}
	non_ketuate_goal(accel, vel);
}

void move_pass(float accel, float vel) {
	uint8_t i = 0;
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	for (i = 0; pass_compression[i] != 0xff; i++) {
		if (pass_compression[i] < 20) {
			set_straight(180.0 * pass_compression[i], accel, vel,
					nomal_run.vel_search, nomal_run.vel_search);
			wait_straight();
		} else if (pass_compression[i] == 20) {
			slalom_left90(nomal_run.accel, nomal_run.vel_search,
					nomal_rotation.accel, nomal_rotation.vel_search);
		} else if (pass_compression[i] == 60) {
			slalom_right90(nomal_run.accel, nomal_run.vel_search,
					nomal_rotation.accel, nomal_rotation.vel_search);
		}
	}
	non_ketuate_goal(nomal_run.accel, nomal_run.vel_search);
}
