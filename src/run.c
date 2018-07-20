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

void real_velocity_control(void) {
	encoder_L = TPU1.TCNT; //L
	encoder_R = TPU2.TCNT; //R

	left_real.velocity = 1 * (float) encoder_L * diameter * 3.14159265359 / 4096
			/ 4 / 40 * 11 / 0.001;
	right_real.velocity = -1 * (float) encoder_R * diameter * 3.14159265359
			/ 4096 / 4 / 40 * 11 / 0.001;

	TPU1.TCNT = 0;
	TPU2.TCNT = 0;
}

void duty_to_moter(void) {
	MTU0.TGRB = (duty.rghit * 4); //MOTER_R
	MTU0.TGRD = (duty.left * 4); //MOTER_L
}

void PID_control(run_t *ideal, run_t *left, run_t *right,
		deviation_t *left_deviation, deviation_t *right_deviation, gain_t *gain,
		duty_t *duty) {
	left_deviation->now = (int) (ideal->velocity - left->velocity);
	right_deviation->now = (int) (ideal->velocity - right->velocity);
	left_deviation->cumulative += left_deviation->now;
	right_deviation->cumulative += right_deviation->now;
	duty->left = left_deviation->now * gain->Kp
			+ left_deviation->cumulative * gain->Ki;
	duty->rghit = right_deviation->now * gain->Kp
			+ right_deviation->cumulative * gain->Ki;

	if (duty->left > 100) {
		duty->left = 100;
	}
	if (duty->rghit > 100) {
		duty->rghit = 100;
	}
	if (duty->left < 0) {
		duty->left = 0;
	}
	if (duty->rghit < 0) {
		duty->rghit = 0;
	}
}

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel) {
	trapezoid_preparation(&translation_parameter, i_distance, accel, max_vel,
			strat_vel, end_vel);

	translation_ideal.velocity = translation_parameter.strat_vel;
	translation_parameter.run_flag = 1;
	log_start();

}

void wait_straight(void) {
	volatile int i;
	LEFTEING = 1;
	while (translation_parameter.run_flag == 1) {
		i++;
	}
	LEFTFRONT = 1;
	translation_ideal.accel = 0.0;
	translation_ideal.dis = 0.0;
	translation_ideal.velocity = 0.0;
	duty.left = 0;
	duty.rghit = 0;
	duty_to_moter();

}

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel) {
//任意のパラメータから台形加速の概要を計算
//trapezoidはポインタでとってる

	trapezoid->accel = accel;
	trapezoid->end_vel = end_vel;
	trapezoid->i_distance = i_distance;
	trapezoid->max_vel = max_vel;
	trapezoid->strat_vel = strat_vel;
//	trapezoid->triangle = triangle;
//	trapezoid->triangle_flag = triangle_flag;
//↑任意の値を代入してる

	if (accel != 0.0) {

		trapezoid->acceldistance = (max_vel * max_vel - strat_vel * strat_vel)
				/ (2.0 * accel);
		trapezoid->deacceldistance = (max_vel * max_vel - end_vel * end_vel)
				/ (2.0 * accel);
		while (i_distance
				< trapezoid->acceldistance + trapezoid->deacceldistance) {
			max_vel = max_vel * 0.95;
			trapezoid->acceldistance = (max_vel * max_vel
					- strat_vel * strat_vel) / (2.0 * accel);
			trapezoid->deacceldistance = (max_vel * max_vel - end_vel * end_vel)
					/ (2.0 * accel);
		}

	} else {
		trapezoid->acceldistance = 0.0;
		trapezoid->deacceldistance = 0.0;
	}

}

void control_accel(run_t *ideal, trapezoid_t *trapezoid) {
	if (ideal->dis < trapezoid->deacceldistance) {
		UI_LED1 = 1;
		if (ideal->velocity < trapezoid->max_vel) {
			ideal->accel = trapezoid->accel;
		} else {
			ideal->velocity = trapezoid->max_vel;
			ideal->accel = 0;
		}

	} else if (ideal->dis
			< (trapezoid->i_distance - trapezoid->deacceldistance)) {
		UI_LED2 = 1;
		ideal->accel = 0;

	} else if (ideal->dis < trapezoid->i_distance) {
		UI_LED3 = 1;
		if (ideal->velocity > trapezoid->end_vel) {
			ideal->accel = -trapezoid->accel;
		} else {
			ideal->velocity = trapezoid->end_vel;
			ideal->accel = 0;
		}
	} else {
		trapezoid->run_flag = 0;
		RIGHTWING = 1;
	}

}

void integral(run_t *ideal) {
	ideal->velocity += ideal->accel * 0.001;
	ideal->dis += ideal->velocity * 0.001 + ideal->accel * 0.001 * 0.001 / 2;
}

void integral_vel_to_dis(float *velocity, float *distance) {
	*distance += *velocity * 0.001;
}
