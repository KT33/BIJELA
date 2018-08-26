/*
 * adachi.h
 *
 *  Created on: 2018/08/13
 *      Author: takao keisuke
 */

#ifndef ADACHI_H_
#define ADACHI_H_

#include "stdint.h"
#include "variable.h"

void right_hand(float accel, float vel);
void go_entrance(float accel, float vel);
void go_center(float, float);
void turn_left(float accel, float vel);
void turn_right(float accel, float vel);
void pass_180(float accel, float vel);
void turn_180(float accel, float vel);
void back_100(void);
void ketuate_goal_left(float accel, float vel);
void ketuate_goal_right(float accel, float vel);
void ketuate_goal(float accel, float vel);
void ketuate_right(float accel, float vel);
void ketuate_left(float accel, float vel);
void adachi_map(uint8_t goal_x, uint8_t goal_y, walldate_t);
void adachi_search_run(uint8_t goal_x, uint8_t goal_y, float accel, float vel);
uint8_t how_to_move(uint8_t direction, int8_t x, int8_t y,walldate_t);
void make_pass(uint8_t,uint8_t);
#endif /* ADACHI_H_ */

