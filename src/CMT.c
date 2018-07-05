/*
 * CMT.c
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#include "iodefine.h"
#include "variable.h"
#include "SPI.h"

void interrupt_cmt0(void) {
	g_count++;
	AD_all();
	if (gyro_flag == 1) {
		angle += test_gyro2() / 1000;
	} else {
		angle = 0;
	}
	encoder_L = TPU1.TCNT; //L
	encoder_R = TPU2.TCNT; //R
	TPU1.TCNT=0;
	TPU2.TCNT=0;

	speed_L.now=1*(float)encoder_L*diameter*3.14159265359/4096/4/40*11/0.001;
	speed_R.now=-1*(float)encoder_R*diameter*3.14159265359/4096/4/40*11/0.001;

	distance_L.now+=speed_L.now*0.001;
	distance_R.now+=speed_R.now*0.001;



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
