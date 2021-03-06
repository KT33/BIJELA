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

void control_accel(run_t *ideal, trapezoid_t *trapezoid,uint8_t);

void integral(run_t *ideal);

void wait_straight(void);

void PID_control(run_t *ideal, run_t *left, run_t *right, deviation_t *,
		deviation_t *, gain_t *, trapezoid_t*,duty_t *duty,uint8_t);
void duty_to_moter(void);

void integral_vel_to_dis(float *velocity, float *distance);

void set_rotation(float i_angle, float accel, float max_vel,float);
void wait_rotation(void);
void wall_control(void);
void coordinate(void);
void coordinate_pass(void);

#endif /* RUN_H_ */
