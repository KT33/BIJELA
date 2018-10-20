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
#include "speaker.h"

void wall_control(void) {
	if (wall_control_flag == 1 && wall_control_oblique_flag == 0) {
		if (((translation_ideal.velocity) > 350) && (SEN_L.diff < 15)
				&& (SEN_R.diff < 15) && (SEN_F.now < SEN_F.reference)) { //&& (SEN_L.diff < 2000) && (SEN_R.diff < 2000)&& (SEN_F.now < SEN_F.threshold * 100))
			if (SEN_L.now > SEN_L.threshold && SEN_R.now > SEN_R.threshold) {
				wallcontrol_value =
						wall_cntrol_gain.Kp
								* (((float) SEN_L.now - (float) SEN_L.reference)
										- ((float) SEN_R.now
												- (float) SEN_R.reference));
//				LEFTFRONT = 1;
//				RIGHTFRONT = 1;
//				CENTERFRONT = 1;
			} else if (SEN_L.now < SEN_L.threshold
					&& SEN_R.now > SEN_R.threshold) {
				wallcontrol_value = -2.0 * wall_cntrol_gain.Kp
						* ((float) SEN_R.now - (float) SEN_R.reference);
//				LEFTFRONT = 0;
//				RIGHTFRONT = 1;
//				CENTERFRONT = 1;
			} else if (SEN_L.now > SEN_L.threshold
					&& SEN_R.now < SEN_R.threshold) {
				wallcontrol_value = 2.0 * wall_cntrol_gain.Kp
						* ((float) SEN_L.now - (float) SEN_L.reference);
//				LEFTFRONT = 1;
//				RIGHTFRONT = 0;
//				CENTERFRONT = 1;
			} else {
				wallcontrol_value = 0.0;
//				LEFTFRONT = 0;
//				RIGHTFRONT = 0;
//				CENTERFRONT = 1;
			}
		} else {
			wallcontrol_value = 0.0;
//			LEFTFRONT = 0;
//			RIGHTFRONT = 0;
//			CENTERFRONT = 0;
		}
	} else if (wall_control_flag == 1 && wall_control_oblique_flag == 1) {
		wallcontrol_value = 0.0; //ここに斜め壁制御を書く
		if (SEN_L.now > SEN_L.oblique_threshold && SEN_L.diff_1ms < 180) {
			wallcontrol_value = oblique_Side_gain
					* (SEN_L.now - SEN_L.oblique_reference);

		} else if (SEN_LF.now > SEN_LF.oblique_threshold && SEN_LF.diff < 20) {
			wallcontrol_value = oblique_Front_gain
					* (SEN_LF.now - SEN_LF.oblique_reference);
		}
		if (SEN_R.now > SEN_R.oblique_threshold && SEN_R.diff_1ms < 180) {
			wallcontrol_value -= oblique_Side_gain
					* (SEN_R.now - SEN_R.oblique_reference);

		} else if (SEN_RF.now > SEN_RF.oblique_threshold && SEN_RF.diff < 30) {
			wallcontrol_value -= oblique_Front_gain
					* (SEN_RF.now - SEN_RF.oblique_reference);
		}

	} else {
		wallcontrol_value = 0.0;
	}

//	rotation_ideal.velocity+=wallcontrol_value;

//	duty->left += duty_value;
//	duty->right += -1 * duty_value;

}

void PID_control(run_t *ideal, run_t *left, run_t *right,
		deviation_t *left_deviation, deviation_t *right_deviation, gain_t *gain,
		trapezoid_t *parameter, duty_t *duty, uint8_t rotation_flag) {
	int duty_left, duty_right;
	float Kp, Ki;

	Ki = gain->Ki;
	Kp = gain->Kp;

	left->velocity = (left->velocity + right->velocity) / 2;
	right->velocity = left->velocity;

	if (rotation_flag == 1) {
		right->velocity += wallcontrol_value;
		left->velocity += wallcontrol_value;
	}

	left_deviation->now = (ideal->velocity - left->velocity);
	right_deviation->now = (ideal->velocity - right->velocity);
	left_deviation->cumulative += left_deviation->now;
	right_deviation->cumulative += right_deviation->now;

	duty_left = (int) left_deviation->now * Kp
			+ left_deviation->cumulative * Ki;
	duty_right = (int) right_deviation->now * Kp
			+ right_deviation->cumulative * Ki;

	if (rotation_flag == 1) {
		duty_left = duty_left * -1;
//		test_float=left_deviation->now;
	}
	if (parameter->back_rightturn_flag == 1) {
		duty_left = duty_left * -1;
		duty_right = duty_right * -1;
	}
	duty->left += duty_left;
	duty->right += duty_right;
	test2=duty_left;

}

