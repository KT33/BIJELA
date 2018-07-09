/*
 * run.c
 *
 *  Created on: 2018/07/06
 *      Author: takao keisuke
 */

#include "run.h"
#include "variable.h"
#include "stdint.h"
#include "other.h"
#include "iodefine.h"

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel) {
	trapezoid_preparation(&translation_parameter, i_distance,accel,
			max_vel,strat_vel, end_vel);

	translation_ideal.velocity=translation_parameter.strat_vel;

}

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel) {
//任意のパラメータから台形加速の概要を計算
//trapezoidはポインタでとってる
	int8_t triangle_flag = 0;

	trapezoid->accel = accel;
	trapezoid->end_vel = end_vel;
	trapezoid->i_distance = i_distance;
	trapezoid->max_vel = max_vel;
	trapezoid->strat_vel = strat_vel;
	trapezoid->triangle = triangle;
	trapezoid->triangle_flag = triangle_flag;
//↑任意の値を代入してる

	if (accel != 0.0) {
		trapezoid->acceldistance = (max_vel * max_vel - strat_vel * strat_vel)
				/ (2.0 * accel);
		trapezoid->deacceldistance = (max_vel * max_vel - end_vel * end_vel)
				/ (2.0 * accel);
	} else {
		trapezoid->acceldistance = 0.0;
		trapezoid->deacceldistance = 0.0;
	}

	trapezoid->run_flag = 1;
}

void control_accel(run_t *ideal, trapezoid_t *trapezoid) {
	if (ideal->dis < trapezoid->deacceldistance) {
		UI_LED1=1;
		if (ideal->velocity < trapezoid->max_vel) {
			ideal->accel = trapezoid->accel;
		} else {
			ideal->velocity = trapezoid->max_vel;
			ideal->accel = 0;
		}

	} else if (ideal->dis
			< (trapezoid->i_distance - trapezoid->deacceldistance)) {
		UI_LED2=1;
		ideal->accel = 0;

	} else if (ideal->dis < trapezoid->i_distance) {
		UI_LED3=1;
		if (ideal->velocity > trapezoid->min_vel) {
			ideal->accel = -trapezoid->accel;
		} else {
			ideal->velocity = trapezoid->min_vel;
			ideal->accel = 0;
		}
	}
//	UI_LED1=0;
//	UI_LED2=0;
//	UI_LED3=0;
}

void integral(run_t *ideal) {
	ideal->velocity += ideal->accel * 0.001;
	ideal->dis += ideal->velocity * 0.001 + ideal->accel * 0.001 * 0.001 / 2;
}
