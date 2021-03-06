/*
 * other.c
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#include "iodefine.h"
#include "machine.h"
#include "stdarg.h"
#include "variable.h"
#include "Clock.h"
#include "mode.h"
#include "other.h"
#include "SPI.h"
#include "walldate.h"
#include "speaker.h"

void real_angle_control(void) {
	rotation_real.velocity = -angle_calibration + test_gyro2(); //-0.2450 +
	if (rotation_parameter.back_rightturn_flag == 1) {
		rotation_real.velocity = -1 * rotation_real.velocity;
	}
}

void real_velocity_control(void) {
	encoder_L = TPU1.TCNT; //L
	encoder_R = TPU2.TCNT; //R

	left_real.velocity = 1 * (float) encoder_L * diameter * 3.14159265359 / 4096
			/ 4 / 40 * 11 / 0.001;
	right_real.velocity = -1 * (float) encoder_R * diameter * 3.14159265359
			/ 4096 / 4 / 40 * 11 / 0.001;

	TPU1.TCNT = 0;
	TPU2.TCNT = 0;
	if (translation_parameter.back_rightturn_flag == 1) {
		left_real.velocity = -1 * left_real.velocity;
		right_real.velocity = -1 * right_real.velocity;
	}
}

void ui_reset(void) {
	ui_led_3bit(0);
	LEFTEING = 0;
	LEFTFRONT = 0;
	CENTERFRONT = 0;
	RIGHTFRONT = 0;
	RIGHTWING = 0;
}

void ui_led_3bit(uint8_t value) {
	if (value == 1) {
		UI_LED1 = 0;
		UI_LED2 = 0;
		UI_LED3 = 1;
	}
	if (value == 2) {
		UI_LED1 = 0;
		UI_LED2 = 1;
		UI_LED3 = 0;
	}
	if (value == 3) {
		UI_LED1 = 0;
		UI_LED2 = 1;
		UI_LED3 = 1;
	}
	if (value == 4) {
		UI_LED1 = 1;
		UI_LED2 = 0;
		UI_LED3 = 0;
	}
	if (value == 5) {
		UI_LED1 = 1;
		UI_LED2 = 0;
		UI_LED3 = 1;
	}
	if (value == 6) {
		UI_LED1 = 1;
		UI_LED2 = 1;
		UI_LED3 = 0;
	}
	if (value == 7) {
		UI_LED1 = 1;
		UI_LED2 = 1;
		UI_LED3 = 1;
	}
	if (value == 0) {
		UI_LED1 = 0;
		UI_LED2 = 0;
		UI_LED3 = 0;
	}
}

void mode_select_by_encoder(void) {
	real_velocity_control();

}

void init_ALL(void) {
	FAN = 0;
	Clock_Settting();
	LED_Setting();
	init_sci1();
	init_cmt0();
	init_AD();
	init_SPI();
	init_speaker();
	init_gyro();
	init_Encoder();
	init_Moter_PWM();
	wait_time(5);
	Battery_Check();
	FAN = 0;
	clear_Map(&walldate_real);
	clear_Map(&walldate_checked);
	clear_adachiMap(&walldate_adachi);
	walldate_checked.row[1] = 0x0001;
//	clear_Map(&walldate_adachi);
	SEN_F.threshold = (SEN_LF.threshold + SEN_RF.threshold) / 2;
	SEN_F.reference = (SEN_LF.reference + SEN_RF.reference) / 2;
	x.now = 0;
	y.now = 0;
	init_dataflash();
}

void wait_time(int ms) {
	g_count = 0;
	while (g_count < ms) {
	}
}

void log_start(void) {
	log_counter = 0;
	log_index = 0;
	log_how_often = 1;
	log_flag = 1;
}

void log_sampling(void) {
	log_counter++;
	if (log_counter == log_how_often) {
		Log[log_index] = test_float;
		log2[log_index] = oblique_Front_gain;
		log3[log_index] = oblique_Side_gain;
//		log4[log_index] = (float) SEN_R.now;
		log_index++;
		log_counter = 0;
		if (log_index == LogMax - 1) {
			log_flag = 0;
			log_index = 0;
		}
	}
}

void log_output(void) {
	int i;
	for (i = 0; i < LogMax; i++) {
//		myprintf("%d	%.3f\n",i, Log[i]);
//		myprintf("%.3f	%.3f\n", Log[i], log2[i]);
		myprintf("%d	%.9f	%.9f  %.9f\n",i, Log[i], log2[i], log3[i]);
//		myprintf("%.3f	%.3f  %.3f	%.3f\n", Log[i], log2[i], log3[i],log4[i]);
	}
//	myprintf("\n");
//	myprintf("\n");
//	for (i = 0; i < LogMax; i++) {
//		myprintf("%.3f\n", log2[i]);
//	}
//	myprintf("\n");
//	myprintf("\n");
//	for (i = 0; i < LogMax; i++) {
//		myprintf("%.3f\n", log3[i]);
//	}
}

void Battery_Check(void) {
	S12AD.ADANS0.WORD = 0x0001; //AD変換(Batt)を設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}

	Batt = S12AD.ADDR0;
	Battery = (float) Batt * 0.00248648; //(9.97+20.8)/9.97*3.3/4096
	if (Battery < 7.74) { //7.7
		Moter_Stby = 0;
		while (1) {
			UI_LED1 = 1;
			UI_LED2 = 0;
			UI_LED3 = 1;
			RIGHTWING = 0;
			LEFTEING = 1;
			RIGHTFRONT = 1;
			LEFTFRONT = 0;
			wait_time(500);
			UI_LED1 = 0;
			UI_LED2 = 1;
			UI_LED3 = 0;
			RIGHTWING = 1;
			LEFTEING = 0;
			RIGHTFRONT = 0;
			LEFTFRONT = 1;
			wait_time(500);
			//	myprintf("%6.2f\n",Battery);
		}
	}
}

void chattering(void) {
	int i = 0;
	while (i < CHATT) {
		i++;
	}
	while (SWITCH == 0) {
	}
	while (i < CHATT) {
		i++;
	}
}

void init_sci1(void) { //ビットレートは38400
//低消費電力モードレジスタのプロテクト解除
	SYSTEM.PRCR.WORD = 0xA502;

//SIC1の低消費電力モード解除
	SYSTEM.MSTPCRB.BIT.MSTPB30 = 0;

//低消費電力モードレジスタのプロテクト
	SYSTEM.PRCR.WORD = 0xA500;
	SCI1.SCR.BYTE = 0; //動作停止とかとか
	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)
	PORT2.PMR.BIT.B6 = 1; //P26周辺機能として使用
	PORT3.PMR.BIT.B0 = 1; //P30を周辺機のとして使用
	MPC.P30PFS.BIT.ISEL = 0; //P30をIRQn端子として使用しない
	MPC.P26PFS.BIT.PSEL = 0xa; //P26をTXD1に設定
	MPC.P30PFS.BIT.PSEL = 0xa; //P30をRXD1に設定

//SCI1.SCR.BYTE = 0; //動作停止とかとか

	SCI1.SCR.BIT.CKE = 0;
	SCI1.SIMR1.BIT.IICM = 0; //シリアルインターフェースモード
	SCI1.SPMR.BIT.CKPH = 0;
	SCI1.SPMR.BIT.CKPOL = 0;

	SCI1.SMR.BYTE = 0; //各種設定

	SCI1.SCMR.BIT.SMIF = 0; //シリアルコミュニケーションインターフェイスモード
	SCI1.SCMR.BIT.SINV = 0; //内容を反転しない
	SCI1.SCMR.BIT.SDIR = 0; //LSBファーストで受送信

	SCI1.SEMR.BIT.ABCS = 0; //16サイクルで転送

	SCI1.BRR = 40;

	SCI1.SCR.BIT.TEIE = 1;
	SCI1.SCR.BIT.TIE = 1;
	SCI1.SCR.BIT.RIE = 1;
	SCI1.SCR.BIT.TE = 1;
	SCI1.SCR.BIT.RE = 1;

}

void put1byte(char c) {
	while ( SCI1.SSR.BIT.TEND == 0)
		;
	SCI1.SSR.BIT.TEND = 0;
	SCI1.TDR = c;

}

void putnbyte(char *buf, int len) {
	int c;
	for (c = 0; c < len; c++) {
		put1byte(buf[c]);
	}
}

int myprintf(const char *fmt, ...) {
	static char buffer[255];
	volatile long len;

	va_list ap;
	va_start(ap, fmt);

	len = vsprintf(buffer, fmt, ap);

	putnbyte(buffer, len);
	va_end(ap);
	return len;
}

void LED_Setting(void) {
	PORT2.PMR.BIT.B7 = 0; //P27をIOモードに設定
	PORT2.PDR.BIT.B7 = 1; //P27のポート方向設定(In:0,Out:1)
	PORT5.PMR.BIT.B5 = 0;
	PORT5.PDR.BIT.B5 = 1;
	PORT5.PMR.BIT.B4 = 0;
	PORT5.PDR.BIT.B4 = 1;
	PORTC.PMR.BIT.B5 = 0;
	PORTC.PDR.BIT.B5 = 1;
	PORT0.PMR.BIT.B5 = 0;
	PORT0.PDR.BIT.B5 = 1;
	PORTA.PMR.BIT.B6 = 0;
	PORTA.PDR.BIT.B6 = 1;
	PORTC.PMR.BIT.B3 = 0;
	PORTC.PDR.BIT.B3 = 1; /////
	PORTA.PMR.BIT.B0 = 0; /////
	PORTA.PDR.BIT.B0 = 1;
	PORT3.PMR.BIT.B1 = 0; //SW
	PORT3.PDR.BIT.B1 = 0; //SW
	PORT4.PMR.BIT.B1 = 0; //以下SENLED
	PORT4.PDR.BIT.B1 = 1;
	PORT4.PMR.BIT.B3 = 0;
	PORT4.PDR.BIT.B3 = 1;
	PORTB.PMR.BIT.B7 = 0;
	PORTB.PDR.BIT.B7 = 1;
	PORTC.PMR.BIT.B2 = 0;
	PORTC.PDR.BIT.B2 = 1; //ここまでSENLED

	PORT4.PMR.BIT.B6 = 0; //FANをIO
	PORT4.PDR.BIT.B6 = 1; //FANを出力
	PORT4.PODR.BIT.B6 = 0; //FANをとめとく

	PORTA.PMR.BIT.B3 = 0; //Moter_l_in1
	PORTA.PDR.BIT.B3 = 1;
	PORTA.PMR.BIT.B4 = 0; //Moter_l_in2
	PORTA.PDR.BIT.B4 = 1;
	PORTB.PMR.BIT.B0 = 0; //Moter_stby
	PORTB.PDR.BIT.B0 = 1;
	PORTB.PMR.BIT.B5 = 0; //Moter_r_in1
	PORTB.PDR.BIT.B5 = 1;
	PORTB.PMR.BIT.B6 = 0; //Moter_r_in2
	PORTB.PDR.BIT.B6 = 1;
}

void Clock_Settting(void) {
	clrpsw_i(); /* ---- Disable maskable interrupts ---- */
	R_INIT_StopModule(); /* ---- Stopping the peripherals which start operations  ---- */
	R_INIT_NonExistentPort(); /* ---- Initialization of the non-existent ports ---- */
	R_INIT_Clock(); /* ---- Initialization of the clock ---- */
	setpsw_i();
}

void start_SEN(uint8_t mario_flag) {

	SEN_check_flag = 1;
	wait_time(2);
	while (SEN_R.now < SEN_R.reference || SEN_RF.now < SEN_RF.reference) { //
		moter_flag = 0;
	}
	SEN_check_flag = 0;

	speaker_on(C_5, 6.0, 240);

	angle_calibration_integral = 0.0;
	angle_calibration = 0.0;
	angle_calibration_counter = 0;
	angle_calibration_flag = 0;
	angle_calibration_flag = 1;
	while (angle_calibration_flag == 1) {

	}
	angle_calibration = angle_calibration_integral / 1000.0;
	moter_flag = 1;

	if (x.goal == 7 && y.goal == 7 && mario_flag == 1) {
//		mario_start(180, 1);
		speaker_on(C_5, 6.0, 240);
		wait_time(500);
	} else {

		speaker_on(C_5, 6.0, 240);
		wait_time(500);
	}
	rotation_deviation.cumulative = 0;

}

void fan_on(void) {
	diameter = diameter_absorption;
	FAN = 1;
	wait_time(1000);
}

void fan_off(void) {
	diameter = DIAMETER;
	LEFTEING = 0;
	RIGHTWING = 0;
	wait_time(500);
	FAN = 0;
}