void set_straight(float i_distance, float accel, float max_vel, float strat_vel,
		float end_vel) {

	trapezoid_preparation(&translation_parameter, i_distance, accel, max_vel,
			strat_vel, end_vel);
	wall_control_flag = 1;
	if (i_distance < 0) {
		wall_control_flag = 0;
	}
	translation_parameter.run_flag = 1;
	translation_ideal.velocity = translation_parameter.strat_vel;
//	log_start();

}

void set_rotation(float i_angle, float accel, float max_vel, float center_vel) {
	trapezoid_preparation(&rotation_parameter, i_angle, accel, max_vel, 0.0,
			0.0);

	wall_control_flag = 0;
	rotation_ideal.velocity = 0.0;
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = center_vel;
	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
//	rotation_gain.Ki = 0.04;
	if ((i_angle > 80.0) && (i_angle < 100.0)) {
		direction++;
	} else if ((i_angle < -80.0) && (i_angle > -100.0)) {
		direction += 3;
	} else if (((i_angle > 170.0) && (i_angle < 190.0))
			|| ((i_angle < -170.0) && (i_angle > -190.0))) {
		direction += 2;
	}
	if (direction > 3) {
		direction -= 4;
	}
	rotation_parameter.run_flag = 1;
//	log_start();
}

void wait_straight(void) {
	volatile int i;
//LEFTEING = 1;
	while (translation_parameter.run_flag == 1 && failsafe_flag == 0) {
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
	rotation_parameter.back_rightturn_flag = 0;
//	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;

//	duty.left = 0;
//	duty.right = 0;
//	duty_to_moter();
}

void wait_rotation(void) {
	volatile int i;
//	LEFTEING = 1;
	while (rotation_parameter.run_flag == 1 && failsafe_flag == 0) {

	}
//	LEFTFRONT = 1;
//	rotation_gain.Ki = 0.0;
	translation_ideal.dis = 0.0;
	rotation_ideal.accel = 0.0;
	rotation_ideal.dis = 0.0;
	rotation_ideal.velocity = 0.0;
	rotation_parameter.back_rightturn_flag = 0;
	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
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
		trapezoid->max_vel = max_vel;
	} else {
		trapezoid->acceldistance = 0.0;
		trapezoid->deacceldistance = 0.0;
	}

}

void control_accel(run_t *ideal, trapezoid_t *trapezoid, uint8_t rotation_flag) {
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
////////////////////////////////////////////////
///
///
/////
///
///
///
///
		ideal->velocity = trapezoid->max_vel;
/////////////////////////////////////////
///
///
///
///
///
//////////
	} else if (ideal->velocity > trapezoid->end_vel) {
//		UI_LED3 = 1;
//		if (rotation_flag == 0) {
//			rotation_gain.Ki = 0.0;
//		}
		ideal->accel = -trapezoid->accel;

	} else {
		trapezoid->run_flag = 0;
////////////////////////////
///
///
///
///
///
///
///
/// //////////
		ideal->accel = 0;
		ideal->velocity = trapezoid->end_vel;
////////////////////////////
///
///
///
///
/////////////
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
	} else if (direction == West) {
		x.now--;
	} else if (direction == South) {
		y.now--;
	} else if (direction == East) {
		x.now++;
	}
}

void coordinate_pass(void) {
	if (direction_pass == North) {
		y.pass++;
	} else if (direction_pass == West) {
		x.pass--;
	} else if (direction_pass == South) {
		y.pass--;
	} else if (direction_pass == East) {
		x.pass++;
	}
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

	if (duty_left > 400) {
		duty_left = 400 - 1;
	}
	if (duty_right > 400) {
		duty_right = 400 - 1;
	}
	if (duty_left < -400) {
		duty_left = -400 + 1;
	}
	if (duty_right < -400) {
		duty_right = -400 + 1;
	}

	test1 = duty_left;
//	test2 = duty_right;

	MTU0.TGRB = (duty_right); //MOTER_R
	MTU0.TGRD = (duty_left); //MOTER_L

	duty.left = 0;
	duty.right = 0;
}
