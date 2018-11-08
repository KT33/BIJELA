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

void adachi_map(uint8_t goal_x, uint8_t goal_y, uint8_t, walldate_t);
void adachi_search_run(uint8_t goal_x, uint8_t goal_y, uint8_t, float accel,
		float vel, uint8_t, uint8_t);
uint8_t how_to_move(uint8_t direction, int8_t x, int8_t y, walldate_t);
void adachi_search_run_known(uint8_t goal_x, uint8_t goal_y, uint8_t,
		float accel, float vel, uint8_t, uint8_t);
uint8_t how_to_move_search_known(uint8_t direction, int8_t x, int8_t y);
void search_run_special(uint8_t goal_x,uint8_t goal_y,uint8_t goal_scale);
uint16_t make_temporary_goal_XY(uint8_t ,uint8_t,uint8_t);
uint8_t how_to_move_special(uint8_t x,uint8_t y,uint8_t direction);
void adachi_special_move(uint8_t goal_x, uint8_t goal_y,uint8_t, float accel,
		float vel);
uint8_t check_flag(uint16_t);
void stop_move(float accel,float vel);

#endif /* ADACHI_H_ */

