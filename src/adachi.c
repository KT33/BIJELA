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
#include "stdint.h"
#include "walldate.h"

#define QUEUE_SIZE 255

typedef struct {
	uint16_t queue[QUEUE_SIZE];
	uint16_t head; //先頭位置
	uint16_t tail; //末尾位置
} queue_t;

void queue_pop(queue_t *q, uint8_t *x, uint8_t *y) {
	uint16_t box;
	box = q->queue[q->head];
	q->head++;
	if (q->head == QUEUE_SIZE) {
		q->head = 0;
	}
	*y = (uint8_t) box / 16;
	*x = (uint8_t) box % 16;
//	myprintf("pop,x:%d,y:%d\n",*x,*y);
}

void queue_push(queue_t *q, uint8_t x, uint8_t y) {
	uint16_t box;
	box = (uint16_t) (y * 16 + x);
	q->queue[q->tail] = box;
	q->tail++;
	if (q->tail == QUEUE_SIZE) {
		q->tail = 0;
	}
//	myprintf("pop,x:%d,y:%d\n",x,y);
}

void adachi_map(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		walldate_t walldate) {
	uint8_t x_adachi, y_adachi, step;
	queue_t q;

	q.head = 0;
	q.tail = 0;

	for (x_adachi = 0; x_adachi < 16; x_adachi++) {
		for (y_adachi = 0; y_adachi < 16; y_adachi++) {
			step_map[x_adachi][y_adachi] = 255;
		}
	}

	if (goal_scale == 1) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
	} else if (goal_scale == 4) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x + 1][goal_y] = 0;
		x_adachi = goal_x + 1;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x][goal_y + 1] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y + 1;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x + 1][goal_y + 1] = 0;
		x_adachi = goal_x + 1;
		y_adachi = goal_y + 1;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
	}
	do {
		flag = 0;
		queue_pop(&q, &x_adachi, &y_adachi);
		step = step_map[x_adachi][y_adachi];
		if ((getWall(x_adachi, y_adachi, North, &walldate) == 0)
				&& (step_map[x_adachi][y_adachi + 1] == 255)
				&& ((y_adachi + 1) < 16)) {
			step_map[x_adachi][y_adachi + 1] = step + 1;
			queue_push(&q, x_adachi, y_adachi + 1);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, West, &walldate) == 0)
				&& (step_map[x_adachi - 1][y_adachi] == 255)
				&& ((x_adachi - 1) >= 0)) {
			step_map[x_adachi - 1][y_adachi] = step + 1;
			queue_push(&q, x_adachi - 1, y_adachi);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, South, &walldate) == 0)
				&& (step_map[x_adachi][y_adachi - 1] == 255)
				&& ((y_adachi - 1) >= 0)) {
			step_map[x_adachi][y_adachi - 1] = step + 1;
			queue_push(&q, x_adachi, y_adachi - 1);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, East, &walldate) == 0)
				&& (step_map[x_adachi + 1][y_adachi] == 255)
				&& ((x_adachi + 1) < 16)) {
			step_map[x_adachi + 1][y_adachi] = step + 1;
			queue_push(&q, x_adachi + 1, y_adachi);
			flag = 10;
		}
//		myprintf("%d,%d,%d,%d\n", x_adachi, y_adachi, q.head, q.tail);
	} while (q.tail != q.head);
}

