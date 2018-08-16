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

void wall_control_to_duty(duty_t *duty) {
	int duty_value;
	if ((((left_real.velocity + right_real.velocity) / 2) > 30)
			&& (SEN_L.diff < 30) && (SEN_R.diff < 30)
			&& (SEN_F.now < SEN_F.threshold)) { //&& (SEN_L.diff < 2000) && (SEN_R.diff < 2000)&& (SEN_F.now < SEN_F.threshold * 100))
		if (SEN_L.now > SEN_L.threshold && SEN_R.now > SEN_R.threshold) {
			duty_value = wall_cntrol_gain.Kp
					* ((SEN_L.now - SEN_L.reference)
							- (SEN_R.now - SEN_R.reference));
//			RIGHTFRONT = 1;
//			LEFTFRONT = 1;
//			CENTERFRONT = 0;
		} else if (SEN_L.now < SEN_L.threshold && SEN_R.now > SEN_R.threshold) {
			duty_value = -2 * wall_cntrol_gain.Kp
					* (SEN_R.now - SEN_R.reference);
//			RIGHTFRONT = 1;
//			LEFTFRONT = 0;
//			CENTERFRONT = 0;
		} else if (SEN_L.now > SEN_L.threshold && SEN_R.now < SEN_R.threshold) {
			duty_value = 2 * wall_cntrol_gain.Kp
					* (SEN_L.now - SEN_L.reference);
//			RIGHTFRONT = 0;
//			LEFTFRONT = 1;
//			CENTERFRONT = 0;
		} else {
			duty_value = 0;
//			RIGHTFRONT = 0;
//			LEFTFRONT = 0;
//			CENTERFRONT = 0;
		}
	} else {
		duty_value = 0;
//		RIGHTFRONT = 0;
//		LEFTFRONT = 0;
//		CENTERFRONT = 1;
	}
	duty->left += duty_value;
	duty->right += -1 * duty_value;

}

void duty_to_moter(void) {
	int duty_left = 0, duty_right = 0;

//	if (translation_parameter.back_rightturn_flag == 1) {
//		duty.left = duty.left * -1;
//		duty.right = duty.right * -1;
//	}

	if (duty.left >= 0) {
		Moter_L_FRONT = 1;
		Moter_L_BACK = 0;
		duty_left = duty.left;
	} else {
		Moter_L_FRONT = 0;
		Moter_L_BACK = 1;
		duty_left = (duty.left * -1);
	}
	if (duty.right >= 0) {
		Moter_R_FRONT = 1;
		Moter_R_BACK = 0;
		duty_right = duty.right;
	} else {
		Moter_R_FRONT = 0;
		Moter_R_BACK = 1;
		duty_right = (duty.right * -1);
	}

	if (duty_left > 100) {
		duty_left = 100;
	}
	if (duty_right > 100) {
		duty_right = 100;
	}
	if (duty_left < -100) {
		duty_left = -100;
	}
	if (duty_right < -100) {
		duty_right = -100;
	}

	test1 = duty_left;
	test2 = duty_right;

	MTU0.TGRB = (duty_right * 4); //MOTER_R
	MTU0.TGRD = (duty_left * 4); //MOTER_L

	duty.left = 0;
	duty.right = 0;
}

void PID_control(run_t *ideal, run_t *left, run_t *right,
		deviation_t *left_deviation, deviation_t *right_deviation, gain_t *gain,
		trapezoid_t *parameter, duty_t *duty, int rotation_flag) {
	int duty_left, duty_right;

	if (rotation_flag == 0) {
		left->velocity = (left->velocity + right->velocity) / 2;
		right->velocity = left->velocity;
	}
//
//	if (parameter->back_rightturn_flag == 1) {
//		left->velocity = left->velocity * -1;
//		right->velocity = right->velocity * -1;
//	}

	left_deviation->now = (ideal->velocity - left->velocity);
	right_deviation->now = (ideal->velocity - right->velocity);
	left_deviation->cumulative += left_deviation->now;
	right_deviation->cumulative += right_deviation->now;

	duty_left = (int) left_deviation->now * gain->Kp
			+ left_deviation->cumulative * gain->Ki;
	duty_right = (int) right_deviation->now * gain->Kp
			+ right_deviation->cumulative * gain->Ki;

	if (rotation_flag == 1) {
		duty_left = duty_left * -1; //*0.98
//		duty_right=duty_right*1.07;
	}
	if (parameter->back_rightturn_flag == 1) {
		duty_left = duty_left * -1;
		duty_right = duty_right * -1;
	}
	duty->left += duty_left;
	duty->right += duty_right;
}

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel) {

	trapezoid_preparation(&translation_parameter, i_distance, accel, max_vel,
			strat_vel, end_vel);
	wall_control_flag=1;
	translation_parameter.run_flag = 1;
	translation_ideal.velocity = translation_parameter.strat_vel;
//	log_start();

}

