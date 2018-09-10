/*
 * AD.c

 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */
#include "AD.h"
#include "iodefine.h"
#include "variable.h"
#include "other.h"

#define AD_COUNT 10

void init_AD(void) {
	PORTE.PMR.BIT.B0 = 1;
	PORTE.PMR.BIT.B5 = 1;
	PORT4.PMR.BIT.B0 = 1;
	PORT4.PMR.BIT.B2 = 1;
	PORT4.PMR.BIT.B4 = 1;
	PORTE.PDR.BIT.B0 = 0;
	PORTE.PDR.BIT.B5 = 0;
	PORT4.PDR.BIT.B0 = 0;
	PORT4.PDR.BIT.B2 = 0;
	PORT4.PDR.BIT.B4 = 0;

	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA17 = 0; //S12ADの低消費電力モード解除
	SYSTEM.PRCR.WORD = 0xA500; //低消費電力モードレジスタのプロテクト
///////////////////////
	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)eee
/////////////////
	MPC.P40PFS.BIT.ASEL = 1; //P40をアナログ端子に使用
	MPC.P42PFS.BIT.ASEL = 1;
	MPC.P44PFS.BIT.ASEL = 1;
	MPC.PE0PFS.BIT.ASEL = 1;
	MPC.PE5PFS.BIT.ASEL = 1;

	S12AD.ADCSR.BIT.ADST = 0; //AD変換停止
	S12AD.ADCSR.BIT.TRGE = 0; //トリガによるAD変換の禁止
	S12AD.ADCSR.BIT.EXTRG = 1; //非同期トリガによるAD変換
	S12AD.ADCSR.BIT.CKS = 3; //PCLKの1分周に設定
	S12AD.ADCSR.BIT.ADCS = 0; //シングルスキャンモード

	S12AD.ADANS0.WORD = 0x2115; //AD変換5つを設定
	S12AD.ADADS0.WORD = 0x0; //AN000~AN015を非加算モード

	S12AD.ADANS1.BIT.ANS1 = 0; //AN016~An021は対象外に
	S12AD.ADADS1.BIT.ADS1 = 0; //AN016~AN021を非加算モード

	S12AD.ADCER.BIT.ACE = 1; //自動クリア許可
	S12AD.ADCER.BIT.ADRFMT = 0; //データを右詰に

	S12AD.ADEXICR.WORD = 0; //温度・内部基準電圧を使わない

	S12AD.ADCSR.BIT.ADST = 1;
}

void AD_SEN(void) {
	uint8_t i;
	int sen_r,sen_rf,sen_l,sen_lf;
	S12AD.ADANS0.WORD = 0x2114; //AD変換4つを設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}
	sen_r = -1 * S12AD.ADDR13;
	sen_lf = -1 * S12AD.ADDR2;
	sen_rf = -1 * S12AD.ADDR8;
	sen_l = -1 * S12AD.ADDR4;

	SENLED_L = 1;
	for(i=0;i<AD_COUNT;i++){

	}
	S12AD.ADANS0.WORD = 0x0010; //AD変換4つを設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}
	sen_l += S12AD.ADDR4;
	SENLED_L = 0;
///////
	SENLED_LF = 1;
	for(i=0;i<AD_COUNT;i++){

	}
	S12AD.ADANS0.WORD = 0x0004; //AD変換4つを設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}
	sen_lf += S12AD.ADDR2;
	SENLED_LF = 0;
///////
	SENLED_R = 1;
	for(i=0;i<AD_COUNT;i++){

	}
	S12AD.ADANS0.WORD = 0x2000; //AD変換4つを設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}
	sen_r += S12AD.ADDR13;
	SENLED_R = 0;
////////
	SENLED_RF = 1;
	for(i=0;i<AD_COUNT;i++){

	}
	S12AD.ADANS0.WORD = 0x0100; //AD変換4つを設定
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}
	sen_rf += S12AD.ADDR8;
	SENLED_RF = 0;
///////
///
///

	SEN_L.now=sen_l;
	SEN_R.now=sen_r;
	SEN_RF.now=sen_rf;
	SEN_LF.now=sen_lf;

	SEN_L_log.before_5ms = SEN_L_log.before_4ms;
	SEN_L_log.before_4ms = SEN_L_log.before_3ms;
	SEN_L_log.before_3ms = SEN_L_log.before_2ms;
	SEN_L_log.before_2ms = SEN_L_log.before_1ms;
	SEN_L_log.before_1ms = SEN_L_log.now;
	SEN_L_log.now = SEN_L.now;

	SEN_L.diff = (SEN_L_log.now - SEN_L_log.before_3ms);
	if (SEN_L.diff < 0) {
		SEN_L.diff = -1 * SEN_L.diff;
	}

	SEN_R_log.before_5ms = SEN_R_log.before_4ms;
	SEN_R_log.before_4ms = SEN_R_log.before_3ms;
	SEN_R_log.before_3ms = SEN_R_log.before_2ms;
	SEN_R_log.before_2ms = SEN_R_log.before_1ms;
	SEN_R_log.before_1ms = SEN_R_log.now;
	SEN_R_log.now = SEN_R.now;

	SEN_R.diff = (SEN_R_log.now - SEN_R_log.before_3ms);
	if (SEN_R.diff < 0) {
		SEN_R.diff = -1 * SEN_R.diff;
	}

	SEN_F.now = (SEN_LF.now + SEN_RF.now) / 2;
}

void output_SEN(void) {
	SEN_check_flag = 1;
	while (1) {
		myprintf("L:%3d,LF:%3d,RF:%3d,R:%3d\n", SEN_L.now, SEN_LF.now,
				SEN_RF.now, SEN_R.now);
		wait_time(10);
		if (SWITCH == 0) {
			break;
		}
	}
}

