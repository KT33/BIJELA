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

void control_accel(run_t *ideal, trapezoid_t *trapezoid);

void integral(run_t *ideal);

void wait_straight(void);

void PID_control(run_t *ideal, run_t *left, run_t *right, deviation_t *,
		deviation_t *,gain_t*, duty_t *duty);
void duty_to_moter(void);
void real_velocity_control(void);
void integral_vel_to_dis(float *velocity, float *distance);

#endif /* RUN_H_ */