void set_rotation(float i_angle, float accel, float max_vel, float center_vel) {
	trapezoid_preparation(&rotation_parameter, i_angle, accel, max_vel, 0.0,
			0.0);

	wall_control_flag = 0;
	rotation_ideal.velocity = rotation_parameter.strat_vel;
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;
	if((i_angle>80.0)||(i_angle<100.0)){
		direction++;
		if(direction==4){
			direction=North;
		}
	}else if((i_angle<-80.0)||(i_angle>-100.0)){
		if(direction==0){
			direction=4;
		}
		direction--;
	}
	rotation_parameter.run_flag = 1;
	log_start();
}

void wait_straight(void) {
	volatile int i;
	//LEFTEING = 1;
	while (translation_parameter.run_flag == 1) {
		//	myprintf("%6.2f", rotation_ideal.velocity);
	}
	//LEFTFRONT = 1;
	//translation_parameter.run_flag=1;
	translation_ideal.accel = 0.0;
	translation_ideal.dis = 0.0;
	translation_ideal.velocity = translation_parameter.end_vel;
	translation_parameter.back_rightturn_flag = 0;
	run_left_deviation.now = 0.0;
	run_left_deviation.difference = 0.0;
	run_right_deviation.now = 0.0;
	run_right_deviation.difference = 0.0;

//	duty.left = 0;
//	duty.right = 0;
//	duty_to_moter();
}

void wait_rotation(void) {
	volatile int i;
	LEFTEING = 1;
	while (rotation_parameter.run_flag == 1) {

	}
	LEFTFRONT = 1;
	rotation_ideal.accel = 0.0;
	rotation_ideal.dis = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.back_rightturn_flag = 0;
//	duty.left = 0;
//	duty.right = 0;
//	duty_to_moter();
}

void trapezoid_preparation(trapezoid_t *trapezoid, float i_distance,
		float accel, float max_vel, float strat_vel, float end_vel) {
//任意のパラメータから台形加速の概要を計算
//trapezoidはポインタでとってる

	trapezoid->back_rightturn_flag = 0;

	if (i_distance < 0) {
		i_distance = i_distance * -1;
		trapezoid->back_rightturn_flag = 1;
	}

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
	if (ideal->dis < trapezoid->acceldistance) {
//		UI_LED1 = 1;
		if (ideal->velocity < trapezoid->max_vel) {
			ideal->accel = trapezoid->accel;
		} else {
			ideal->velocity = trapezoid->max_vel;
			ideal->accel = 0;
		}

	} else if (ideal->dis
			< (trapezoid->i_distance - trapezoid->deacceldistance)) {
//		UI_LED2 = 1;
		ideal->accel = 0;

	} else if (ideal->velocity > trapezoid->end_vel) {
//		UI_LED3 = 1;
		ideal->accel = -trapezoid->accel;

	} else {
		trapezoid->run_flag = 0;
//		RIGHTWING = 1;
	}

}

void integral(run_t *ideal) {
	ideal->velocity += ideal->accel * 0.001;
	ideal->dis += ideal->velocity * 0.001 + ideal->accel * 0.001 * 0.001 / 2;
}

void integral_vel_to_dis(float *velocity, float *distance) {
	*distance += *velocity * 0.001;
}

void coordinate(void) {
	if (direction == North) {
		y.now++;
	}else if(direction==West){
		x.now--;
	}else if(direction==South){
		y.now--;
	}else if(direction==East){
		x.now++;
	}
}

