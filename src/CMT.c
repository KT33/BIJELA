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

void interrupt_cmt0(void) {
	g_count++;
	AD_all();
	if (gyro_flag == 1) {
		angle += test_gyro2() / 1000;
	} else {
		angle = 0;
	}

	if (mode_flag & 0x80) { //モード内
		real_velocity_control();
		real_angle_control();
//		if (translation_parameter.run_flag == 1) {
//			control_accel(&translation_ideal, &translation_parameter);
//			PID_control(&translation_ideal, &left_real, &right_real,
//					&run_left_deviation, &run_right_deviation, &run_gain,
//					&duty);
//			integral(&translation_ideal);
//		}
////		if (rotation_parameter.run_flag == 1) {
////			CENTERFRONT = 1;
////			control_accel(&rotation_ideal, &rotation_parameter);
////			PID_control(&rotation_ideal, &rotation_real, &rotation_real,
////					&rotation_deviation, &rotation_deviation, &rotation_gain,
////					&duty);
////			integral(&rotation_ideal);
////		}
//
//		if (log_flag == 1) {
//			log_sampling();
//		}

		if (test_flag == 1) {
			RIGHTWING = 1;
			rotation_ideal.accel = 0.0;
			rotation_ideal.velocity=0.0;
			rotation_real.dis += rotation_real.velocity * 0.001;
			PID_control(&rotation_ideal, &rotation_real, &rotation_real,
					&rotation_deviation, &rotation_deviation, &rotation_gain,
					&duty);
		}
		duty_to_moter();
	} else { //モード選択中
		real_velocity_control();
		integral_vel_to_dis(&right_real.velocity, &mode_select_dis);
	}
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
