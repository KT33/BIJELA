/*
 * motion.c
 *
 *  Created on: 2018/10/01
 *      Author: takao keisuke
 */

#include "adachi.h"
#include "iodefine.h"
#include "variable.h"
#include "CMT.h"
#include "run.h"
#include "other.h"
#include "stdint.h"
#include "walldate.h"
#include "motion.h"

void go_entrance(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//rotation_deviation.cumulative = 0.0;
	moter_flag = 1;
	set_straight(140.0, accel, vel, 0.0, vel);
	wait_straight();
}

void go_center(float accel, float vel) {
//	rotation_deviation.now = 0.0;
//	rotation_deviation.cumulative = 0.0;
	set_straight(54.0, accel, vel, 0.0, 0.0);
	wait_straight();
}

void turn_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void turn_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void pass_180(float accel, float vel) {
	set_straight(180.0, accel, vel, vel, vel);
	wait_straight();
}

void turn_180(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(90.0, accel, vel, 0.0, vel);
	wait_straight();
}

void ketuate(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void ketuate_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void ketuate_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	set_straight(57.0, accel, vel, 0.0, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
	go_entrance(accel, vel);
}

void back_100(void) {
	set_straight(-80.0, 300, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
}

void ketuate_goal_left(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(-90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
}

void ketuate_goal_right(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	go_center(accel, vel);
	wait_time(50);
	set_rotation(90.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	back_100();
	wait_time(50);
	rotation_deviation.cumulative = 0.0;
}

void non_ketuate_goal_turn(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_rotation(180.0, nomal_rotation.accel, nomal_rotation.vel_search, 0.0);
	wait_rotation();
	wait_time(50);
	set_straight(-50.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	wait_time(50);
}

void non_ketuate_goal(float accel, float vel) {
	set_straight(90.0, accel, vel, vel, 0.0);
	wait_straight();
	wait_time(50);
	set_straight(-50.0, 500, 150, 0.0, 0.0);
	wall_control_flag = 0;
	wait_straight();
	wait_time(50);
}

void slalom_left90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset;
	if (run_vel == 500.0) {
		rota_accel = slarom_500.accel;
		rota_vel = slarom_500.max_vel;
		in_offset = slarom_500.left.in_offset;
		out_offset = slarom_500.left.out_offset;
	} else if (run_vel == 600.0) {
		rota_accel = slarom_600.accel;
		rota_vel = slarom_600.max_vel;
		in_offset = slarom_600.left.in_offset + 1.0;
		out_offset = slarom_600.left.out_offset;
	}
	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(90.0, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
}

void slalom_right90(float run_accel, float run_vel) {
	float rota_accel, rota_vel, in_offset, out_offset;
	float angle_offset = 0.0;
	if (run_vel == 500.0) {
		rota_accel = slarom_500.accel;
		rota_vel = slarom_500.max_vel;
		in_offset = slarom_500.right.in_offset;
		out_offset = slarom_500.right.out_offset;
		angle_offset = 1.0;
	} else if (run_vel == 600.0) {
		rota_accel = slarom_600.accel;
		rota_vel = slarom_600.max_vel;
		in_offset = slarom_600.right.in_offset - 2.0;
		out_offset = slarom_600.right.out_offset + 3.0;
		angle_offset = 2.0;
	}
	set_straight(in_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, run_vel);
	wait_rotation();
	set_straight(out_offset, run_accel, run_vel, run_vel, run_vel);
	wait_straight();
}

void stop90(float run_accel, float run_vel) {
	set_straight(90.0, run_accel, run_vel, run_vel, 0.0);
	wait_straight();
}

void slalom_left_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_left90(accel, vel);
	stop90(accel, vel);
}

void slalom_right_check(float accel, float vel) {
	moter_flag = 1;
	nomal_run.vel_search = 600.0;
	go_entrance(accel, vel);
	slalom_right90(accel, vel);
	stop90(accel, vel);
}

void turn_left_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 20.0;
		out_offset = 38.0;
		rota_accel = 6000.0;
		rota_vel = 400.0;
		kabekire_dis = 7.2;
		angle_offset = 0;
	}

	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 39.0;
		rota_accel = 8700.0;
		rota_vel = 642.0;
		kabekire_dis = 1.0;
		angle_offset = -1.2;
	}

	if (vel == 1200.0) {
		in_offset = 0.0;
		out_offset = 42.0;
		rota_accel = 8700.0;
		rota_vel = 795.0;
		kabekire_dis = 0.0;
		angle_offset = 0.0;
	}
	if(vel==100.0){
		in_offset = 0.0;
		out_offset = 42.0;
		rota_accel = 8500.0;
		rota_vel = 1000.0;
		kabekire_dis = 0.0;
		angle_offset = 0.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0]=translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();

}

void turn_right_180_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 30.0;
		out_offset = 43.0;
		rota_accel = 4700.0;
		rota_vel = 385.0;
		angle_offset = 1.0;
		kabekire_dis = 13.2;
	}
	if (vel == 1000.0) {
		in_offset = 0.0;
		out_offset = 35.0;
		rota_accel = 8700.0;
		rota_vel = 660.0;
		kabekire_dis = 4.0;
		angle_offset = 1.5;
	}

	if (vel == 1200.0) {
		in_offset = 0.0;
		out_offset = 38.0;
		rota_accel = 10000.0; //8700
		rota_vel = 782.0;
		kabekire_dis = 0.0;
		angle_offset = 2.0;
	}
	if (vel == 1300.0) {
		in_offset = 0.0;
		out_offset = 38.0;
		rota_accel = 9000.0; //8700
		rota_vel = 1100.0;
		kabekire_dis = 0.0;
		angle_offset = 2.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-180.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	direction += 1;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
	direction += 3;
	if (direction > 3) {
		direction -= 4;
	}
	coordinate();
}

