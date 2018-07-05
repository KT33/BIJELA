/*
 * speaker.c
 *
 *  Created on: 2018/05/06
 *      Author: takao keisuke
 */

#include "speaker.h"
#include "iodefine.h"
#include "variable.h"
#include "other.h"

void init_speaker(void) {
	PORTA.PMR.BIT.B1 = 1; //周辺機能として使用
	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)
	MPC.PA1PFS.BIT.PSEL = 3; //PA1をTIOCB0に設定

	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRA.BIT.MSTPA13 = 0; //RSPI1の低消費電力モード解除
	SYSTEM.PRCR.WORD = 0xA500; //低消費電力モードレジスタのプロテクト

	TPU0.TCR.BIT.TPSC = 1; //クロック周期:PCLK(50MHz)/4
	TPU0.TCR.BIT.CKEG = 1; //立ち上がりでカウントアップ
	TPU0.TCR.BIT.CCLR = 1; //TGRAでクリア

	TPU0.TMDR.BIT.MD = 3; //PWMモード2

	TPU0.TIORH.BIT.IOA = 0; //Aは出力禁止??????????????
	TPU0.TIORH.BIT.IOB = 2; //Bは初期LOWでコンペアマッチでHigh

	TPU0.TGRA = 60000; //初期値(意味なし)
	TPU0.TGRB = 30000; //初期値(意味なし)

	TPUA.TSTR.BIT.CST0 = 0; //TPU0を停止
}

void speaker_on(int frq, float beat, int bpm) {
	if (frq != REST) {
		TPU0.TGRA = (int) (12500000 / frq) - 1;
		TPU0.TGRB = TPU0.TGRA / 2;
		TPUA.TSTR.BIT.CST0 = 1;     // TPU5 0:停止,1:開始
	}
	wait_time((int) (60000.0f / bpm * 4.0f / beat - 10.0f));
	TPUA.TSTR.BIT.CST0 = 0;     // TPU5 0:停止,1:開始
	wait_time(10);

}

void nchancha(int bpm, int pitch) {
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * G_3, 6.000000, bpm);
	speaker_on(pitch * A_3, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);

	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * G_3, 6.000000, bpm);
	speaker_on(pitch * A_3, 6.000000, bpm);
}

void mizutamari(int bpm, int pitch) {
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 3.000000, bpm);
	speaker_on(pitch * A_4, 3.000000, bpm);
	speaker_on(pitch * G_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 3.000000, bpm);

	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * G_3, 6.000000, bpm);
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 1.2000, bpm);

	speaker_on(pitch * REST, 1.500000, bpm);
//////////////////////////////////////////////////////////////////////////////
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 3.000000, bpm);
	speaker_on(pitch * A_4, 3.000000, bpm);
	speaker_on(pitch * G_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 3.000000, bpm);

	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * G_3, 6.000000, bpm);
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 1.2000, bpm);

	speaker_on(pitch * REST, 1.500000, bpm);
////////////////////////////////////////////////////////////////////////////
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 3.000000, bpm);
	speaker_on(pitch * A_4, 3.000000, bpm);
	speaker_on(pitch * G_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 3.000000, bpm);

	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * G_3, 6.000000, bpm);
	speaker_on(pitch * F_3, 6.000000, bpm);
	speaker_on(pitch * B_b_3, 6.000000, bpm);
	speaker_on(pitch * C_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 1.2000, bpm);

	speaker_on(pitch * REST, 1.500000, bpm);
////////////////////////////////////////////////////////////////////////////
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 3.000000, bpm);
	speaker_on(pitch * A_4, 3.000000, bpm);
	speaker_on(pitch * G_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * D_4, 6.000000, bpm);
	speaker_on(pitch * E_b_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 3.000000, bpm);

	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * E_b_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * G_4, 6.000000, bpm);
	speaker_on(pitch * F_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 1.2000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);

}

void seven_ATM(int bpm, int pitch) {
	speaker_on(pitch * E_4, 6.000000, bpm);
	speaker_on(pitch * A_4, 6.000000, bpm);
	speaker_on(pitch * D_b_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_b_5, 6.000000, bpm);
	speaker_on(pitch * A_4, 6.000000, bpm);
	speaker_on(pitch * E_5, 6.000000, bpm);
	speaker_on(pitch * REST, 6.000000, bpm);

	speaker_on(pitch * G_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * B_4, 6.000000, bpm);
	speaker_on(pitch * REST, 6.000000, bpm);
	speaker_on(pitch * G_5, 6.000000, bpm);
	speaker_on(pitch * E_5, 6.000000, bpm);
	speaker_on(pitch * B_4, 6.000000, bpm);
	speaker_on(pitch * REST, 6.000000, bpm);
	speaker_on(pitch * G_b_5, 1.500000, bpm);
	speaker_on(pitch * REST, 6.000000, bpm);
}

void mario_start(int bpm, int pitch) {
	speaker_on(pitch * F_5, 18.000000, bpm);
	speaker_on(pitch * REST, 18.000000, bpm);
	speaker_on(pitch * B_b_4, 18.000000, bpm);
	speaker_on(pitch * D_5, 18.000000, bpm);
	speaker_on(pitch * REST, 18.000000, bpm);
	speaker_on(pitch * F_5, 18.000000, bpm);
	speaker_on(pitch * C_6, 3.000000, bpm);
	speaker_on(pitch * REST, 4.000000, bpm);

	speaker_on(pitch * A_4, 4.000000, bpm);
	speaker_on(pitch * REST, 3.000000, bpm);
	speaker_on(pitch * A_4, 4.000000, bpm);
	speaker_on(pitch * REST, 3.000000, bpm);
	speaker_on(pitch * A_4, 4.000000, bpm);
	speaker_on(pitch * REST, 3.000000, bpm);
	speaker_on(pitch * A_5, 2.00000, bpm);
}

