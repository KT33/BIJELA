/*
 * Moter.c
 *
 *  Created on: 2018/05/06
 *      Author: takao keisuke
 */
#include "iodefine.h"
#include "Moter.h"
#include "other.h"

void init_Moter_PWM(void) {
	MTU.TSTR.BIT.CST0 = 0; //MTU0を動作停止
	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0; //MTUのモジュールストップ解除
	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)
	PORTB.PMR.BIT.B3 = 1; //PB3を周辺機能に設定
	PORTB.PMR.BIT.B1 = 1; //PB1を周辺機能に設定
	MPC.PB1PFS.BIT.PSEL = 1; //PB1をMTIOC0Cに設定
	MPC.PB3PFS.BIT.PSEL = 1; //PB3をMTIOC0Aに設定
	MTU0.TCR.BIT.CCLR = 1; //TGRAのコンペアマッチでTCNTをクリア
	MTU0.TCR.BIT.CKEG = 0; //立ち上がりエッジでカウント
	MTU0.TCR.BIT.TPSC = 0; //PCLK/1でカウント(50MHz)
	MTU0.TMDR.BIT.MD=2; //PWMモード1
	MTU0.TMDR.BIT.BFA = 0; //TGRA,TGRCは通常動作
	MTU0.TMDR.BIT.BFB = 0; //TGRB,TGRDは通常動作
	MTU0.TIORH.BIT.IOB = 5; //MTIOC0Bはhigh→Low
	MTU0.TIORL.BIT.IOD = 5; //MTIOC0Dはhigh→Low
	MTU0.TIORH.BIT.IOA = 6; //MTIOC0AはLow→High
	MTU0.TIORL.BIT.IOC = 6; //MTIOC0CはLow→High
	MTU0.TGRB=200;
	MTU0.TGRD=200;
	MTU0.TGRA=400; //125KHzに設定
	MTU0.TGRC=400; //125KHzに設定
	MTU.TSTR.BIT.CST0 = 1; //MTU0を動作停止

}

void init_Encoder(void) {

	TPUA.TSTR.BIT.CST1 = 0; //TPU1を動作停止
	TPUA.TSTR.BIT.CST2 = 0; //TPU2を動作停止
	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA13 = 0; //TPUのモジュールストップ解除
	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)
	PORT1.PMR.BIT.B7 = 1;
	PORT1.PMR.BIT.B6 = 1; //周辺機能として使用
	PORT1.PMR.BIT.B5 = 1;
	PORT1.PMR.BIT.B4 = 1;
	MPC.P14PFS.BIT.PSEL = 4;
	MPC.P15PFS.BIT.PSEL = 4;
	MPC.P16PFS.BIT.PSEL = 4;
	MPC.P17PFS.BIT.PSEL = 4;
	TPU1.TMDR.BIT.MD = 4;//位相係数モード1
	TPU2.TMDR.BIT.MD = 4;//位相係数モード1
	TPUA.TSTR.BIT.CST1 = 1; //TPU1を動作開始(L)
	TPUA.TSTR.BIT.CST2 = 1; //TPU2を動作開始(R)

//	MTU.TSTR.BIT.CST1 = 0; //MTU1を動作停止
//	MTU.TSTR.BIT.CST2 = 0; //MTU2を動作停止
//	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
//	SYSTEM.MSTPCRA.BIT.MSTPA9 = 0; //MTUのモジュールストップ解除
//	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
//	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)
//	PORT1.PMR.BIT.B7 = 1;
//	PORT1.PMR.BIT.B6 = 1; //周辺機能として使用
//	PORT1.PMR.BIT.B5 = 1;
//	PORT1.PMR.BIT.B4 = 1;
//	MPC.P14PFS.BIT.PSEL = 4;
//	MPC.P15PFS.BIT.PSEL = 4;
//	MPC.P16PFS.BIT.PSEL = 4;
//	MPC.P17PFS.BIT.PSEL = 4;
//	MTU1.TMDR.BIT.MD = 4; //MTU1位相係数モード1に設定
//	MTU2.TMDR.BIT.MD = 4; //MTU2を位相係数モード1に設定
//	MTU1.TSR.BIT.TCFD = 1; //TCNTをアップカウント
//	MTU2.TSR.BIT.TCFD = 1; //TCNをTアップカウント
//	MTU.TSTR.BIT.CST1 = 1; //MTU1を動作開始
//	MTU.TSTR.BIT.CST2 = 1; //MTU2を動作開始
}
