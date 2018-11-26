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
#include "speaker.h"

void make_pass(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		uint8_t straight_flag) {
	uint8_t i = 0, j = 0, straight_count = 0;
	uint8_t flag;
//	straight_flag = 0;

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

//	myprintf("x:%d,y:%d\n",x.pass,y.pass);

		if (step_map[x.pass][y.pass] == 0) {
			break;
		}

		flag = how_to_move_pass(direction_pass, (int8_t) x.pass,
				(int8_t) y.pass, walldate_adachi, pass[i - 1]);
//			myprintf("flag:%d,%d,%d,%d\n", flag, x.pass, y.pass,direction_pass);
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

	if (goal_scale == 4) {
		pass[i] = 0;
		i++;
	}

	direction_pass += 2;
	pass[i] = 0xff;

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
//	if (goal_scale == 4) {
//		straight_count++;
//	}
	if (straight_count != 0) {
		pass_compression[j] = straight_count * 2;
		j++;
		straight_count = 0;
	}
	pass_compression[j] = 0xff;
	if (goal_scale == 1) {
		pass[254] = 1;
	} else {
		pass[254] = 4;
	}
}

void move_pass(float accel, float vel) {
	uint8_t i = 0;
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 0) {
			pass_180(accel, vel);
		} else if (pass[i] == 1) {
			slalom_left90(accel, vel);
		} else if (pass[i] == 3) {
			slalom_right90(accel, vel);
		}
	}
	non_ketuate_goal(accel, vel);
}

