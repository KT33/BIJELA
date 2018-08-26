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

#define QUEUE_SIZE 255

typedef struct {
	uint16_t queue[QUEUE_SIZE];
	uint8_t head; //先頭位置
	uint8_t tail; //末尾位置
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
}

void queue_push(queue_t *q, uint8_t x, uint8_t y) {
	uint16_t box;
	box = (uint16_t) (y * 16 + x);
	q->queue[q->tail] = box;
	q->tail++;
	if (q->tail == QUEUE_SIZE) {
		q->tail = 0;
	}
}

void adachi_map(uint8_t goal_x, uint8_t goal_y, walldate_t walldate) {
	uint8_t x_adachi, y_adachi, step;
	queue_t q;

	q.head = 0;
	q.tail = 0;

	for (x_adachi = 0; x_adachi < 16; x_adachi++) {
		for (y_adachi = 0; y_adachi < 16; y_adachi++) {
			step_map[x_adachi][y_adachi] = 255;
		}
	}

	step_map[goal_x][goal_y] = 0;
	x_adachi = goal_x;
	y_adachi = goal_y;
	step = 0;
	queue_push(&q, x_adachi, y_adachi);
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

void adachi_search_run(uint8_t goal_x, uint8_t goal_y, float accel, float vel) {
	uint8_t flag; //flag 0:前,1:左折2:Uターン(けつあて)3:右折4:Uターン
	go_entrance(accel, vel);
	coordinate();
	addWall();
	while (1) {
		adachi_map(goal_x, goal_y, walldate_real);
		if (((x.now == goal_x && y.now == goal_y))) {
			if ((getWall(goal_x, goal_y, direction + 1, walldate_real))
					&& (getWall(goal_x, goal_y, direction, walldate_real))) {
				ketuate_goal_left(accel, vel);
			} else if ((getWall(goal_x, goal_y, direction + 3, walldate_real))
					&& (getWall(goal_x, goal_y, direction, walldate_real))) {
				ketuate_goal_right(accel, vel);
			} else {
				ketuate_goal(accel, vel);
			}
			break;
		}
		flag = how_to_move(direction, (int8_t) x.now, (int8_t) y.now,
				walldate_real);
		if (flag == 0) {
			pass_180(accel, vel);
		}
		if (flag == 1) {
			turn_left(accel, vel);
		}
		if (flag == 2) {
			ketuate_right(accel, vel);
		}
		if (flag == 3) {
			turn_right(accel, vel);
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

void pass_180(float accel, float vel) {
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
	set_straight(-65.0, 1000, 200, 0.0, 0.0);
	wait_straight();
}

void ketuate_goal_left(float accel, float vel) {
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
}

void ketuate_goal_right(float accel, float vel) {
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
}

void ketuate_goal(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(100);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(100);
	back_100();
	wait_time(100);
}
