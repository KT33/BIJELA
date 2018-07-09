/*
 * run.h
 *
 *  Created on: 2018/07/06
 *      Author: takao keisuke
 */

#ifndef RUN_H_
#define RUN_H_

#include "stdint.h"
#include "variable.h"

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel);

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel);

void control_accel(run_t *ideal,trapezoid_t *trapezoid);

void integral(run_t *ideal);

#endif /* RUN_H_ */