void adachi_map_straight(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		walldate_t walldate) {
	uint8_t x_adachi, y_adachi, step;
	queue_t q;
//	uint8_t test = 0;

//	moter_flag = 1;
	q.head = 0;
	q.tail = 0;

	for (x_adachi = 0; x_adachi < 16; x_adachi++) {
		for (y_adachi = 0; y_adachi < 16; y_adachi++) {
			step_map[x_adachi][y_adachi] = 255;
		}
	}

	if (goal_scale == 1) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
	} else if (goal_scale == 4) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x + 1][goal_y] = 0;
		x_adachi = goal_x + 1;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x][goal_y + 1] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y + 1;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		step_map[goal_x + 1][goal_y + 1] = 0;
		x_adachi = goal_x + 1;
		y_adachi = goal_y + 1;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
	}
	do {
		flag = 0;
		queue_pop(&q, &x_adachi, &y_adachi);

///////////////////////////////////////////
//		step = step_map[x_adachi][y_adachi];
/////////////////////////////////////////

//////
//		while (step + 3 < step_map[x_adachi][y_adachi]) {
//			queue_push(&q, x_adachi, y_adachi);
//			queue_pop(&q, &x_adachi, &y_adachi);
//		}

		step = step_map[x_adachi][y_adachi];

//////

		if ((getWall(x_adachi, y_adachi, North, &walldate) == 0)
				&& (step_map[x_adachi][y_adachi + 1] > step + 1)
				&& ((y_adachi + 1) < 16)) {
			if (((step_map[x_adachi][y_adachi - 1] == step - 1)
					|| (step_map[x_adachi][y_adachi - 1] == step - 2))
					&& (y_adachi - 1) >= 0
					&& getWall(x_adachi, y_adachi, South, &walldate) == 0) {
				step_map[x_adachi][y_adachi + 1] = step + 1;
			} else {
				step_map[x_adachi][y_adachi + 1] = step + 2;
			}
			queue_push(&q, x_adachi, y_adachi + 1);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, West, &walldate) == 0)
				&& (step_map[x_adachi - 1][y_adachi] > step + 1)
				&& ((x_adachi - 1) >= 0)) {
			if (((step_map[x_adachi + 1][y_adachi] == step - 1)
					|| (step_map[x_adachi + 1][y_adachi] == step - 2))
					&& (x_adachi + 1) < 16
					&& getWall(x_adachi, y_adachi, East, &walldate) == 0) {
				step_map[x_adachi - 1][y_adachi] = step + 1;
			} else {
				step_map[x_adachi - 1][y_adachi] = step + 2;
			}

			queue_push(&q, x_adachi - 1, y_adachi);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, South, &walldate) == 0)
				&& (step_map[x_adachi][y_adachi - 1] > step + 1)
				&& ((y_adachi - 1) >= 0)) {
			if (((step_map[x_adachi][y_adachi + 1] == step - 1)
					|| (step_map[x_adachi][y_adachi + 1] == step - 2))
					&& (y_adachi + 1) < 16
					&& getWall(x_adachi, y_adachi, North, &walldate) == 0) {
				step_map[x_adachi][y_adachi - 1] = step + 1;
			} else {
				step_map[x_adachi][y_adachi - 1] = step + 2;
			}
			queue_push(&q, x_adachi, y_adachi - 1);
			flag = 10;
		}
		if ((getWall(x_adachi, y_adachi, East, &walldate) == 0)
				&& (step_map[x_adachi + 1][y_adachi] > step + 1)
				&& ((x_adachi + 1) < 16)) {
			if (((step_map[x_adachi - 1][y_adachi] == step - 1)
					|| (step_map[x_adachi - 1][y_adachi] == step - 2))
					&& (x_adachi - 1) >= 0
					&& getWall(x_adachi, y_adachi, West, &walldate) == 0) {
				step_map[x_adachi + 1][y_adachi] = step + 1;
			} else {
				step_map[x_adachi + 1][y_adachi] = step + 2;
			}
			queue_push(&q, x_adachi + 1, y_adachi);
			flag = 10;
		}
//		myprintf("fast	%d,%d,%d,%d\n", x_adachi, y_adachi, q.head, q.tail);
	} while (q.tail != q.head);
}

