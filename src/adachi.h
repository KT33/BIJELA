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
void non_ketuate_goal(float accel, float vel);
void ketuate_right(float accel, float vel);
void ketuate_left(float accel, float vel);
void adachi_map(uint8_t goal_x, uint8_t goal_y, uint8_t, walldate_t);
void adachi_search_run(uint8_t goal_x, uint8_t goal_y, uint8_t, float accel,
		float vel, uint8_t,uint8_t);
uint8_t how_to_move(uint8_t direction, int8_t x, int8_t y, walldate_t);
void slalom_left90(float run_accel, float run_vel);
void slalom_right90(float run_accel, float run_vel);
void ketuate(float accel, float vel);
void adachi_map_straight(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		walldate_t walldate);
void stop90(float run_accel, float run_vel);
void slalom_left_check(float accel,float vel);
void slalom_right_check(float accel,float vel);
void turn_left_180_big(float vel);
void turn_right_180_big(float vel);
void turn_left_90_big(float vel);
void turn_right_90_big(float vel);
void farst_turn_right_90_big(float vel);
void turn_right_45(float vel);
void turn_left_45(float vel);
void turn_right_135(float vel);
void turn_left_135(float vel);

#endif /* ADACHI_H_ */

