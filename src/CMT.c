/*
 * CMT.c
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#include "iodefine.h"
#include "variable.h"
#include "SPI.h"
#include "run.h"
#include "other.h"
#include "speaker.h"
#include "CMT.h"

void interrupt_cmt0(void) {
	g_count++;
	failsafe_accel = test_gyro();
	real_velocity_control();

	if (SEN_check_flag == 1) {
		AD_SEN();
	}

	if ((right_real.velocity > 4500.0 || left_real.velocity > 4500.0
			|| rotation_deviation.cumulative > 20000.0 //20000.0
			|| rotation_deviation.cumulative < -20000.0
			|| failsafe_accel > 50000.0)
			&& translation_parameter.back_rightturn_flag == 0) { //|| failsafe_accel > 39.2
		failsafe();
	}

	if (failsafe_flag == 1 && failsafe_counter < 1000) {
	//	ui_led_3bit(7);
		translation_ideal.accel=0.0;
		translation_ideal.velocity=0.0;
		run_left_deviation.cumulative=0.0;
		run_right_deviation.cumulative=0.0;
		failsafe_counter++;
		Moter_Stby = 1;
		PID_control(&translation_ideal, &left_real, &right_real,
				&run_left_deviation, &run_right_deviation, &run_gain,
				&translation_parameter, &duty, 0);
		duty_to_moter();
	}

	//	real_velocity_control();
	if (mode_flag & 0x80) { //モード内
		if (moter_flag == 1 && failsafe_flag == 0) {
			failsafe_accel = test_gyro();
//			ui_led_3bit(x.now);
			Moter_Stby = 1;
			AD_SEN();
			wall_control();
			real_angle_control();

			if (angle_calibration_flag == 1) {
				angle_calibration_counter++;
				angle_calibration_integral += rotation_real.velocity;
				if(angle_calibration_counter==1000){
					angle_calibration_flag=0;
				}
			}

			if (translation_parameter.run_flag == 1) {
				control_accel(&translation_ideal, &translation_parameter, 0);
			}
			if (rotation_parameter.run_flag == 1) {
//			CENTERFRONT = 1;
				control_accel(&rotation_ideal, &rotation_parameter, 1);
				integral(&rotation_ideal);
			}

			PID_control(&translation_ideal, &left_real, &right_real,
					&run_left_deviation, &run_right_deviation, &run_gain,
					&translation_parameter, &duty, 0);
			if (translation_parameter.back_rightturn_flag == 0
					|| translation_ideal.velocity > 100.0) {
				PID_control(&rotation_ideal, &rotation_real, &rotation_real,
						&rotation_deviation, &rotation_deviation,
						&rotation_gain, &rotation_parameter, &duty, 1);
			}
			integral(&translation_ideal);
			if (log_flag == 1) {
				log_sampling();
			}
			integral_vel_to_dis(&right_real.velocity, &right_real.dis);
			integral_vel_to_dis(&rotation_real.velocity, &rotation_real.dis);
			duty_to_moter();

			led_count++;
			if (led_count < 500) {
				LEFTEING = 1;
				RIGHTWING = 0;
				//			LEFTFRONT = 0;
				//			RIGHTFRONT = 1;
			} else if (led_count < 1000) {
				LEFTEING = 0;
				RIGHTWING = 1;
				//			LEFTFRONT = 1;
				//			RIGHTFRONT = 0;
			} else {
				led_count = 0;
			}
		} else {
			if (failsafe_flag == 0) {
				Moter_Stby = 0;
				real_angle_control();

			}
		}
//		LEFTEING = 0;
//		RIGHTWING = 0;
//		LEFTFRONT = 0;
//		RIGHTFRONT = 0;
	} else { //モード選択中
//		real_velocity_control();
		integral_vel_to_dis(&right_real.velocity, &mode_select_dis);
	}
	if(para_mode_flag==1){
		integral_vel_to_dis(&right_real.velocity, &mode_select_dis);
	}
}

void failsafe(void) {
	failsafe_flag = 1;
	translation_ideal.accel = 0.0;
	translation_ideal.velocity = 0.0;
//		duty.left = 0;
//		duty.right = 0;
	duty_to_moter();
	Moter_Stby = 1;
//		MTU0.TGRB = 0; //MOTER_R
//		MTU0.TGRD = 0; //MOTER_L
	x.now = 0;
	y.now = 0;
	direction = 0;
	FAN=0;
//	UI_LED1 = 1;
//	UI_LED2 = 1;
//	UI_LED3 = 1;
//	if (right_real.velocity > 2500.0 || left_real.velocity > 2500.0) {
//		LEFTEING = 0;
//		RIGHTWING = 0;
//	} else if (rotation_deviation.cumulative > 20000.0) {
//		LEFTEING = 1;
//		RIGHTWING = 0;
//	} else if (failsafe_accel > 39.2) {
//		LEFTEING = 0;
//		RIGHTWING = 1;
//	}
//
//	RIGHTFRONT = 1;
//	LEFTFRONT = 1;
//	CENTERFRONT = 1;
	failsafe_flag = 1;
	failsafe_counter = 0;
	//moter_flag=0;
}

void init_cmt0(void) {

	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA15 = 0; //CMT0の低消費電力モード解除
	SYSTEM.PRCR.WORD = 0xA500; //低消費電力モードレジスタのプロテクト

	CMT0.CMCR.BIT.CKS = 0; //PCLK(50MHz)/8に設定,6.25MHz
	CMT0.CMCOR = 6249; //6.25k回/1ms→1msで6250-1

	CMT0.CMCR.BIT.CMIE = 1; //割り り込み許可

	IEN(CMT0,CMI0)=1; //割り込み許可
	IPR(CMT0,CMI0)=15; //割り込み優先度Max

	CMT.CMSTR0.BIT.STR0 = 1; //カウントスタート

}

//		if (test_flag == 1) {	//enkaigei
//			rotation_deviation.cumulative = 0;
//			rotation_real.dis = 0.0;
//			//		rotation_real.velocity = 0.0;
//			rotation_ideal.accel = 0.0;
//			rotation_ideal.velocity = 0.0;
//			rotation_parameter.run_flag = 1;
//			RIGHTWING = 1;
//			rotation_ideal.accel = 0.0;
//			rotation_ideal.velocity = 0.0;
//			translation_ideal.velocity = 0.0;
//			translation_ideal.accel = 0;
//			rotation_real.dis += rotation_real.velocity * 0.001;
//			PID_control(&rotation_ideal, &rotation_real, &rotation_real,
//					&rotation_deviation, &rotation_deviation, &rotation_gain,
//					&rotation_parameter, &duty, 1);
//			PID_control(&translation_ideal, &left_real, &right_real,
//					&run_left_deviation, &run_right_deviation, &run_gain,
//					&translation_parameter, &duty, 0);
//
//		}
