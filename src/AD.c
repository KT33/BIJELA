/*
 * AD.c

 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */
#include "AD.h"
#include "iodefine.h"
#include "variable.h"

void init_AD(void) {
	PORTE.PMR.BIT.B0 = 1;
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

void AD_all(void) {
	S12AD.ADCSR.BIT.ADST = 1;
	while (S12AD.ADCSR.BIT.ADST == 1) {

	}

	Batt = S12AD.ADDR0;
	Battery=(float)Batt*0.00248648;//(9.97+20.8)/9.97*3.3/4096

	SEN_R.now=S12AD.ADDR13;
	SEN_LF.now=S12AD.ADDR2;
	SEN_RF.now=S12AD.ADDR8;
	SEN_L.now=S12AD.ADDR4;

	SEN_L_log.before_5ms = SEN_L_log.before_4ms;
	SEN_L_log.before_4ms = SEN_L_log.before_3ms;
	SEN_L_log.before_3ms = SEN_L_log.before_2ms;
	SEN_L_log.before_2ms = SEN_L_log.before_1ms;
	SEN_L_log.before_1ms = SEN_L_log.now;
	SEN_L_log.now = SEN_L.now;

	SEN_L.diff = (SEN_L_log.now - SEN_L_log.before_5ms);
	if (SEN_L.diff < 0) {
		SEN_L.diff = -1 * SEN_L.diff;
	}

	SEN_R_log.before_5ms = SEN_R_log.before_4ms;
	SEN_R_log.before_4ms = SEN_R_log.before_3ms;
	SEN_R_log.before_3ms = SEN_R_log.before_2ms;
	SEN_R_log.before_2ms = SEN_R_log.before_1ms;
	SEN_R_log.before_1ms = SEN_R_log.now;
	SEN_R_log.now = SEN_R.now;

	SEN_R.diff = (SEN_R_log.now - SEN_R_log.before_5ms);
	if (SEN_R.diff < 0) {
		SEN_R.diff = -1 * SEN_R.diff;
	}
}

