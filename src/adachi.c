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
#include "speaker.h"
#include "motion.h"

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
			step_map[x_adachi][y_adachi] = 0xffff;
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
	} else if (goal_scale == 100) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_y < 15) {
			step_map[goal_x][goal_y + 1] = 0;
			x_adachi = goal_x;
			y_adachi = goal_y + 1;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 101) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_x > 0) {
			step_map[goal_x - 1][goal_y] = 0;
			x_adachi = goal_x - 1;
			y_adachi = goal_y;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 102) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_y > 0) {
			step_map[goal_x][goal_y - 1] = 0;
			x_adachi = goal_x;
			y_adachi = goal_y - 1;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 103) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_x < 15) {
			step_map[goal_x + 1][goal_y] = 0;
			x_adachi = goal_x + 1;
			y_adachi = goal_y;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
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

uint8_t how_to_move_pass(uint8_t direction, int8_t x, int8_t y,
		walldate_t walldate, uint8_t before_flag) {
	uint8_t step, flag = 255, i;
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
//	if ((y - a >= 0) && (x + b < 16)) {
//		if ((getWall(x, y, (North + i) % 4, &walldate) == 1)
//				&& (getWall(x, y, (East + i) % 4, &walldate) == 1)
//				&& (getWall(x, y, (West + i) % 4, &walldate) == 1)) {
//			flag = 2;
//		} else {
//			flag = 4;
//		}
//		step = step_map[x + b][y - a];
//	}
	step = step_map[x + b][y - a];
	if ((x - a >= 0) && (y - b >= 0)
			&& (getWall(x, y, (West + i) % 4, &walldate) == 0)) {
		if (step_map[x - a][y - b] <= step) {
			flag = 1;
			step = step_map[x - a][y - b];
		}
	}
	if ((x + a <= 15) && (y + b <= 15)
			&& (getWall(x, y, (East + i) % 4, &walldate) == 0)) {
		myprintf("(%d,%d),dire:%d,flag:%d,before_flag:%d\n", x, y, direction,
				flag, before_flag);
		if (step_map[x + a][y + b] == step && flag == 1) {
			if (before_flag == 3) {
				flag = 1;
				myprintf("flag=1\n");
			} else {
				flag = 3;
				step = step_map[x + a][y + b];
				myprintf("flag=3\n");
			}
		} else if (step_map[x + a][y + b] <= step) {
			flag = 3;
			step = step_map[x + a][y + b];
			myprintf("flag=33\n");
		}
	}
	if ((y + a <= 15) && (x - b >= 0)
			&& (getWall(x, y, (North + i) % 4, &walldate) == 0)) {
		if (step_map[x - b][y + a] <= step) {
			flag = 0;
			step = step_map[x - b][y + a];
		}
	}
	myprintf("last%d\n", flag);
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
			set_straight(180.0 * (flag - 10), 5000.0, 1500.0, vel, vel);
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

void search_run_special(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale) {
	uint16_t Next_XY_16bit;
	uint8_t x_local, y_local, wall_direction, Next_XY_8bit;

	if (goal_scale == 4) {
		x.now = goal_x;
		y.now = goal_y;
		add_East_wall(&walldate_checked);
		add_North_wall(&walldate_checked);
		pull_East_wall(&walldate_adachi);
		pull_North_wall(&walldate_adachi);
		x.now = goal_x + 1;
		y.now = goal_y + 1;
		add_West_wall(&walldate_checked);
		add_South_wall(&walldate_checked);
		pull_West_wall(&walldate_adachi);
		pull_South_wall(&walldate_adachi);
	}

	x.now = 0;
	y.now = 0;
	direction = 0;
	special_goal_flag = 0;
	add_wall_flag = 1;
	go_entrance(nomal_run.accel, nomal_run.vel_search);
	coordinate();
	addWall();
	while (failsafe_flag == 0) {

//		adachi_map(goal_x, goal_y, goal_scale, walldate_real); //歩数マップ展開
		Next_XY_16bit = make_temporary_goal_XY(goal_x, goal_y, goal_scale); //見たい壁の位置からゴールを算出
		if (Next_XY_16bit == 0xffff) {
//			speaker_on( C_4, 6.0, 500);
			break;
		}
		wall_direction = check_flag(Next_XY_16bit);
		Next_XY_8bit = (uint8_t) (Next_XY_16bit & 0x00ff);
		y_local = Next_XY_8bit / 16;
		x_local = Next_XY_8bit % 16;

		if (wall_direction == 10) {
//			speaker_on( C_4, 6.0, 500);
			failsafe_flag = 1;
		}

		adachi_special_move(x_local, y_local, wall_direction, nomal_run.accel,
				nomal_run.vel_search, (goal_y * 16 + goal_x), goal_scale,
				Next_XY_16bit); //見たい壁に対する位置と壁の絶対方角を入力
		speaker_on( C_4, 6.0, 1000);
	}
	/////////////////////////
	failsafe_flag = 1;
	speaker_on( C_4, 6.0, 700);
	/////////////////////////////////////
	u_turn_counter = 100;
//	speaker_on( D_4, 6.0, 1200);
//	add_wall_flag = 0;
	if (special_goal_flag == 0) {
//		speaker_on( C_5, 6.0, 700);
		adachi_special_move(goal_x, goal_y, goal_scale, nomal_run.accel,
				nomal_run.vel_search, (goal_y * 16 + goal_x), goal_scale,
				0xffff);
	}
	if (goal_scale == 1) {
		go_entrance(nomal_run.accel, nomal_run.vel_search);
		coordinate();
		addWall();
	}
//	speaker_on( C_7, 6.0, 1000);
	u_turn_counter = 5;
	adachi_special_move(0, 0, 255, nomal_run.accel, nomal_run.vel_search,
			(goal_y * 16 + goal_x), goal_scale, 0xffff);

}

uint16_t make_temporary_goal_XY(uint8_t ture_goal_x, uint8_t ture_goal_y,
		uint8_t goal_scale) {
	uint8_t flag = 0;
	uint8_t x_local = 0, y_local = 0, local_direction = 0; //常にスタート座標からゴールへの道を探す
	uint16_t Next_XY = 0, shift = 1;

	adachi_map(ture_goal_x, ture_goal_y, goal_scale, walldate_real); //歩数マップ展開

	while (failsafe_flag == 0) {
		flag = how_to_move_special(x_local, y_local, local_direction);
//		myprintf("flag:%d.x:%d,y:%d,dire:%d\n",flag,x_local,y_local,local_direction);

		if (flag > 9) { //相対方角から絶対方角に変換
			flag -= 10;
			flag += local_direction;
			flag %= 4;
			flag += 10;
//			speaker_on( C_4, 6.0, 500);
			break;
		}
		if (flag == 1) {
			local_direction++;
		} else if (flag == 2) {
			local_direction += 2;
		} else if (flag == 3) {
			local_direction += 3;
		}
		if (local_direction > 3) {
			local_direction -= 4;
		}

		if (local_direction == North && y_local < 15) {
			y_local++;
		} else if (local_direction == West && x_local > 0) {
			x_local--;
		} else if (local_direction == South && y_local > 0) {
			y_local--;
		} else if (local_direction == East && x_local < 15) {
			x_local++;
		}

		if ((goal_scale == 1)
				&& ((x_local == ture_goal_x && y_local == ture_goal_y))) { //経路内に未知壁なし
			shift = 255;
			break;
		}
		if ((goal_scale == 4)
				&& ((x_local == ture_goal_x && y_local == ture_goal_y)
						|| (x_local == ture_goal_x + 1 && y_local == ture_goal_y)
						|| (x_local == ture_goal_x && y_local == ture_goal_y + 1)
						|| (x_local == ture_goal_x + 1
								&& y_local == ture_goal_y + 1))) { //経路内に未知壁なし
			shift = 255;
			break;
		}
	}

	if (shift == 1) { //まだ探索が必要
		Next_XY = 0;
		Next_XY = (uint16_t) (16 * y_local + x_local);
		shift = shift << flag;
		Next_XY = Next_XY | shift;
	}

	if (shift == 255) { //経路内に未知壁なし
		Next_XY = 0xffff;
	}
//	myprintf("%d\n",Next_XY);
	return Next_XY; //一時的なゴール座標を返す
}

uint8_t how_to_move_special(uint8_t x, uint8_t y, uint8_t direction) {
	uint16_t step;
	int8_t a, b;
	uint8_t flag = 111, i;
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

	step = step_map[x][y];
	if (step == 0xffff) {
		stop90(7000.0, 600.0);
		failsafe_flag = 1;
	}

	if ((x - a >= 0) && (y - b >= 0)
			&& (getWall(x, y, (West + i) % 4, &walldate_real) == 0)) {
		if (step_map[x - a][y - b] <= step) {
			step = step_map[x - a][y - b];
			if ((getWall(x, y, (West + i) % 4, &walldate_checked) == 1)) {
				flag = 1; //checkしてるからすすめる
			} else {
				flag = 11; //checkしてないからここを確認しにく
			}
		}
	}
	if ((x + a <= 15) && (y + b <= 15)
			&& (getWall(x, y, (East + i) % 4, &walldate_real) == 0)) {
		if (step_map[x + a][y + b] <= step) {
			step = step_map[x + a][y + b];
			if ((getWall(x, y, (East + i) % 4, &walldate_checked) == 1)) {
				flag = 3;
			} else {
				flag = 13;
			}
		}
	}
	if ((y + a <= 15) && (x - b >= 0)
			&& (getWall(x, y, (North + i) % 4, &walldate_real) == 0)) {
		if (step_map[x - b][y + a] <= step) {
			step = step_map[x - b][y + a];
			if ((getWall(x, y, (North + i) % 4, &walldate_checked) == 1)) {
				flag = 0;
			} else {
				flag = 10;
			}
		}
	}
	return flag; //進行方向と未知壁かの情報を返す
}

void adachi_special_move(uint8_t goal_x, uint8_t goal_y, uint8_t wall_direction,
		float accel, float vel, uint8_t ture_goal, uint8_t ture_goal_scale,
		uint16_t input) {
	uint8_t flag, i; //flag 0:前,1:左折2:Uターン(けつあて)3:右折4:Uターン
	uint8_t goal_scale = 1;
	uint8_t straight_flag = 0;
	uint8_t slalom_flag = 1;
	uint16_t NEW_XY_16;

	if (wall_direction == 0) { //座標と壁の方角の関係から2つのゴール座標を設定
		goal_scale = 100;
	} else if (wall_direction == 1) {
		goal_scale = 101;
	} else if (wall_direction == 2) {
		goal_scale = 102;
	} else if (wall_direction == 3) {
		goal_scale = 103;
	} else if (wall_direction == 255) { //スタートに帰宅
		goal_scale = 1;
	} else if (wall_direction == 4) { //ゴールに入っておくとき
		goal_scale = 4;
	}

	while (failsafe_flag == 0) {
		NEW_XY_16 = make_temporary_goal_XY((ture_goal % 16), (ture_goal / 16),
				ture_goal_scale);

		if ((NEW_XY_16 != input) && (input != 0xffff)) {
			break;
		}
		if ((NEW_XY_16 != input) && (input == 0xffff)) {
			failsafe_flag = 1;
			break;
		}

		if (ture_goal_scale == 4) {
			if ((x.now == (ture_goal % 16) && y.now == (ture_goal / 16))
					|| (x.now == (ture_goal % 16) + 1
							&& y.now == (ture_goal / 16))
					|| (x.now == (ture_goal % 16)
							&& y.now == (ture_goal / 16) + 1)
					|| (x.now == (ture_goal % 16) + 1
							&& y.now == (ture_goal / 16) + 1)) {
				special_goal_flag = 1;
//				failsafe_flag=1;
//				write_all_walldatas();
			}
		}

		if (straight_flag == 1) {
			adachi_map_straight(goal_x, goal_y, goal_scale, walldate_real);
		} else {
			if (wall_direction == 255) {
				adachi_map_special(goal_x, goal_y, goal_scale, walldate_real);
			} else {
				adachi_map_special(goal_x, goal_y, goal_scale, walldate_real);
			}
		}
		if (step_map[x.now][y.now] == 0xffff) {
			stop90(accel, vel);
			failsafe_flag = 1;
			break;
		}

		if ((goal_scale == 1) && ((x.now == goal_x && y.now == goal_y))) { //帰宅時
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
		} else if ((goal_scale > 99 || goal_scale == 4)
				&& step_map[x.now][y.now] == 0) {
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
				slalom_left90(accel, vel);
			}
		}
		if (flag == 2) {
			ketuate(accel, vel);
		}
		if (flag == 3) {
			if (slalom_flag == 0) {
				turn_right(accel, vel);
			} else {
				slalom_right90(accel, vel);
			}
		}

		if (flag == 4) {
			turn_180(accel, vel);
		}
		if (flag > 11) {
			set_straight(180.0 * (flag - 10), 5000.0, 1400.0, vel, vel);
			wait_straight();
			for (i = 0; i < flag - 10 - 1; i++) {
				coordinate();
			}
		}
		coordinate();
		addWall();
	}
}

uint8_t check_flag(uint16_t input) {
//	uint16_t shift = 1;
	uint8_t flag = 10;
	if ((input & (0x400)) != 0) {
		flag = 0;
	}
	if ((input & (0x800)) != 0) {
		flag = 1;
	}
	if ((input & (0x1000)) != 0) {
		flag = 2;
	}
	if ((input & (0x2000)) != 0) {
		flag = 3;
	}

	return flag;
}

void stop_move(float accel, float vel) {
	if (getWall(x.now, y.now, direction + 1, &walldate_real)) {
		set_straight(95.0, accel, vel, vel, 0.0);
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
		set_straight(95.0, accel, vel, vel, 0.0);
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
		set_straight(93.0, accel, vel, vel, 0.0);
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

void adachi_map_special(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		walldate_t walldate) {
	uint8_t x_adachi, y_adachi, step;
	queue_t q;
	uint8_t unknown_priority = 0;
	uint8_t straight_priority = 0;
	uint8_t nomal_value = 5;
	uint8_t flag = 0;

	if (goal_x == 0 && goal_y == 0) {
		straight_priority = 4;
	}

//	moter_flag = 1;
	q.head = 0;
	q.tail = 0;

	for (x_adachi = 0; x_adachi < 16; x_adachi++) {
		for (y_adachi = 0; y_adachi < 16; y_adachi++) {
			step_map[x_adachi][y_adachi] = 0xffff;
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
	} else if (goal_scale == 100) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_y < 15) {
			step_map[goal_x][goal_y + 1] = 0;
			x_adachi = goal_x;
			y_adachi = goal_y + 1;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 101) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_x > 0) {
			step_map[goal_x - 1][goal_y] = 0;
			x_adachi = goal_x - 1;
			y_adachi = goal_y;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 102) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_y > 0) {
			step_map[goal_x][goal_y - 1] = 0;
			x_adachi = goal_x;
			y_adachi = goal_y - 1;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
	} else if (goal_scale == 103) {
		step_map[goal_x][goal_y] = 0;
		x_adachi = goal_x;
		y_adachi = goal_y;
		step = 0;
		queue_push(&q, x_adachi, y_adachi);
		if (goal_x < 15) {
			step_map[goal_x + 1][goal_y] = 0;
			x_adachi = goal_x + 1;
			y_adachi = goal_y;
			step = 0;
			queue_push(&q, x_adachi, y_adachi);
		}
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

//		if ((getWall(x_adachi, y_adachi, North, &walldate) == 0)
//				&& (step_map[x_adachi][y_adachi + 1] > step + 1)
//				&& ((y_adachi + 1) < 16)) {
//			if ((getWall(x_adachi, y_adachi, North, &walldate_checked) == 0)) {
//				step_map[x_adachi][y_adachi + 1] = step + 1;
//			} else {
//				step_map[x_adachi][y_adachi + 1] = step + unknown_priority;
//			}
//
//			queue_push(&q, x_adachi, y_adachi + 1);
//			flag = 10;
//		}
//		if ((getWall(x_adachi, y_adachi, West, &walldate) == 0)
//				&& (step_map[x_adachi - 1][y_adachi] > step + 1)
//				&& ((x_adachi - 1) >= 0)) {
//			if ((getWall(x_adachi, y_adachi, West, &walldate_checked) == 0)) {
//				step_map[x_adachi - 1][y_adachi] = step + 1;
//			} else {
//				step_map[x_adachi - 1][y_adachi] = step + unknown_priority;
//			}
//			queue_push(&q, x_adachi - 1, y_adachi);
//			flag = 10;
//		}
//		if ((getWall(x_adachi, y_adachi, South, &walldate) == 0)
//				&& (step_map[x_adachi][y_adachi - 1] > step + 1)
//				&& ((y_adachi - 1) >= 0)) {
//			if ((getWall(x_adachi, y_adachi, South, &walldate_checked) == 0)) {
//				step_map[x_adachi][y_adachi - 1] = step + 1;
//			} else {
//				step_map[x_adachi][y_adachi - 1] = step + unknown_priority;
//			}
//			queue_push(&q, x_adachi, y_adachi - 1);
//			flag = 10;
//		}
//		if ((getWall(x_adachi, y_adachi, East, &walldate) == 0)
//				&& (step_map[x_adachi + 1][y_adachi] > step + 1)
//				&& ((x_adachi + 1) < 16)) {
//			if ((getWall(x_adachi, y_adachi, East, &walldate_checked) == 0)) {
//				step_map[x_adachi + 1][y_adachi] = step + 1;
//			} else {
//				step_map[x_adachi + 1][y_adachi] = step + unknown_priority;
//			}
//			queue_push(&q, x_adachi + 1, y_adachi);
//			flag = 10;
//		}
///////////////////////////////////////////////////////////////////
		if ((getWall(x_adachi, y_adachi, North, &walldate) == 0)
				&& ((y_adachi + 1) < 16)) {
			if (((step_map[x_adachi][y_adachi - 1] < step)
					&& (step_map[x_adachi][y_adachi - 1] >= step - nomal_value))
					&& ((y_adachi - 1) >= 0)
					&& getWall(x_adachi, y_adachi, South, &walldate_adachi) == 0
					&& step_map[x_adachi][y_adachi + 1]
							> step + nomal_value - straight_priority + 1) {
				step_map[x_adachi][y_adachi + 1] = step + nomal_value
						- straight_priority;
				flag = 1;
			}
			if ((getWall(x_adachi, y_adachi, North, &walldate_checked) == 0)
					&& (step_map[x_adachi][y_adachi + 1]
							> step + nomal_value - unknown_priority + 1)) {
				step_map[x_adachi][y_adachi + 1] = step + nomal_value
						- unknown_priority;
				flag = 1;
			}
			if (step_map[x_adachi][y_adachi + 1] > step + nomal_value + 1
					&& flag == 0) {
				step_map[x_adachi][y_adachi + 1] = step + nomal_value;
				flag = 1;
			}
			if (flag == 1) {
				queue_push(&q, x_adachi, y_adachi + 1);
				flag = 0;
			}
		}
		if ((getWall(x_adachi, y_adachi, West, &walldate) == 0)
				&& ((x_adachi - 1) >= 0)) {
			if (((step_map[x_adachi + 1][y_adachi] < step)
					&& (step_map[x_adachi + 1][y_adachi] >= step - nomal_value))
					&& ((x_adachi + 1) < 16)
					&& (getWall(x_adachi, y_adachi, East, &walldate_adachi) == 0)
					&& (step_map[x_adachi - 1][y_adachi]
							> step + nomal_value - straight_priority + 1)) {
				step_map[x_adachi - 1][y_adachi] = step + nomal_value
						- straight_priority;
				flag = 1;
			}
			if ((getWall(x_adachi, y_adachi, West, &walldate_checked) == 0)
					&& (step_map[x_adachi - 1][y_adachi]
							> step + nomal_value - unknown_priority + 1)) {
				step_map[x_adachi - 1][y_adachi] = step + nomal_value
						- unknown_priority;
				flag = 1;
			}
			if (step_map[x_adachi - 1][y_adachi] > step + nomal_value + 1
					&& flag == 0) {
				step_map[x_adachi - 1][y_adachi] = step + nomal_value;
				flag = 1;
			}

			if (flag == 1) {
				queue_push(&q, x_adachi - 1, y_adachi);
				flag = 0;
			}
		}
		if ((getWall(x_adachi, y_adachi, South, &walldate) == 0)
				&& ((y_adachi - 1) >= 0)) {
			if (((step_map[x_adachi][y_adachi + 1] < step)
					&& (step_map[x_adachi][y_adachi + 1] >= step - nomal_value))
					&& (y_adachi + 1) < 16
					&& getWall(x_adachi, y_adachi, North, &walldate_adachi) == 0
					&& step_map[x_adachi][y_adachi - 1]
							> step + nomal_value - straight_priority + 1) {
				step_map[x_adachi][y_adachi - 1] = step + nomal_value
						- straight_priority;
				flag = 1;
			}
			if ((getWall(x_adachi, y_adachi, South, &walldate_checked) == 0)
					&& step_map[x_adachi][y_adachi - 1]
							> step + nomal_value - unknown_priority + 1) {
				step_map[x_adachi][y_adachi - 1] = step + nomal_value
						- unknown_priority;
				flag = 1;
			}
			if (step_map[x_adachi][y_adachi - 1] > step + nomal_value + 1
					&& flag == 0) {
				step_map[x_adachi][y_adachi - 1] = step + nomal_value;
				flag = 1;
			}
			if (flag == 1) {
				queue_push(&q, x_adachi, y_adachi - 1);
				flag = 0;
			}
		}
		if ((getWall(x_adachi, y_adachi, East, &walldate) == 0)
				&& (step_map[x_adachi + 1][y_adachi] > step + 1)
				&& ((x_adachi + 1) < 16)) {
			if (((step_map[x_adachi - 1][y_adachi] < step)
					&& (step_map[x_adachi - 1][y_adachi] >= step - nomal_value))
					&& (x_adachi - 1) >= 0
					&& getWall(x_adachi, y_adachi, West, &walldate_adachi) == 0
					&& step_map[x_adachi + 1][y_adachi]
							> step + nomal_value - straight_priority + 1) {
				step_map[x_adachi + 1][y_adachi] = step + nomal_value
						- straight_priority;
				flag = 1;
			}
			if ((getWall(x_adachi, y_adachi, East, &walldate_checked) == 0)
					&& step_map[x_adachi + 1][y_adachi]
							> step + nomal_value - unknown_priority + 1) {
				step_map[x_adachi + 1][y_adachi] = step + nomal_value
						- unknown_priority;
				flag = 1;
			}
			if (step_map[x_adachi + 1][y_adachi] > step + nomal_value + 1
					&& flag == 0) {
				step_map[x_adachi + 1][y_adachi] = step + nomal_value;
				flag = 1;
			}
			if (flag == 1) {
				queue_push(&q, x_adachi + 1, y_adachi);
				flag = 0;
			}
		}
//		myprintf("%d,%d,%d\n", x_adachi, y_adachi, step);
	} while (q.tail != q.head);
}

