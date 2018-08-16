/*
 * adachi.c
 *
 *  Created on: 2018/08/13
 *      Author: takao keisuke
 */

#include "adachi.h"
#include "iodefine.h"
#include "variable.h"
#include "CMT.h"
#include "run.h"
#include "other.h"

void right_hand(void){
	set_straight(142.0, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
	wait_straight();
	while(1){
		if(SEN_L.now<SEN_L.threshold){
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search, nomal_run.vel_search, 0.0);
			wait_straight();
			wait_time(100);
			set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search,0.0);
			wait_rotation();
			wait_time(100);
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
			wait_straight();
		}else if(SEN_F.now<SEN_F.threshold){
			set_straight(180.0, nomal_run.accel, nomal_run.vel_search, nomal_run.vel_search, nomal_run.vel_search);
						wait_straight();
		}else if(SEN_R.now<SEN_R.threshold){
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search, nomal_run.vel_search, 0.0);
			wait_straight();
			wait_time(100);
			set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search,0.0);
			wait_rotation();
			wait_time(100);
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
			wait_straight();
		}else{
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search,nomal_run.vel_search, 0.0);
			wait_straight();
			wait_time(100);
			set_rotation(-180.0, nomal_rotation.accel, nomal_rotation.vel_search,0.0);
			wait_rotation();
			wait_time(100);
			set_straight(90.0, nomal_run.accel, nomal_run.vel_search, 0.0, nomal_run.vel_search);
			wait_straight();
		}
	}

}
