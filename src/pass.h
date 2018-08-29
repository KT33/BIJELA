/*
 * pass.h
 *
 *  Created on: 2018/08/24
 *      Author: takao keisuke
 */

#ifndef PASS_H_
#define PASS_H_

#include "stdint.h"


void make_pass(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale);
void move_pass(float accel, float vel);
void move_pass_compression(float accel, float vel);
#endif /* PASS_H_ */