void victory_fanfare(int bpm, int pitch) {
	speaker_on(pitch * G_4, 42.000000, bpm);
	speaker_on(pitch * A_4, 42.000000, bpm);
	speaker_on(pitch * B_4, 42.000000, bpm);
	speaker_on(pitch * C_5, 42.000000, bpm);
	speaker_on(pitch * D_5, 42.000000, bpm);
	speaker_on(pitch * E_5, 42.000000, bpm);
	speaker_on(pitch * G_b_5, 42.000000, bpm);
	speaker_on(pitch * G_5, 42.000000, bpm);
	speaker_on(pitch * A_5, 42.000000, bpm);
	speaker_on(pitch * B_5, 42.000000, bpm);
	speaker_on(pitch * C_6, 42.000000, bpm);

	speaker_on(pitch * C_5, 18.000000, bpm);
	speaker_on(pitch * REST, 18.000000, bpm);
	speaker_on(pitch * C_5, 18.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * A_b_4, 6.000000, bpm);
	speaker_on(pitch * B_b_4, 6.000000, bpm);
	speaker_on(pitch * C_5, 18.000000, bpm);
	speaker_on(pitch * REST, 18.000000, bpm);
	speaker_on(pitch * B_b_4, 18.000000, bpm);
	speaker_on(pitch * C_5, 6.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);
}

void cde(int bpm, int pitch) {
	speaker_on(pitch * C_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * D_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * E_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * F_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * G_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * A_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * B_5, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * C_6, 12.000000, bpm);
}

void hikakin(int bpm, float pitch) {
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * D_b_5, 4.000000, bpm);
	speaker_on(pitch * D_5, 6.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);

	speaker_on(pitch * D_5, 4.000000, bpm);
	speaker_on(pitch * E_5, 4.000000, bpm);
	speaker_on(pitch * D_5, 4.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);

}

void KirbyDance(int bpm, int pitch) {
	speaker_on(pitch * F_5, 12.000000, bpm);
	speaker_on(pitch * G_5, 12.000000, bpm);
	speaker_on(pitch * A_5, 12.000000, bpm);
	speaker_on(pitch * B_5, 12.000000, bpm);
	speaker_on(pitch * A_5, 12.000000, bpm);
	speaker_on(pitch * B_5, 12.000000, bpm);
	speaker_on(pitch * C_6, 6.000000, bpm);
	speaker_on(pitch * G_5, 12.000000, bpm);
	speaker_on(pitch * E_5, 6.000000, bpm);
	speaker_on(pitch * G_6, 12.000000, bpm);
	speaker_on(pitch * F_6, 6.000000, bpm);
	speaker_on(pitch * E_6, 12.000000, bpm);
	speaker_on(pitch * D_6, 6.000000, bpm);
	speaker_on(pitch * E_6, 12.000000, bpm);
	speaker_on(pitch * C_6, 4.000000, bpm);
	speaker_on(pitch * C_7, 12.000000, bpm);
	speaker_on(pitch * REST, 1.500000, bpm);
	speaker_on(pitch * C_6, 4.000000, bpm);
}

void HPB(int bpm, int pitch) {
	speaker_on(pitch * G_5, 9.000000, bpm);
	speaker_on(pitch * G_5, 18.000000, bpm);
	speaker_on(pitch * A_5, 6.000000, bpm);
	speaker_on(pitch * G_5, 6.000000, bpm);
	speaker_on(pitch * C_6, 6.000000, bpm);
	speaker_on(pitch * B_5, 6.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);

	speaker_on(pitch * G_5, 9.000000, bpm);
	speaker_on(pitch * G_5, 18.000000, bpm);
	speaker_on(pitch * A_5, 6.000000, bpm);
	speaker_on(pitch * G_5, 6.000000, bpm);
	speaker_on(pitch * D_6, 6.000000, bpm);
	speaker_on(pitch * C_6, 6.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);

	speaker_on(pitch * G_5, 9.000000, bpm);
	speaker_on(pitch * G_5, 18.000000, bpm);
	speaker_on(pitch * G_6, 6.000000, bpm);
	speaker_on(pitch * E_6, 6.000000, bpm);
	speaker_on(pitch * C_6, 9.000000, bpm);
	speaker_on(pitch * C_6, 18.000000, bpm);
	speaker_on(pitch * B_5, 6.000000, bpm);
	speaker_on(pitch * A_5, 6.000000, bpm);
	speaker_on(pitch * REST, 6.00000, bpm);

	speaker_on(pitch * F_6, 9.000000, bpm);
	speaker_on(pitch * F_6, 18.000000, bpm);
	speaker_on(pitch * E_6, 6.000000, bpm);
	speaker_on(pitch * C_6, 6.000000, bpm);
	speaker_on(pitch * D_6, 6.000000, bpm);
	speaker_on(pitch * C_6, 3.000000, bpm);
}
