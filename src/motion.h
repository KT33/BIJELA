/*
 * motion.h
 *
 *  Created on: 2018/10/01
 *      Author: takao keisuke
 */

#ifndef MOTION_H_
#define MOTION_H_

void slalom_left90(float run_accel, float run_vel);
void slalom_right90(float run_accel, float run_vel);
void ketuate(float accel, float vel);
void adachi_map_straight(uint8_t goal_x, uint8_t goal_y, uint8_t goal_scale,
		walldate_t walldate);
void stop90(float run_accel, float run_vel);
void slalom_left_check(float accel,float vel);
void slalom_right_check(float accel,float vel);
void turn_left_180_big(float vel);
void turn_right_180_big(float vel);
void turn_left_90_big(float vel);
void turn_right_90_big(float vel);
void farst_turn_right_90_big(float vel);
void turn_right_45_in(float vel);
void turn_left_45_in(float vel);
void turn_right_135_in(float vel);
void turn_left_135_in(float vel);
void turn_right_45_out(float vel);
void turn_left_45_out(float vel);
void turn_right_135_out(float vel);
void turn_left_135_out(float vel);
void turn_right_v90(float vel);
void turn_left_v90(float vel);
void farst_turn_right_135_in(float vel);
void farst_turn_right_45_in(float vel);
void turn_left(float accel, float vel);
void turn_right(float accel, float vel);
void pass_180(float accel, float vel);
void turn_180(float accel, float vel);
void back_100(void);
void ketuate_goal_left(float accel, float vel);
void ketuate_goal_right(float accel, float vel);
void non_ketuate_goal(float accel, float vel);
void ketuate_right(float accel, float vel);
void ketuate_left(float accel, float vel);
void go_entrance(float accel, float vel);
void go_center(float, float);

#endif /* MOTION_H_ */
