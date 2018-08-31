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

void make_pass(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		uint8_t straight_flag) {
	uint8_t i = 0, j = 0, straight_count = 0;
	uint8_t flag;
	straight_flag=0;

	for (i = 0; i < 255; i++) {
		pass[i] = 0x0;
		pass_compression[i] = 0x0;
	}
	x.pass = x.now;
	y.pass = y.now;
	direction_pass = direction;
	if (straight_flag == 0) {
		adachi_map(goal_x, goal_y, goal_scale, walldate_adachi);
	} else if (straight_flag == 1) {
		adachi_map_straight(goal_x, goal_y, goal_scale, walldate_adachi);
	}
	coordinate_pass();
	i = 0;
	while (1) {

		if (step_map[x.pass][y.pass] == 0) {
			direction_pass += 2;
			pass[i] = 0xff;
			break;
		}

		flag = how_to_move(direction_pass, (int8_t) x.pass, (int8_t) y.pass,
				walldate_adachi);
//		myprintf("flag:%d,%d,%d\n", flag, x.pass, y.pass);
		if (flag == 0) {
			pass[i] = 0; //直進
		}
		if (flag == 1) {
			direction_pass += 1;
			pass[i] = 1; //右折
		}
		if (flag == 3) {
			direction_pass += 3;
			pass[i] = 3; //左折
		}
		if (flag == 2 || flag == 4) {
			direction_pass += 2;
			pass[i] = 2; //ターン(使わない)
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
			pass_compression[j] = straight_count * 2;
			j++;
			straight_count = 0;
			if (pass[i] == 1) {
				pass_compression[j] = 40;
			} else if (pass[i] == 3) {
				pass_compression[j] = 50;
			}
			j++;
		} else {
			if (pass[i] == 1) {
				pass_compression[j] = 40;
			} else if (pass[i] == 3) {
				pass_compression[j] = 50;
			}
			j++;
		}
	}
	straight_count++;
	if (straight_count != 0) {
		pass_compression[j] = straight_count * 2;
		j++;
		straight_count = 0;
	}
	pass_compression[j] = 0xff;
}

void move_pass(float accel, float vel) {
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

void move_pass_compression(float accel, float vel) {
	uint8_t i = 0, j, first_turn_flag = 0;
	if (pass[i] == 0) {
		first_turn_flag = 0;
	} else {
		first_turn_flag = 1;
	}
	for (i = 0; pass_compression[i] != 0xff; i++) {
		if (pass_compression[i] < 35) { //直進の途中
			if (first_turn_flag == 0) {
				set_straight(90.0 * pass_compression[i] + 140.0, accel, vel,
						0.0, nomal_run.vel_search);
				wait_straight();
				first_turn_flag = 0xff;
				for (j = 0; j < 1 + (pass_compression[i] / 2); j++) {
					coordinate();
				}
			} else {
				set_straight(90.0 * pass_compression[i], accel, vel,
						nomal_run.vel_search, nomal_run.vel_search);
				wait_straight();
				for (j = 0; j < pass_compression[i] / 2; j++) {
					coordinate();
				}
			}
		} else if (pass_compression[i] == 40) { //左折
			if (first_turn_flag == 1) {
				go_entrance(accel, nomal_run.vel_search);
				first_turn_flag = 0xff;
				coordinate();
			}
			if (nomal_run.vel_search == 600.0) {
				slalom_left90_600(nomal_run.accel, nomal_run.vel_search, 9000.0, 750.0);
			} else {
				slalom_left90(nomal_run.accel, nomal_run.vel_search,
						nomal_rotation.accel, nomal_rotation.vel_search);
			}
			coordinate();
		} else if (pass_compression[i] == 50) { //右折
			if (first_turn_flag == 1) {
				go_entrance(accel, nomal_run.vel_search);
				first_turn_flag = 0xff;
				coordinate();
			}
			if (nomal_run.vel_search == 600.0) {
				slalom_right90_600(nomal_run.accel, nomal_run.vel_search, 9000.0, 750.0);
			} else {
				slalom_right90(nomal_run.accel, nomal_run.vel_search,
						nomal_rotation.accel, nomal_rotation.vel_search);
			}
			coordinate();
		}
	}

	if (getWall(x.now, y.now, direction + 1, &walldate_real)) {
		set_straight(95.0, accel, nomal_run.vel_search, nomal_run.vel_search,
				0.0);
		wait_straight();
		wait_time(50);
		set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
				0.0);
		wait_rotation();
		wait_time(50);
		back_100();
		wait_time(50);
		rotation_deviation.cumulative = 0.0;
	} else if (getWall(x.now, y.now, direction + 3, &walldate_real)) {
		set_straight(93.0, accel, nomal_run.vel_search, nomal_run.vel_search,
				0.0);
		wait_straight();
		wait_time(50);
		set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
				0.0);
		wait_rotation();
		wait_time(50);
		back_100();
		wait_time(50);
		rotation_deviation.cumulative = 0.0;
	} else {
		set_straight(93.0, accel, nomal_run.vel_search, nomal_run.vel_search,
				0.0);
		wait_straight();
		wait_time(50);
		set_rotation(-180.0, nomal_rotation.accel, nomal_rotation.vel_search,
				0.0);
		wait_rotation();
		wait_time(50);
		set_straight(-50.0, 500, 150, 0.0, 0.0);
		wall_control_flag = 0;
		wait_straight();
		wait_time(50);
	}
}