void turn_left_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 52.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		kabekire_dis = 7.8;
		angle_offset = 0.0;
	}
	if (vel == 1000.0) {
		in_offset = 19.0;
		out_offset = 53.0;
		rota_accel = 6200.0;
		rota_vel = 1500.0;
		kabekire_dis = 3.0;
		angle_offset = 0.3;
	}
	if (vel == 1200.0) {
		in_offset = 18.0;
		out_offset = 64.0;
		rota_accel = 9500.0;
		rota_vel = 1660.0;
		kabekire_dis = 3.0;
		angle_offset = -0.4;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(90.0 - angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
		kabekire_dis = 12.0;
	}

	if (vel == 1000.0) {
		in_offset = 26.0;
		out_offset = 55.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		kabekire_dis = 5.0;
		angle_offset = 1.5;
	}
	if (vel == 1200.0) {
		in_offset = 17.0;
		out_offset = 48.0;
		rota_accel = 8600.0;
		rota_vel = 1700.0;
		kabekire_dis = 5.0;
		angle_offset = 2.2;
	}
	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}
//	Log[0] = translation_ideal.dis;

	coordinate();

	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void farst_turn_right_90_big(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel;
	float angle_offset;
	if (vel == 600.0) {
		in_offset = 36.0;
		out_offset = 50.0;
		rota_accel = 3000.0;
		rota_vel = 350.0;
		angle_offset = 1.0;
	}

	if (vel == 1000.0) {
		in_offset = 25.0;
		out_offset = 53.0;
		rota_accel = 6700.0;
		rota_vel = 1700.0;
		angle_offset = 1.0;
	}

	if (vel == 1200.0) {
		in_offset = 16.0;
		out_offset = 48.0;
		rota_accel = 8600.0;
		rota_vel = 1700.0;
		angle_offset = 2.2;
	}
	coordinate();

	set_straight(in_offset + 50.0, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();

	coordinate();
}

void turn_right_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 15.0;
		out_offset = 62.0 - 20.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
		kabekire_dis = 4.0;
	}

	if (vel == 1200.0) {
		in_offset = 4.5;
		out_offset = 80.0 - 20.0;
		rota_accel = 16800.0;
		rota_vel = 1200.0;
		angle_offset = 4.0;
		kabekire_dis = 2.4;
	}

	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}

	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
}

void turn_left_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 8.0;
		out_offset = 78.0 - 20.0;
		rota_accel = 12000.0;
		rota_vel = 1100.0;
		angle_offset = 0.0;
		kabekire_dis = 3.0;
	}

	if (vel == 1200.0) {
		in_offset = 9.0;
		out_offset = 106.0 - 20.0;
		rota_accel = 28000.0;
		rota_vel = 1150.0;
		angle_offset = 0.86;
		kabekire_dis = 0.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
}

void turn_right_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 3.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 650.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	if (vel == 1200.0) {
		in_offset = 10.0;
		out_offset = 40.0 - 20.0;
		rota_accel = 12700.0;
		rota_vel = 900.0;
		angle_offset = 3.6;
		kabekire_dis = 0.0;
	}

	while (SEN_R.now > SEN_R.threshold) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
}

void turn_left_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 1.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 658.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	if (vel == 1200.0) {
		in_offset = 10.4;
		out_offset = 51.0 - 20.0;
		rota_accel = 13000.0;
		rota_vel = 900.0;
		angle_offset = 0.0;
		kabekire_dis = 0.0;
	}

	while (SEN_L.now > SEN_L.threshold) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
	set_rotation(135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
}

