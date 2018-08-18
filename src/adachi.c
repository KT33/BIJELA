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

void adachi_map(uint8_t goal_x, uint8_t goal_y) {
	uint8_t x_adachi, y_adachi, step, flag = 1;
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

	do {
		flag = 0;
		if ((getWall(x_adachi, y_adachi, North, &walldate_adachi) == 0)
				&& (step_map[x_adachi][y_adachi + 1] > step)
				&& ((y_adachi + 1) < 16)) {
			step_map[x_adachi][y_adachi + 1] = step + 1;
			queue_push(&q, x_adachi, y_adachi + 1);
			flag = 1;
		}
		if ((getWall(x_adachi, y_adachi, West, &walldate_adachi) == 0)
				&& (step_map[x_adachi - 1][y_adachi] > step)
				&& ((x_adachi - 1) >= 0)) {
			step_map[x_adachi - 1][y_adachi] = step + 1;
			queue_push(&q, x_adachi - 1, y_adachi);
			flag = 1;
		}
		if ((getWall(x_adachi, y_adachi, South, &walldate_adachi) == 0)
				&& (step_map[x_adachi][y_adachi - 1] > step)
				&& ((y_adachi - 1) >= 0)) {
			step_map[x_adachi][y_adachi - 1] = step + 1;
			queue_push(&q, x_adachi, y_adachi - 1);
			flag = 1;
		}
		if ((getWall(x_adachi, y_adachi, East, &walldate_adachi) == 0)
				&& (step_map[x_adachi + 1][y_adachi] > step)
				&& ((x_adachi + 1) < 16)) {
			step_map[x_adachi + 1][y_adachi] = step + 1;
			queue_push(&q, x_adachi + 1, y_adachi);
			flag = 1;
		}
		queue_pop(&q, &x_adachi, &y_adachi);
		step = step_map[x_adachi][y_adachi];
		myprintf("%d,%d,%d,%d\n", x_adachi, y_adachi, q.head, q.tail);
	} while (q.tail != q.head);

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
			pass(accel, vel);
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

