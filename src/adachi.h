/*
 * adachi.h
 *
 *  Created on: 2018/08/13
 *      Author: takao keisuke
 */

#ifndef ADACHI_H_
#define ADACHI_H_

#include "stdint.h"

void right_hand(float accel,float vel);
void go_entrance(float accel, float vel);
void go_center(float,float);
void turn_left(float accel,float vel);
void turn_right(float accel, float vel);
void pass(float accel, float vel);
void turn_180(float accel, float vel);
void back_100(void);
void ketuate_right(float accel, float vel);
void ketuate_left(float accel, float vel);
void adachi_map(uint8_t goal_x,uint8_t goal_y);
#endif /* ADACHI_H_ */