void adachi_search_run(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		float accel, float vel, uint8_t slalom_flag, uint8_t straight_flag) {
	uint8_t flag; //flag 0:前,1:左折2:Uターン(けつあて)3:右折4:Uターン
	go_entrance(accel, vel);
	coordinate();
	addWall();

	while (failsafe_flag == 0) {
		if (straight_flag == 1) {
			adachi_map_straight(goal_x, goal_y, goal_scale, walldate_real);
		} else {
			adachi_map(goal_x, goal_y, goal_scale, walldate_real);
		}

		if ((goal_scale == 1) && ((x.now == goal_x && y.now == goal_y))) {
			if (getWall(goal_x, goal_y, direction, &walldate_real)) {
				set_straight(90.0, accel, vel, vel, 0.0);
				wait_straight();
				wait_time(50);
				set_rotation(180.0, nomal_rotation.accel,
						nomal_rotation.vel_search, 0.0);
				wait_rotation();
				wait_time(50);
				back_100();
				wait_time(50);
				rotation_deviation.cumulative = 0.0;
			} else {
				non_ketuate_goal(accel, vel);
			}
			break;
		}
		if ((goal_scale == 4)
				&& ((x.now == goal_x && y.now == goal_y)
						|| (x.now == goal_x + 1 && y.now == goal_y)
						|| (x.now == goal_x && y.now == goal_y + 1)
						|| (x.now == goal_x + 1 && y.now == goal_y + 1))) {

			if (getWall(x.now, y.now, direction + 1, &walldate_real)) {
				set_straight(95.0, accel, vel, vel, 0.0);
				wait_straight();
				wait_time(50);
				set_rotation(-90.0, nomal_rotation.accel,
						nomal_rotation.vel_search, 0.0);
				wait_rotation();
				wait_time(50);
				back_100();
				wait_time(50);
				rotation_deviation.cumulative = 0.0;
			} else if (getWall(x.now, y.now, direction + 3, &walldate_real)) {
				set_straight(90.0, accel, vel, vel, 0.0);
				wait_straight();
				wait_time(50);
				set_rotation(90.0, nomal_rotation.accel,
						nomal_rotation.vel_search, 0.0);
				wait_rotation();
				wait_time(50);
				back_100();
				wait_time(50);
				rotation_deviation.cumulative = 0.0;
			} else {
				set_straight(93.0, accel, vel, vel, 0.0);
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
			break;
		}
		flag = how_to_move(direction, (int8_t) x.now, (int8_t) y.now,
				walldate_real);
		if (flag == 0) {
			pass_180(accel, vel);
		}
		if (flag == 1) {
			if (slalom_flag == 0) {
				turn_left(accel, vel);
			} else {
//				if (vel == 600.0) {
//					slalom_left90_600(accel, nomal_run.vel_search,
//							nomal_rotation.accel, nomal_rotation.vel_search);
//				} else {
				slalom_left90(accel, vel);
//				}
			}
		}
		if (flag == 2) {
//			ketuate_right(accel, vel);
			ketuate(accel, vel);
		}
		if (flag == 3) {
			if (slalom_flag == 0) {
				turn_right(accel, vel);
			} else {
//				if (vel == 600.0) {
//					slalom_right90_600(accel, nomal_run.vel_search,
//							nomal_rotation.accel, nomal_rotation.vel_search);
//				} else {
				slalom_right90(accel, vel);
//				}
			}
		}

		if (flag == 4) {
			turn_180(accel, vel);
		}
		coordinate();
		addWall();
	}
}

uint8_t how_to_move(uint8_t direction, int8_t x, int8_t y, walldate_t walldate) {
	uint8_t step, flag, i;
	int8_t a, b;
	if (direction == North) {
		i = 0;
		a = 1;
		b = 0;
	} else if (direction == West) {
		i = 1;
		a = 0;
		b = 1;
	} else if (direction == South) {
		i = 2;
		a = -1;
		b = 0;
	} else if (direction == East) {
		i = 3;
		a = 0;
		b = -1;
	}
	if ((y - a >= 0) && (x + b < 16)) {
		if ((getWall(x, y, (North + i) % 4, &walldate) == 1)
				&& (getWall(x, y, (East + i) % 4, &walldate) == 1)
				&& (getWall(x, y, (West + i) % 4, &walldate) == 1)) {
			flag = 2;
		} else {
			flag = 4;
		}
		step = step_map[x + b][y - a];
	}
	if ((x - a >= 0) && (y - b >= 0)
			&& (getWall(x, y, (West + i) % 4, &walldate) == 0)) {
		if (step_map[x - a][y - b] <= step) {
			flag = 1;
			step = step_map[x - a][y - b];
		}
	}
	if ((x + a <= 15) && (y + b <= 15)
			&& (getWall(x, y, (East + i) % 4, &walldate) == 0)) {
		if (step_map[x + a][y + b] <= step) {
			flag = 3;
			step = step_map[x + a][y + b];
		}
	}
	if ((y + a <= 15) && (x - b >= 0)
			&& (getWall(x, y, (North + i) % 4, &walldate) == 0)) {
		if (step_map[x - b][y + a] <= step) {
			flag = 0;
			step = step_map[x - b][y + a];
		}
	}
	return flag;
}

void right_hand(float accel, float vel) {
	int i = 0;
	go_entrance(accel, vel);
	coordinate();
	addWall();
	while (1) {
		if (SEN_L.now < SEN_L.threshold) {
			turn_left(accel, vel);
		} else if (SEN_F.now < SEN_F.threshold) {
			pass_180(accel, vel);
		} else if (SEN_R.now < SEN_R.threshold) {
			turn_right(accel, vel);
		} else {
			ketuate_right(accel, vel);
		}
		coordinate();
		addWall();
		i++;
		if (i == 20) {
			set_straight(90.0, accel, vel, vel, 0.0);
			wait_straight();
			break;
		}
	}
}

void go_entrance(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//rotation_deviation.cumulative = 0.0;
	moter_flag = 1;
	set_straight(140.0, accel, vel, 0.0, vel);
	wait_straight();
}

void go_center(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
	set_straight(54.0, accel, vel, 0.0, 0.0);
	wait_straight();
}

void turn_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void turn_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void pass_180(float accel, float vel) {
	set_straight(180.0, accel, vel, vel, vel);
	wait_straight();
}

void turn_180(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void ketuate(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void ketuate_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void ketuate_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	set_straight(57.0, accel, vel, 0.0, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void back_100(void) {
	set_straight(-80.0, 300, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
}

void ketuate_goal_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
}

void ketuate_goal_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
}

void non_ketuate_goal_turn(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(-50.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	wait_time(50);
}

void non_ketuate_goal(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_straight(-50.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	wait_time(50);
}

void slalom_left90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset;
	if (run_vel == 500.0) {
		rota_accel = slarom_500.accel;
		rota_vel = slarom_500.max_vel;
		in_offset = slarom_500.left.in_offset;
		out_offset = slarom_500.left.out_offset;
	} else if (run_vel == 600.0) {
		rota_accel = slarom_600.accel;
		rota_vel = slarom_600.max_vel;
		in_offset = slarom_600.left.in_offset;
		out_offset = slarom_600.left.out_offset;
	}
	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(90.0, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
}

void slalom_right90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset;
	float angle_offset = 0.0;
	if (run_vel == 500.0) {
		rota_accel = slarom_500.accel;
		rota_vel = slarom_500.max_vel;
		in_offset = slarom_500.right.in_offset;
		out_offset = slarom_500.right.out_offset;
		angle_offset = 1.0;
	} else if (run_vel == 600.0) {
		rota_accel = slarom_600.accel;
		rota_vel = slarom_600.max_vel;
		in_offset = slarom_600.right.in_offset;
		out_offset = slarom_600.right.out_offset;
		angle_offset = 2.0;
	}
	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
}

void stop90(float run_accel, float run_vel) {
	set_straight(90.0, run_accel, run_vel, run_vel, 0.0);
	wait_straight();
}

void slalom_left_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_left90(accel, vel);
	stop90(accel, vel);
}

void slalom_right_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_right90(accel, vel);
	stop90(accel, vel);
}

void turn_left_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 20.0;
		out_offset = 38.0;
		rota_accel = 6000.0;
		rota_vel = 400.0;
		kabekire_dis = 7.2;
		angle_offset = 0;
	}

	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 45.0;
		rota_accel = 8700.0;
		rota_vel = 640.0;
		kabekire_dis = 1.0;
		angle_offset = -1.2;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0]=translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();

}