void move_pass_compression(float accel, float vel) {
	uint8_t i = 0, j, first_turn_flag = 0;
	moter_flag = 1;
	if (pass[i] == 0) {
		first_turn_flag = 0;
	} else {
		first_turn_flag = 1;
	}
	for (i = 0; pass_compression[i] != 0xff; i++) {
		if (pass_compression[i] < 35) { //直進の途中
			if (first_turn_flag == 0) {
				set_straight(90.0 * (float) pass_compression[i] + 140.0, accel,
						vel, 0.0, nomal_run.vel_search);
				wait_straight();
				first_turn_flag = 0xff;
				for (j = 0; j < 1 + (pass_compression[i] / 2); j++) {
					coordinate();
				}
			} else {
				set_straight(90.0 * (float) pass_compression[i], accel, vel,
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

			slalom_left90(nomal_run.accel, nomal_run.vel_search);

			coordinate();
		} else if (pass_compression[i] == 50) { //右折
			if (first_turn_flag == 1) {
				go_entrance(accel, nomal_run.vel_search);
				first_turn_flag = 0xff;
				coordinate();
			}

			slalom_right90(nomal_run.accel, nomal_run.vel_search);

			coordinate();
		}
	}

	if (pass[254] == 1) {
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
	} else {
		if (getWall(x.now, y.now, direction + 1, &walldate_real)) {
			set_straight(95.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
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
			set_straight(93.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
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
			set_straight(93.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
			wait_straight();
			wait_time(50);
			set_rotation(-180.0, nomal_rotation.accel,
					nomal_rotation.vel_search, 0.0);
			wait_rotation();
			wait_time(50);
			set_straight(-50.0, 500, 150, 0.0, 0.0);
			wall_control_flag = 0;
			wait_straight();
			wait_time(50);
		}
	}
}

#define SHORTLEFT90 40
#define SHORTRIGHT90 50
#define BIGLEFT90 60
#define BIGRIGHT90 70
#define BIGLEFT180 80
#define BIGRIGHT180 90
#define FARST_SHORTRIGHT90 100
#define FARST_BIGRIGHT90 110
#define LEFT45IN 150
#define RIGHT45IN 160
#define LEFT135IN 170
#define RIGHT135IN 180
#define LEFT45OUT 155
#define RIGHT45OUT 165
#define LEFT135OUT 175
#define RIGHT135OUT 185
#define V_90_LEFT 190
#define V_90_RIGHT 195

void move_pass_big_turn(float accel, float max_vel, float big_turn_vel) {
	uint8_t i = 0, j;
	int8_t straight_count;
	uint8_t x_box, y_box, direction_box;

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

	for (i = 0; i < 255; i++) {
		pass_big[i] = 0;
	}

//	x.now = 0;
//	y.now = 0;

	coordinate();
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 1) {
			direction++;
		} else if (pass[i] == 3) {
			direction += 3;
		}
		if (direction > 3) {
			direction -= 4;
		}
		coordinate();
	}

	x_box = x.now;
	y_box = y.now;
	direction_box = direction;

	j = 0;
	straight_count = 0;

	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 0) {
			straight_count += 2;
		} else {
			if (pass[i + 1] == pass[i] && (pass[i + 2] == 0)
					&& pass[i - 1] == 0) {
				if (straight_count > 1) {
					pass_big[j] = straight_count - 1;
					j++;
				}
				straight_count = 0;
				if (pass[i] == 1) {
					pass_big[j] = BIGLEFT180;
				} else if (pass[i] == 3) {
					pass_big[j] = BIGRIGHT180;
				}
				j++;
				i++;
				straight_count = -1;
			} else if ((pass[i + 1] == 0) && (i == 0 || pass[i - 1] == 0)) {
				if (straight_count > 1) {
					pass_big[j] = straight_count - 1;
					j++;
				}
				straight_count = 0;
				if (pass[i] == 1) {
					pass_big[j] = BIGLEFT90;
				} else if (pass[i] == 3) {
					pass_big[j] = BIGRIGHT90;
				}

				straight_count = -1;
				j++;

			} else {
				if (straight_count > 0) {
					pass_big[j] = straight_count;
					j++;
				}
				straight_count = 0;
				if (pass[i] == 1) {
					pass_big[j] = SHORTLEFT90;
				} else if (pass[i] == 3) {
					pass_big[j] = SHORTRIGHT90;
				}
				j++;
			}
		}
	}
//	if (pass[254] == 4) {
//		straight_count += 2;
//	}
	if (straight_count != 0) {
		pass_big[j] = straight_count;
		j++;
		straight_count = 0;
	}
	pass_big[j] = 0xff;

//	while (SWITCH == 1) {
//		moter_flag=0;
//	}
//	out_put_pass(pass_big);
//		while (1)
//			;
//
//	for (i = 0; pass[i] != 0xff; i++) {
//		myprintf("%d\n", pass[i]);
//	}
//	myprintf("\n");
//	for (i = 0; pass_big[i] != 0xff; i++) {
//		myprintf("%d\n", pass_big[i]);
//	}
//
//	while (1)
//		;

	moter_flag = 1;

	i = 0;
	coordinate();
	if (pass_big[i] < 35) {
		if (pass_big[1] == SHORTLEFT90 || pass_big[1] == SHORTRIGHT90) {
			set_straight(140.0 + 90.0 * (float) pass_big[i], accel, max_vel,
					0.0, nomal_run.vel_search);
		} else {
			set_straight(140.0 + 90.0 * (float) pass_big[i], accel, max_vel,
					0.0, big_turn_vel);
		}
		wait_straight();
		for (j = 0; j < pass_big[i] / 2; j++) {
			coordinate();
		}
	} else if (pass_big[i] == SHORTRIGHT90) { //右折
		go_entrance(accel, nomal_run.vel_search);
		slalom_right90(nomal_run.accel, nomal_run.vel_search);
		coordinate();
	} else if (pass_big[i] == BIGRIGHT90) {
		farst_turn_right_90_big(big_turn_vel);
	} else if (pass_big[i] == SHORTLEFT90) {
		go_entrance(accel, nomal_run.vel_search);
		slalom_left90(nomal_run.accel, nomal_run.vel_search);
		coordinate();
	}

	for (i = 1; pass_big[i] != 0xff; i++) {
		if (pass_big[i] < 35) { //直進の途中
			if (pass_big[i - 1] == SHORTLEFT90
					|| pass_big[i - 1] == SHORTRIGHT90) {
				if (pass_big[i + 1] == SHORTLEFT90
						|| pass_big[i + 1] == SHORTRIGHT90
						|| pass_big[i + 1] == 0xff) {
					set_straight(90.0 * (float) pass_big[i], accel, max_vel,
							nomal_run.vel_search, nomal_run.vel_search);
				} else {
					set_straight(90.0 * (float) pass_big[i], accel, max_vel,
							nomal_run.vel_search, big_turn_vel);
				}
			} else {
				if (pass_big[i + 1] == SHORTLEFT90
						|| pass_big[i + 1] == SHORTRIGHT90) {
					set_straight(90.0 * (float) pass_big[i], accel, max_vel,
							big_turn_vel, nomal_run.vel_search);
				} else {
					set_straight(90.0 * (float) pass_big[i], accel, max_vel,
							big_turn_vel, big_turn_vel);
				}
			}
			wait_straight();
			for (j = 0; j < pass_big[i] / 2; j++) {
				coordinate();
			}
		} else if (pass_big[i] == SHORTLEFT90) { //左折
			slalom_left90(nomal_run.accel, nomal_run.vel_search);
			coordinate();
		} else if (pass_big[i] == SHORTRIGHT90) { //右折
			slalom_right90(nomal_run.accel, nomal_run.vel_search);
			coordinate();
		} else if (pass_big[i] == BIGLEFT90) {
			turn_left_90_big(big_turn_vel);
			coordinate();
		} else if (pass_big[i] == BIGRIGHT90) {
			turn_right_90_big(big_turn_vel);
			coordinate();
		} else if (pass_big[i] == BIGLEFT180) {
			turn_left_180_big(big_turn_vel);
			coordinate();
		} else if (pass_big[i] == BIGRIGHT180) {
			turn_right_180_big(big_turn_vel);
			coordinate();
		}
	}

	x.now = x_box;
	y.now = y_box;
	direction = direction_box;

	if (pass[254] == 1) {
		if (pass_big[i - 1] == SHORTLEFT90 || pass_big[i - 1] == SHORTRIGHT90) {
			set_straight(93.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
		} else {
			set_straight(93.0, accel, max_vel, big_turn_vel, 0.0);
		}
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
	} else {
//		coordinate();
		addWall();

		if (pass_big[i - 1] == SHORTLEFT90 || pass_big[i - 1] == SHORTRIGHT90) {
			set_straight(90.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
		} else {
			set_straight(90.0, accel, max_vel, big_turn_vel, 0.0);
		}
		wait_straight();
		wait_time(50);

		if (getWall(x.now, y.now, direction + 1, &walldate_real)) {
			set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
					0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
			rotation_deviation.cumulative = 0.0;
		} else if (getWall(x.now, y.now, direction + 3, &walldate_real)) {
			set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
					0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
			rotation_deviation.cumulative = 0.0;
		} else if (getWall(x.now, y.now, direction, &walldate_real)) {
			set_rotation(-180.0, nomal_rotation.accel,
					nomal_rotation.vel_search, 0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
		} else {
			set_rotation(-180.0, nomal_rotation.accel,
					nomal_rotation.vel_search, 0.0);
			wait_rotation();
			wait_time(50);
			set_straight(-40.0, 500, 150, 0.0, 0.0);
//			wall_control_flag = 0;
			wait_straight();
			wait_time(50);
		}
	}
}

void move_pass_oblique(float accel, float max_vel, float big_turn_vel,
		float oblique_accel, float oblique_max_vel, uint8_t absorption_flag) {
	volatile uint8_t i = 0;
	volatile int j = 0;
	int8_t straight_count, oblique_flag = 0, oblique_straight_count = 0;
	uint8_t x_box, y_box, direction_box;
	float ro_ki_box=rotation_gain.Ki;
	float ro_kp_box=rotation_gain.Kp;

	if (big_turn_vel >= 1300.0) {
		rotation_gain.Kp = 0.67;
		rotation_gain.Ki = 0.0025;
	} else {
		rotation_gain.Kp = 0.62;
		rotation_gain.Ki = 0.01;
	}

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

	if (absorption_flag == 1) {
		fan_on();
	}

	for (i = 0; i < 255; i++) {
		pass_oblique[i] = 0;
	}

//	x.now = 0;
//	y.now = 0;
//	direction = 0;

	coordinate();
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == 1) {
			direction++;
		} else if (pass[i] == 3) {
			direction += 3;
		}
		if (direction > 3) {
			direction -= 4;
		}
		coordinate();
	}

	x_box = x.now;
	y_box = y.now;
	direction_box = direction;

	j = 0;
	straight_count = 0;
	oblique_straight_count = 0;

	for (i = 0; pass[i] != 0xff; i++) {
		if (oblique_flag == 0) {
			if (pass[i] == 0) {
				straight_count += 2;
			} else {
				if ((pass[i + 1] == pass[i]) && (pass[i - 1] == 0 || i == 0)) {
					if (straight_count > 1) {
						pass_oblique[j] = straight_count - 1;
						j++;
					} else {
						myprintf("error3\n");
					}
					if (pass[i + 2] == 0) {
						if (pass[i] == 1) {
							pass_oblique[j] = BIGLEFT180;
						} else if (pass[i] == 3) {
							pass_oblique[j] = BIGRIGHT180;
						} else {
							myprintf("error4\n");
						}
						j++;
						i++;
						straight_count = -1;
					} else if (pass[i + 2] == 3 && pass[i] == 1) {
						straight_count = 0;
						oblique_flag = 1;
						pass_oblique[j] = LEFT135IN;
						i++;
						j++;
					} else if (pass[i + 2] == 1 && pass[i] == 3) {
						straight_count = 0;
						oblique_flag = 1;
						pass_oblique[j] = RIGHT135IN;
						i++;
						j++;
					} else {
						myprintf("error5\n");
					}
				} else if ((pass[i + 1] == 0) && (i == 0 || pass[i - 1] == 0)) {
					if (straight_count > 1) {
						pass_oblique[j] = straight_count - 1;
						j++;
					} else {
						myprintf("error6\n");
					}
					straight_count = 0;
					if (pass[i] == 1) {
						pass_oblique[j] = BIGLEFT90;
					} else if (pass[i] == 3) {
						pass_oblique[j] = BIGRIGHT90;
					} else {
						myprintf("error7\n");
					}

					straight_count = -1;
					j++;

				} else if (pass[i - 1] == 0 && pass[i] == 1
						&& pass[i + 1] == 3) {
					if (straight_count > 1) {
						pass_oblique[j] = straight_count - 1;
						j++;
					} else {
						myprintf("error8\n");
					}
					straight_count = 0;
					oblique_flag = 1;
					pass_oblique[j] = LEFT45IN;
					j++;
				} else if ((pass[i - 1] == 0 || i == 0) && pass[i] == 3
						&& pass[i + 1] == 1) {
					if (straight_count > 1) {
						pass_oblique[j] = straight_count - 1;
						j++;
					} else {
						myprintf("error9\n");
					}
					straight_count = 0;
					oblique_flag = 1;
					pass_oblique[j] = RIGHT45IN;
					j++;
				} else {
					if (straight_count > 1) {
						pass_oblique[j] = straight_count;
						j++;
					} else {
						myprintf("error10\n");
					}
					straight_count = 0;
					if (pass[i] == 1) {
						pass_oblique[j] = SHORTLEFT90;
					} else if (pass[i] == 3) {
						pass_oblique[j] = SHORTRIGHT90;
					} else {
						myprintf("error11\n");
					}
					j++;
				}
			}
			oblique_straight_count = 0;
		} else if (oblique_flag == 1) {
			if ((pass[i] == 1 && pass[i + 1] == 3)
					|| (pass[i] == 3 && pass[i + 1] == 1)) {
				oblique_straight_count++;
			} else {
				if (oblique_straight_count != 0) {
					pass_oblique[j] = 200 + oblique_straight_count;
					j++;
				}
				oblique_straight_count = 0;
				if (pass[i] == 1 && pass[i + 1] == 0) {
					pass_oblique[j] = LEFT45OUT;
					j++;
					straight_count = -1;
					oblique_flag = 0;
				} else if (pass[i] == 3 && pass[i + 1] == 0) {
					pass_oblique[j] = RIGHT45OUT;
					j++;
					straight_count = -1;
					oblique_flag = 0;
				} else if (pass[i] == 1 && pass[i + 1] == 1
						&& pass[i + 2] == 0) {
					pass_oblique[j] = LEFT135OUT;
					j++;
					i++;
					straight_count = -1;
					oblique_flag = 0;
				} else if (pass[i] == 3 && pass[i + 1] == 3
						&& pass[i + 2] == 0) {
					pass_oblique[j] = RIGHT135OUT;
					j++;
					i++;
					straight_count = -1;
					oblique_flag = 0;
				} else if (pass[i] == 1 && pass[i + 1] == 1
						&& pass[i + 2] == 3) {
					pass_oblique[j] = V_90_LEFT;
					j++;
					i++;
				} else if (pass[i] == 3 && pass[i + 1] == 3
						&& pass[i + 2] == 1) {
					pass_oblique[j] = V_90_RIGHT;
					j++;
					i++;
				} else {
					myprintf("error1\n");
				}
			}
		} else {
			myprintf("error2\n");
		}
		myprintf("j=%d\n", j);
	}
//	if (pass[254] == 4) {
//		straight_count += 2;
//	}
	if (straight_count != 0) {
		pass_oblique[j] = straight_count;
		j++;
		straight_count = 0;
	}
//	myprintf("lasj=%d\n",j);
	pass_oblique[j] = 0xff;

///////////////////////////////////////////////////////////////////////////
//	while (SWITCH == 1) {
//moter_flag=0;
//	}
//	out_put_pass(pass_oblique);
//	while (1)
//		;
////
//	for (i = 0; pass[i] != 0xff; i++) {
//		myprintf("%d\n", pass[i]);
//	}
//	myprintf("\n");
////	ui_led_3bit(1);
//	myprintf("%d\n", pass_oblique[0]);
//	for (i = 0; pass_oblique[i] != 0xff; i++) {
//		myprintf("%d\n", pass_oblique[i]);
//	}
////	ui_led_3bit(3);
//	out_put_pass(pass_oblique);
////
//	while (1)
//		;
//////////////////////////////////////////////////////////////////////////
	moter_flag = 1;

	i = 0;
	coordinate();
	if (pass_oblique[i] < 35) {
		if (pass_oblique[1] == SHORTLEFT90 || pass_oblique[1] == SHORTRIGHT90) {
			set_straight(140.0 + 90.0 * (float) pass_oblique[i], accel, max_vel,
					0.0, nomal_run.vel_search);
		} else {
			set_straight(140.0 + 90.0 * (float) pass_oblique[i], accel, max_vel,
					0.0, big_turn_vel);
		}
		wait_straight();
		for (j = 0; j < pass_oblique[i] / 2; j++) {
			coordinate();
		}
	} else if (pass_oblique[i] == SHORTRIGHT90) { //右折
		go_entrance(accel, nomal_run.vel_search);
		slalom_right90(nomal_run.accel, nomal_run.vel_search);
		coordinate();
	} else if (pass_oblique[i] == BIGRIGHT90) {
		farst_turn_right_90_big(big_turn_vel);
	} else if (pass_oblique[i] == RIGHT45IN) {
		farst_turn_right_45_in(big_turn_vel);
	} else if (pass_oblique[i] == RIGHT135IN) {
		farst_turn_right_135_in(big_turn_vel);
	}

	for (i = 1; pass_oblique[i] != 0xff && failsafe_flag == 0; i++) {
		if (pass_oblique[i] < 35) { //直進の途中
			wall_control_oblique_flag = 0;
			if (pass_oblique[i - 1] == SHORTLEFT90
					|| pass_oblique[i - 1] == SHORTRIGHT90) {
				if (pass_oblique[i + 1] == SHORTLEFT90
						|| pass_oblique[i + 1] == SHORTRIGHT90
						|| pass_oblique[i + 1] == 0xff) {
					set_straight(90.0 * (float) pass_oblique[i], accel, max_vel,
							nomal_run.vel_search, nomal_run.vel_search);
				} else {
					set_straight(90.0 * (float) pass_oblique[i], accel, max_vel,
							nomal_run.vel_search, big_turn_vel);
				}
			} else {
				if (pass_oblique[i + 1] == SHORTLEFT90
						|| pass_oblique[i + 1] == SHORTRIGHT90) {
					set_straight(90.0 * (float) pass_oblique[i], accel, max_vel,
							big_turn_vel, nomal_run.vel_search);
				} else {
					set_straight(90.0 * (float) pass_oblique[i], accel, max_vel,
							big_turn_vel, big_turn_vel);
				}
			}
			wait_straight();
//			for (j = 0; j < pass_oblique[i] / 2; j++) {
//				coordinate();
//			}
		} else if (pass_oblique[i] == SHORTLEFT90) { //左折
			slalom_left90(nomal_run.accel, nomal_run.vel_search);
//			coordinate();
		} else if (pass_oblique[i] == SHORTRIGHT90) { //右折
			slalom_right90(nomal_run.accel, nomal_run.vel_search);
//			coordinate();
		} else if (pass_oblique[i] == BIGLEFT90) {
			turn_left_90_big(big_turn_vel);
//			coordinate();
		} else if (pass_oblique[i] == BIGRIGHT90) {
			turn_right_90_big(big_turn_vel);
//			coordinate();
		} else if (pass_oblique[i] == BIGLEFT180) {
			turn_left_180_big(big_turn_vel);
//			coordinate();
		} else if (pass_oblique[i] == BIGRIGHT180) {
			turn_right_180_big(big_turn_vel);
//			coordinate();
		} else if (pass_oblique[i] == LEFT45IN) {
			turn_left_45_in(big_turn_vel);
		} else if (pass_oblique[i] == RIGHT45IN) {
			turn_right_45_in(big_turn_vel);
		} else if (pass_oblique[i] == LEFT135IN) {
			turn_left_135_in(big_turn_vel);
		} else if (pass_oblique[i] == RIGHT135IN) {
			turn_right_135_in(big_turn_vel);
		} else if (pass_oblique[i] == LEFT45OUT) {
			turn_left_45_out(big_turn_vel);
		} else if (pass_oblique[i] == RIGHT45OUT) {
			turn_right_45_out(big_turn_vel);
		} else if (pass_oblique[i] == LEFT135OUT) {
			turn_left_135_out(big_turn_vel);
		} else if (pass_oblique[i] == RIGHT135OUT) {
			turn_right_135_out(big_turn_vel);
		} else if (pass_oblique[i] == V_90_LEFT) {
			turn_left_v90(big_turn_vel);
		} else if (pass_oblique[i] == V_90_RIGHT) {
			turn_right_v90(big_turn_vel);
		} else if (pass_oblique[i] > 200) {
			wall_control_oblique_flag = 1;
			set_straight(127.28 * (pass_oblique[i] - 200), oblique_accel,
					oblique_max_vel, big_turn_vel, big_turn_vel);
			wait_straight();
			wall_control_oblique_flag = 0;
		}
	}

	x.now = x_box;
	y.now = y_box;
	direction = direction_box;
	rotation_gain.Kp = 0.62;
	rotation_gain.Ki = 0.01;

	if (pass[254] == 1) {
		if (pass_oblique[i - 1] == SHORTLEFT90
				|| pass_oblique[i - 1] == SHORTRIGHT90) {
			set_straight(90.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
		} else if (pass[i - 1] > 35) {
			set_straight(90.0, accel, max_vel, big_turn_vel, 0.0);
		} else {
			set_straight(90.0, accel, max_vel, big_turn_vel, 0.0);
		}
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
	} else {
//		coordinate();
		addWall();

		if (pass_oblique[i - 1] == SHORTLEFT90
				|| pass_oblique[i - 1] == SHORTRIGHT90) {
			set_straight(88.0, accel, nomal_run.vel_search,
					nomal_run.vel_search, 0.0);
		} else if (pass[i - 1] > 35) {
			set_straight(88.0, accel, max_vel, big_turn_vel, 0.0);
		} else {
			set_straight(88.0, accel, max_vel, big_turn_vel, 0.0); //max_vel????
		}
		wait_straight();
//		if (x.goal == 7 && y.goal == 7 && failsafe_flag == 0) {
//			wait_time(5);
//			victory_fanfare(100, 1);
//		} else {
//			wait_time(200);
//		}

		fan_off();
		wait_time(700);
		rotation_deviation.now = 0.0;
		rotation_deviation.cumulative = 0.0;
		wallcontrol_value = 0.0;
		run_left_deviation.cumulative = 0.0;
		run_right_deviation.cumulative = 0.0;

		if (getWall(x.now, y.now, direction, &walldate_real)) {

			set_rotation(-180.0, nomal_rotation.accel,
					nomal_rotation.vel_search, 0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
		} else if (getWall(x.now, y.now, direction + 1, &walldate_real)) {

			set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
					0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
			rotation_deviation.cumulative = 0.0;
		} else if (getWall(x.now, y.now, direction + 3, &walldate_real)) {
			set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,
					0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
			rotation_deviation.cumulative = 0.0;
		} else if (getWall(x.now, y.now, direction + 1, &walldate_real)) {

			set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,
					0.0);
			wait_rotation();
			wait_time(50);
			back_100();
			wait_time(50);
			rotation_deviation.cumulative = 0.0;
		} else {

			set_rotation(-180.0, nomal_rotation.accel,
					nomal_rotation.vel_search, 0.0);
			wait_rotation();
			wait_time(50);
			set_straight(-40.0, 500, 150, 0.0, 0.0);
//			wall_control_flag = 0;
			wait_straight();
			wait_time(50);
		}
	}
	fan_off();
	wait_time(100);
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
	wallcontrol_value=0.0;
}

void out_put_pass(uint8_t *pass) {
	int i = 0;
	for (i = 0; pass[i] != 0xff; i++) {
		if (pass[i] == SHORTLEFT90) {
			myprintf("SHORTLEFT90\n");
		} else if (pass[i] == SHORTRIGHT90) {
			myprintf("SHORTRIGHT90\n");
		} else if (pass[i] == BIGLEFT180) {
			myprintf("BIGLEFT180\n");
		} else if (pass[i] == BIGRIGHT180) {
			myprintf("BIGRIGHT180\n");
		} else if (pass[i] == BIGLEFT90) {
			myprintf("BIGLEFT90\n");
		} else if (pass[i] == BIGRIGHT90) {
			myprintf("BIGRIGHT90\n");
		} else if (pass[i] == FARST_BIGRIGHT90) {
			myprintf("FARST_BIGRIGHT90\n");
		} else if (pass[i] == FARST_SHORTRIGHT90) {
			myprintf("BIGLEFT180\n");
		} else if (pass[i] == FARST_SHORTRIGHT90) {
			myprintf("BIGRIGHT180\n");
		} else if (pass[i] == LEFT45IN) {
			myprintf("LEFT45IN\n");
		} else if (pass[i] == RIGHT45IN) {
			myprintf("RIGHT45IN\n");
		} else if (pass[i] == LEFT135IN) {
			myprintf("LEFT135IN\n");
		} else if (pass[i] == RIGHT135IN) {
			myprintf("RIGHT135IN\n");
		} else if (pass[i] == LEFT45OUT) {
			myprintf("LEFT45OUT\n");
		} else if (pass[i] == RIGHT45OUT) {
			myprintf("RIGHT45OUT\n");
		} else if (pass[i] == LEFT135OUT) {
			myprintf("LEFT135OUT\n");
		} else if (pass[i] == RIGHT135OUT) {
			myprintf("RIGHT135OUT\n");
		} else if (pass[i] == V_90_LEFT) {
			myprintf("V_90_LEFT\n");
		} else if (pass[i] == V_90_RIGHT) {
			myprintf("V_90_RIGHT\n");
		} else if (pass[i] < 30) {
			myprintf("straight:%d\n", pass[i]);
		} else if (pass[i] > 200) {
			myprintf("oblique_straight:%d\n", pass[i] - 200);
		} else {
			myprintf("%d\n", pass[i]);
		}

	}

}

//void make_pass_big_turn(void) {
//	uint8_t i = 0, j;
//	int8_t straight_count;
//	uint8_t x_box, y_box, direction_box;
//
//	for (i = 0; i < 255; i++) {
//		pass_big[i] = 0;
//	}
//
//	x.now = 0;
//	y.now = 0;
//
//	coordinate();
//	for (i = 0; pass[i] != 0xff; i++) {
//		if (pass[i] == 1) {
//			direction++;
//		} else if (pass[i] == 3) {
//			direction += 3;
//		}
//		if (direction > 3) {
//			direction -= 4;
//		}
//		coordinate();
//	}
//
//	x_box = x.now;
//	y_box = y.now;
//	direction_box = direction;
//
//	j = 0;
//	straight_count = 0;
//
//	for (i = 0; pass[i] != 0xff; i++) {
//		if (pass[i] == 0) {
//			straight_count += 2;
//		} else {
//			if (pass[i + 1] == pass[i] && pass[i + 2] == 0
//					&& pass[i - 1] == 0) {
//				if (straight_count > 1) {
//					pass_big[j] = straight_count - 1;
//					j++;
//				}
//				straight_count = 0;
//				if (pass[i] == 1) {
//					pass_big[j] = BIGLEFT180;
//				} else if (pass[i] == 3) {
//					pass_big[j] = BIGRIGHT180;
//				}
//				j++;
//				i++;
//				straight_count = -1;
//			} else if (pass[i + 1] == 0 && (i == 0 || pass[i - 1] == 0)) {
//				if (straight_count > 1) {
//					pass_big[j] = straight_count - 1;
//					j++;
//				}
//				straight_count = 0;
//				if (pass[i] == 1) {
//					pass_big[j] = BIGLEFT90;
//				} else if (pass[i] == 3) {
//					pass_big[j] = BIGRIGHT90;
//				}
//
//				straight_count = -1;
//				j++;
//
//			} else {
//				if (straight_count > 0) {
//					pass_big[j] = straight_count;
//					j++;
//				}
//				straight_count = 0;
//				if (pass[i] == 1) {
//					pass_big[j] = SHORTLEFT90;
//				} else if (pass[i] == 3) {
//					pass_big[j] = SHORTRIGHT90;
//				}
//				j++;
//			}
//		}
//	}
////	if (pass[254] == 4) {
////		straight_count += 2;
////	}
//	if (straight_count != 0) {
//		pass_big[j] = straight_count;
//		j++;
//		straight_count = 0;
//	}
//	pass_big[j] = 0xff;
//}