void turn_right_45_out(float vel) {
	float in_offset = 0.0, out_offset = 0.0, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 44.0 + 20.0;
		out_offset = 30.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 2.0;
		kabekire_dis = 12.8;
	}

	if (vel == 1200.0) {
		in_offset = 34.0 + 20.0;
		out_offset = 22.0;
		rota_accel = 11000.0;
		rota_vel = 1100.0;
		angle_offset = 2.8;
		kabekire_dis = 12.2;
	}

	while (SEN_R.now > 2000) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_left_45_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 41.0 + 20.0;
		out_offset = 42.0;
		rota_accel = 12000.0;
		rota_vel = 1100.0;
		angle_offset = 0.0;
		kabekire_dis = 7.0;
	}

	if (vel == 1200.0) {
		in_offset = 28.0 + 20.0;
		out_offset = 35.0;
		rota_accel = 12500.0;
		rota_vel = 1170.0;
		angle_offset = 0.0;
		kabekire_dis = 7.2;
	}

	while (SEN_L.now > 2000) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_right_135_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 0.0 + 20;
		out_offset = 50.0;
		rota_accel = 8000.0;
		rota_vel = 830.0;
		angle_offset = 2.0;
		kabekire_dis = 16.0;
	}

	if (vel == 1200.0) {
		in_offset = 0.0 + 20;
		out_offset = 58.0;
		rota_accel = 12700.0;
		rota_vel = 940.0;
		angle_offset = 3.0;
		kabekire_dis = 13.0;
	}

	while (SEN_R.now > 2000) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	log_start();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_left_135_out(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 0.0 + 20.0;
		out_offset = 50.0;
		rota_accel = 9000.0;
		rota_vel = 710.0;
		angle_offset = 0.0;
		kabekire_dis = 10.8;
	}

	if (vel == 1200.0) {
		in_offset = 5.0 + 20.0;
		out_offset = 70.0;
		rota_accel = 12600.0;
		rota_vel = 980.0;
		angle_offset = 0.8;
		kabekire_dis = 7.2;
	}

	while (SEN_L.now > 2000.0) {
		translation_ideal.dis = kabekire_dis;
	}

	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void turn_left_v90(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;

	if (vel == 1000.0) {
		in_offset = 0.0 + 20.0;
		out_offset = 39.0 - 20.0;
		rota_accel = 11000.0;
		rota_vel = 900.0;
		angle_offset = -1.0;
		kabekire_dis = 10.8;
	}

	if (vel == 1200.0) {
		in_offset = -8.4 + 20.0;
		out_offset = 55.0 - 20.0;
		rota_accel = 16000.0;
		rota_vel = 1350.0;
		angle_offset = 0.8;
		kabekire_dis = 6.0;
	}

	while (SEN_L.now > 2000) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 1;
	wait_straight();
	wall_control_oblique_flag = 0;
}

void turn_right_v90(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel, kabekire_dis;
	float angle_offset;

	if (vel == 1000.0) {
		in_offset = 4.0 + 20.0;
		out_offset = 28.0 - 20.0;
		rota_accel = 12000.0;
		rota_vel = 705.0;
		angle_offset = 2.0;
		kabekire_dis = 14.3;
	}

	if (vel == 1200.0) {
		in_offset = -4.8 + 20.0;
		out_offset = 34.0 - 20.0;
		rota_accel = 14000.0;
		rota_vel = 1300.0;
		angle_offset = 2.2;
		kabekire_dis = 13.2;
	}

	while (SEN_R.now > 2000.0) {
		translation_ideal.dis = kabekire_dis;
	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset, nomal_run.accel, vel, vel, vel);
	wall_control_flag = 0;
	wait_straight();
	set_rotation(-90.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wall_control_oblique_flag = 1;
	wait_straight();
	wall_control_oblique_flag = 0;
}

void farst_turn_right_45_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel; //, kabekire_dis
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 15.0;
		out_offset = 62.0 - 20.0;
		rota_accel = 10000.0;
		rota_vel = 1000.0;
		angle_offset = 0.0;
//		kabekire_dis = 4.0;
	}

	if (vel == 1200.0) {
		in_offset = 4.5;
		out_offset = 80.0 - 20.0;
		rota_accel = 16800.0;
		rota_vel = 1200.0;
		angle_offset = 4.0;
//		kabekire_dis = 2.4;
	}

//	while (SEN_R.now > SEN_R.threshold) {
//		translation_ideal.dis = kabekire_dis;
//	}

//	Log[0] = translation_ideal.dis;
//	log_sampling();
	set_straight(in_offset + 50, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-45.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

void farst_turn_right_135_in(float vel) {
	float in_offset, out_offset, rota_accel, rota_vel; //, kabekire_dis
	float angle_offset;
//	if (vel == 600.0) {
//		in_offset = 36.0;
//		out_offset = 50.0;
//		rota_accel = 3000.0;
//		rota_vel = 350.0;
//		angle_offset = 1.0;
//	}

	if (vel == 1000.0) {
		in_offset = 3.0;
		out_offset = 20.0 - 20.0;
		rota_accel = 9000.0;
		rota_vel = 650.0;
		angle_offset = 0.0;
//		kabekire_dis = 5.0;
	}

	if (vel == 1200.0) {
		in_offset = 10.0;
		out_offset = 40.0 - 20.0;
		rota_accel = 12700.0;
		rota_vel = 900.0;
		angle_offset = 3.6;
//		kabekire_dis = 0.0;
	}

//	while (SEN_R.now > SEN_R.threshold) {
////		translation_ideal.dis = kabekire_dis;
//	}

//	Log[0] = translation_ideal.dis;
	log_sampling();
	set_straight(in_offset + 50, nomal_run.accel, vel, 0.0, vel);
	wait_straight();
	set_rotation(-135.0 + angle_offset, rota_accel, rota_vel, vel);
	wait_rotation();
	set_straight(out_offset, nomal_run.accel, vel, vel, vel);
	wait_straight();
}