void turn_right_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 30.0;
		out_offset = 43.0;
		rota_accel = 4700.0;
		rota_vel = 385.0;
		angle_offset = 1.0;
		kabekire_dis = 13.2;
	}
	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 38.0;
		rota_accel = 8700.0;
		rota_vel = 666.0;
		kabekire_dis = 4.0;
		angle_offset = 1.5;
	}
	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
}

void turn_left_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 52.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		kabekire_dis = 7.8;
		angle_offset = 0.0;
	}
	if (vel == 1000.0) {
		in_offset = 23.0;
		out_offset = 57.0;
		rota_accel = 6200.0;
		rota_vel = 1500.0;
		kabekire_dis = 2.0;
		angle_offset = 0.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(90.0 - angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
		kabekire_dis = 12.0;
	}

	if (vel == 1000.0) {
		in_offset = 25.0;
		out_offset = 53.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		kabekire_dis = 5.0;
		angle_offset = 1.0;
	}
	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void farst_turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
	}

	if (vel == 1000.0) {
		in_offset = 25.0;
		out_offset = 53.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		angle_offset = 1.0;
	}

	coordinate();

	set_straight(in_offset + 50.0, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_45(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 15.0;
		out_offset = 66.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
		kabekire_dis = 4.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_left_45(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 5.0;
		out_offset = 85.0;
		rota_accel = 12000.0;
		rota_vel = 1100.0;
		angle_offset = 0.0;
		kabekire_dis = 2.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
			translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_right_135(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 0.0;
		rota_accel = 9000.0;
		rota_vel = 700.0;
		angle_offset = 0.0;
		kabekire_dis = 4.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
//		translation_ideal.dis = kabekire_dis;
	}

	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_left_135(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 5.0;
		out_offset = 85.0;
		rota_accel = 12000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
		kabekire_dis = 2.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
			translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}


