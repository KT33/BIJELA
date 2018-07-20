/*
 * SPI.c
 *
 *  Created on: 2018/05/02
 *      Author: takao keisuke
 */

#include "SPI.h"
#include "iodefine.h"
#include "variable.h"
#include "stdint.h"
#include "other.h"

void init_SPI(void) {
	SYSTEM.PRCR.WORD = 0xA502; //低消費電力モードレジスタのプロテクト解除
	SYSTEM.MSTPCRB.BIT.MSTPB16 = 0; //RSPI1の低消費電力モード解除
	SYSTEM.PRCR.WORD = 0xA500; //低消費電力モードレジスタのプロテクト

	MPC.PWPR.BIT.B0WI = 0; //PFSWEへの書き込みを許可(0:許可、1;禁止)
	MPC.PWPR.BIT.PFSWE = 1; //PFSへの書き込みを許可(0:禁止、1;許可)

	PORTE.PMR.BIT.B1 = 1; //PE1を周辺機能で使用
	PORTE.PMR.BIT.B2 = 1; //PE2を周辺機能で使用
	PORTE.PMR.BIT.B3 = 1; //PE3を周辺機能で使用
	PORTE.PMR.BIT.B4 = 1; //PE4を周辺機能で使用

	MPC.PE1PFS.BIT.PSEL = 14; //PE1をPSPCKBに設定
	MPC.PE2PFS.BIT.PSEL = 14; //PE2をMOSIBに設定
	MPC.PE3PFS.BIT.PSEL = 13; //PE3をMISOBに設定
	MPC.PE4PFS.BIT.PSEL = 13; //PE4をSSLB0に設定

	RSPI1.SPCR.BIT.SPE = 0; //ビットレートの設定をするためにSPI1を無効に
	RSPI1.SPCR.BIT.MSTR = 0; //ビットレートの設定をするためにスレープモードに

	RSPI1.SPBR = 3; //ジャイロが7MHzまでらしいので下のと合わせて6.67Mbpsび設定
	RSPI1.SPCMD0.BIT.BRDV = 0; //上に同じく

	RSPI1.SSLP.BIT.SSL0P = 0; //Lowでアクティブ

	RSPI1.SPCMD0.BIT.CPHA = 1; //奇数エッジで変化、偶数エッジでサンプル
	RSPI1.SPCMD0.BIT.CPOL = 1; //クロックはアイドルでHigh
	RSPI1.SPCMD0.BIT.SSLKP = 1; //信号終了後保持////
	RSPI1.SPCMD0.BIT.SPB = 7; //8bitに設定/////
	RSPI1.SPCMD0.BIT.LSBF = 0; //MSBファースト

//	RSPI1.SPSCR.BIT.SPSLN = 0; //1シーケンスに設定
	RSPI1.SPCR.BIT.SPMS = 0; //SPI動作
	RSPI1.SPCR.BIT.TXMD = 0; //全2重線式シリアル
	RSPI1.SPCR.BIT.MSTR = 1; //マスタモード
	while (RSPI1.SPCR.BIT.MSTR != 1) {

	}
	RSPI1.SPDCR.BIT.SPLW = 0; //ワードアクセス
	RSPI1.SPCR.BIT.SPE = 0; //SPI機能を無効
}

void init_gyro(void) {

	communicate_gyro(0, 0x07, 0x3f); //PWR_MGMT_2
	wait_time(3);
	communicate_gyro(0, 0x05, 0x0); //LP_CONFIG
	wait_time(3);
	communicate_gyro(0, 0x06, 0x09); //PWR_MGMT_1
	wait_time(3);

	communicate_gyro(0, 0x7f, 0x20); //REG_BANK_SEL
	wait_time(3);
	communicate_gyro(0, 0x01, 0x06); //GYRO_CONFIG_1
	wait_time(3);
	communicate_gyro(0, 0x14, 0x02); //ACCEL_CONFIG+-2000dps
	wait_time(3);

	communicate_gyro(0, 0x7f, 0x00); //REG_BANK_SEL
	wait_time(3);
	communicate_gyro(0, 0x7, 0x00); //PWR_MGMT_2
}

void test_gyro(void) {
	uint8_t read_1, read_2;
	int16_t value;
	read_1 = communicate_gyro(1, 0x37, 0x0);
	read_2 = communicate_gyro(1, 0x38, 0x0);
	value = (int16_t) read_1 << 8;
	value = value | read_2;
//	myprintf("%d	%d	%d\n", read_1, read_2,value);
//	myprintf("value %f\n",(float)value*9.80665/8192); //加速度
	myprintf("%f\n", (float) value * 0.00106252644); //1000*pi/(2^15*180)
}

float test_gyro2(void) {
	uint8_t read_1, read_2;
	int16_t value;
	read_1 = communicate_gyro(1, 0x37, 0x0);
	read_2 = communicate_gyro(1, 0x38, 0x0);
	value = (int16_t) read_1 << 8;
	value = value | read_2;
//	myprintf("value %f\n",(float)value*9.80665/8192); //加速度
//	myprintf("%f\n",(float)value*0.00106252644); //1000*pi/(2^15*180)
//	angle=(float)value*0.00106252644; //1000*pi/(2^15*180)
	return (float) value * 0.00106252644;//1000*pi/(2^15*180)
}

uint8_t communicate_gyro(char io, uint8_t addres, uint8_t gyro_w) { //1,read 0,write
	uint16_t gyro_r;
	int i;

	io = io << 7;
	addres = addres | io; //書き込みと読み出しを区別

	RSPI1.SPCR.BIT.SPE = 1; //SPI機能を有効
	for (i = 0; i < 5; i++) {
	}

	//以下レジスタを設定
	RSPI1.SPDR.WORD.H = (uint16_t) addres;

	//RSPI1.SPDR.WORD= gyro_w;
	for (i = 0; i < 5; i++) {
	}
	while (RSPI1.SPSR.BIT.IDLNF == 1) {
	}
	gyro_r = RSPI1.SPDR.WORD.H;
	//以上レジスタを設定

	//以下データの受送信
	RSPI1.SPDR.WORD.H = (uint16_t) gyro_w;
	for (i = 0; i < 5; i++) {
	}
	while (RSPI1.SPSR.BIT.IDLNF == 1) {
	}
	gyro_r = RSPI1.SPDR.WORD.H;
	//以上データの受送信

	RSPI1.SPCR.BIT.SPE = 0; //SPI機能を無効

	return (uint8_t) gyro_r;
}

