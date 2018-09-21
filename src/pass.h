/*
 * pass.h
 *
 *  Created on: 2018/08/24
 *      Author: takao keisuke
 */

#ifndef PASS_H_
#define PASS_H_

#include "stdint.h"

void make_pass(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale, uint8_t);
void move_pass(float accel, float vel);
void move_pass_compression(float accel, float vel);
void move_pass_big_turn(float accel, float vel,float);
void move_pass_oblique(float accel, float max_vel, float big_turn_vel);
void out_put_pass(uint8_t *pass);
//void make_pass_big_turn(void);
#endif /* PASS_H_ */
