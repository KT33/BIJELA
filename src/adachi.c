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
//	uint8_t test = 0;

//	moter_flag = 1;
	q.head = 0;
	q.tail = 0;

	for (x_adachi = 0; x_adachi < 16; x_adachi++) {
		for (y_adachi = 0; y_adachi < 16; y_adachi++) {
			step_map[x_adachi][y_adachi] = 999;
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
				step_map[x_adachi][y_adachi + 1] = step + 1;
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
				step_map[x_adachi - 1][y_adachi] = step + 1;
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
				step_map[x_adachi][y_adachi - 1] = step + 1;
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
				step_map[x_adachi + 1][y_adachi] = step + 1;
			}
			queue_push(&q, x_adachi + 1, y_adachi);
			flag = 10;
		}
//		myprintf("fast	%d,%d,%d,%d\n", x_adachi, y_adachi, q.head, q.tail);
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
			step_map[x_adachi][y_adachi] = 999;
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
	moter_flag = 1;
	while (failsafe_flag == 0) {
		if (step_map[x.now][y.now] == 999) {
			stop90(accel, vel);
			failsafe_flag = 1;
			break;
		}
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
				set_straight(95.0, accel, vel, vel, 0.0);
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

void adachi_search_run_known(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		float accel, float vel, uint8_t slalom_flag, uint8_t straight_flag) {
	uint8_t flag, i; //flag 0:前,1:左折2:Uターン(けつあて)3:右折4:Uターン
	go_entrance(accel, vel);
	coordinate();
	addWall();
	moter_flag = 1;
	while (failsafe_flag == 0) {
		if (step_map[x.now][y.now] == 999) {
			stop90(accel, vel);
			failsafe_flag = 1;
			break;
		}
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
				set_straight(95.0, accel, vel, vel, 0.0);
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
		flag = how_to_move_search_known(direction, (int8_t) x.now,
				(int8_t) y.now);
		if (flag == 0 || flag == 11) {
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
		if (flag > 11) {
			set_straight(180.0 * (flag - 10), 6000.0, 1800.0, vel, vel);
			wait_straight();
			for (i = 0; i < flag - 10 - 1; i++) {
				coordinate();
			}
		}
		coordinate();
		addWall();
	}
}

uint8_t how_to_move_search_known(uint8_t direction, int8_t x, int8_t y) {
	uint8_t step, flag, i, known_straight = 0;
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
		if ((getWall(x, y, (North + i) % 4, &walldate_real) == 1)
				&& (getWall(x, y, (East + i) % 4, &walldate_real) == 1)
				&& (getWall(x, y, (West + i) % 4, &walldate_real) == 1)) {
			flag = 2;
		} else {
			flag = 4;
		}
		step = step_map[x + b][y - a];
	}
	if ((x - a >= 0) && (y - b >= 0)
			&& (getWall(x, y, (West + i) % 4, &walldate_real) == 0)) {
		if (step_map[x - a][y - b] <= step) {
			flag = 1;
			step = step_map[x - a][y - b];
		}
	}
	if ((x + a <= 15) && (y + b <= 15)
			&& (getWall(x, y, (East + i) % 4, &walldate_real) == 0)) {
		if (step_map[x + a][y + b] <= step) {
			flag = 3;
			step = step_map[x + a][y + b];
		}
	}
	if ((y + a <= 15) && (x - b >= 0)
			&& (getWall(x, y, (North + i) % 4, &walldate_real) == 0)) {
		if (step_map[x - b][y + a] <= step) {
			flag = 0;
			step = step_map[x - b][y + a];
		}
	}
	if (flag == 0 && (getWall(x, y, (North + i) % 4, &walldate_adachi) == 0)) {
		do {
			known_straight++;
			if (direction == North && y < 15) {
				y++;
			} else if (direction == West && x > 0) {
				x--;
			} else if (direction == South && y > 0) {
				y--;
			} else if (direction == East && y < 15) {
				x++;
			} else {
				break;
			}
		} while ((getWall(x, y, (North + i) % 4, &walldate_adachi) == 0
				&& how_to_move(direction, x, y, walldate_real) == 0));
		flag = known_straight + 10;
	}
	return flag;
}

void I_want_to_check_search_run(uint8_t goal_x,uint8_t goal_y){
	uint8_t straight_flag,goal_scale,flag;
	uint8_t traget_x,traget_y;
	x.pass=0;
	y.pass=0;
	direction_pass=North;

	if (straight_flag == 0) {
		adachi_map(goal_x, goal_y, goal_scale, walldate_real);
	} else if (straight_flag == 1) {
		adachi_map_straight(goal_x, goal_y, goal_scale, walldate_real);
	}

	while(step_map[x.pass][y.pass]!=0){
		if (straight_flag == 0) {
			adachi_map(goal_x, goal_y, goal_scale, walldate_real);
		} else if (straight_flag == 1) {
			adachi_map_straight(goal_x, goal_y, goal_scale, walldate_real);
		}

		flag=how_to_move_search_known(direction_pass, x.pass, y.pass);
		while(flag<10){
			if(flag==0){
			}
			if(flag==1){
				direction_pass += 1;
			}
			if(flag==2||flag==4){
				direction_pass += 2;
			}
			if(flag==3){
				direction_pass += 3;
			}
			if (direction_pass > 3) {
				direction_pass -= 4;
			}
			coordinate_pass();
			flag=how_to_move_search_known(direction_pass, x.pass, y.pass);
		}
		if(flag==10){


		}
	}

}

uint8_t how_to_move_special(uint8_t direction, int8_t x, int8_t y) {
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
		if ((getWall(x, y, (North + i) % 4, &walldate_real) == 1)
				&& (getWall(x, y, (East + i) % 4, &walldate_real) == 1)
				&& (getWall(x, y, (West + i) % 4, &walldate_real) == 1)) {
			flag = 2;
		} else {
			flag = 4;
		}
		step = step_map[x + b][y - a];
	}
	if ((x - a >= 0) && (y - b >= 0)
			&& (getWall(x, y, (West + i) % 4,& walldate_real) == 0)) {
		if (step_map[x - a][y - b] <= step) {
			flag = 1;
			step = step_map[x - a][y - b];
		}
		if(getWall(x, y, (West + i) % 4, &walldate_checked) == 0){
			flag+=10;
		}
	}
	if ((x + a <= 15) && (y + b <= 15)
			&& (getWall(x, y, (East + i) % 4, &walldate_real) == 0)) {
		if (step_map[x + a][y + b] <= step) {
			flag = 3;
			step = step_map[x + a][y + b];
		}
		if(getWall(x, y, (East + i) % 4, &walldate_checked) == 0){
			flag+=10;
		}
	}
	if ((y + a <= 15) && (x - b >= 0)
			&& (getWall(x, y, (North + i) % 4, &walldate_real) == 0)) {
		if (step_map[x - b][y + a] <= step) {
			flag = 0;
			step = step_map[x - b][y + a];
		}
		if(getWall(x, y, (North + i) % 4, &walldate_checked) == 0){
			flag+=10;
		}
	}
	return flag;
}
