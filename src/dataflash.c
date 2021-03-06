/*
 * dataflash.c
 *
 *  Created on: 2018/09/05
 *      Author: takao keisuke
 */

#include "dataflash.h"
#include "iodefine.h"
#include "other.h"
#include "variable.h"
#include "stdint.h"
#include "speaker.h"

void fld_init_fcu_ram(void) {
	int i;
	static const int fcu_ram_size = 8 * 1024; //8K Byte
	volatile uint32_t *fcu_ram = (uint32_t *) 0x007F8000;
	const volatile uint32_t *fuc_fw = (uint32_t *) 0xFEFFE000;

	//change to read mode
//	if (FLASH.FENTRYR.WORD & 0x00ff) { //p1865
	FLASH.FENTRYR.WORD = 0xAA00; //AAh is key
//	}

	//enable access FCU RAM
	FLASH.FCURAME.WORD = 0xC401; //p1861

	//copy to FCU RAM from FCU F/W
	for (i = 0; i < fcu_ram_size / sizeof(uint32_t); i++) {
		*fcu_ram++ = *fuc_fw++; //
	}
}

int32_t fld_int_pclk_notification(void) {
	//0010 0000h is top address of DB00 block
	volatile uint8_t *addr_b = (uint8_t *) 0x00100000;
	volatile uint16_t *addr_w = (uint16_t *) 0x00100000;

	//change PE mode
//	if (FLASH.FENTRYR.WORD & 0x00ff != 0x0080) {
	FLASH.FENTRYR.WORD = 0xAA80;
//	}

	//set peripheral clock
	FLASH.PCKAR.WORD = 50; //PCLKB=50MHz
//	FLASH.FASTAT.BIT.CMDLK=0;

	//周知クロック通知コマンドの発行
	*addr_b = 0xE9;
	*addr_b = 0x03;
	*addr_w = 0x0F0F;
	*addr_w = 0x0F0F;
	*addr_w = 0x0F0F;
	*addr_b = 0xD0;

	//コマンド完了待ち
	wait_time(5);
	while (FLASH.FSTATR0.BIT.FRDY == 0) {

	}

	//エラーチェック
	if (FLASH.FSTATR0.BIT.ILGLERR == 1) {
		RIGHTFRONT = 1;
		return FLD_ERROR;
	}
	return FLD_OK;
}

int32_t flf_blank_check_2KB(uint32_t addr) {
	volatile uint8_t *addr_b = (uint8_t *) addr;

	//ブランクチェック許可
	FLASH.FMODR.BIT.FRDMD = 1;

	//チェックサイズの設定
	FLASH.DFLBCCNT.BIT.BCSIZE = 1;

	FLASH.FASTAT.BIT.CMDLK = 0;

	//ブランクチェックコマンド
	*addr_b = 0x71;
	*addr_b = 0xD0;

	//コマンド完了待ち
	wait_time(5);
	while (FLASH.FSTATR0.BIT.FRDY == 0) {

	}

	//エラーチェック
	if (FLASH.FSTATR0.BIT.ILGLERR == 1) {
		return FLD_ERROR; //エラー
		CENTERFRONT = 1;
	}

	//チェック結果の受け取り
	if (FLASH.DFLBCSTAT.BIT.BCST == 0) {
		return FLD_BLANK; //ブランク

	}

	return FLD_NOBLANK; //ブランクじゃない
}

int32_t fld_erase_2KB(uint32_t addr) {
	volatile uint8_t *addr_b = (uint8_t *) addr;
	int32_t ret = FLD_OK;
	if (FLASH.FENTRYR.WORD != 0xAA80) {
		FLASH.FENTRYR.WORD = 0xAA80;
	}
	if (FLASH.FENTRYR.WORD != 0xAA80) {
		FLASH.FENTRYR.WORD = 0xAA80;
	}

	//消去プロテクトの解除
	FLASH.FWEPROR.BYTE = 0x01;

	//ブロック単位の消去許可
	FLASH.DFLWE0.WORD = 0x1EFF;
	FLASH.DFLWE1.WORD = 0xE1FF;

//	FLASH.FASTAT.BIT.CMDLK=0;

//	for (i = 0; i < 16; i++) {
//		addr_b=(uint8_t*)((uint32_t)addr+(uint32_t)128*i);
	//ブロックイレースコマンド発行
	*addr_b = 0x20;
	*addr_b = 0xD0;

//	wait_time(10);
	while (FLASH.FSTATR0.BIT.FRDY == 0) {

	}

//	if (FLASH.FASTAT.BIT.DFLAE == 1) {
//		LEFTFRONT = 1;
//	}

	if (FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1) {
		LEFTFRONT = 1;
		ret = FLD_ERROR;
	}
//	if (FLASH.FSTATR0.BIT.ERSERR == 1) {
//			RIGHTFRONT = 1;
//		ret = FLD_ERROR;
//	}

//	}
	//protect and disable to erase
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	FLASH.DFLWE1.WORD = 0xE100;

	return ret;
}

int32_t fld_program_2byte(uint32_t addr, uint16_t *ram) {
	int32_t ret = FLD_OK;
	volatile uint8_t *addr_b = (uint8_t *) addr;
	volatile uint16_t *adde_w = (uint16_t *) addr;

	if (FLASH.FENTRYR.WORD != 0xAA80) {
		FLASH.FENTRYR.WORD = 0xAA80;
	}
	if (FLASH.FENTRYR.WORD != 0xAA80) {
		FLASH.FENTRYR.WORD = 0xAA80;
	}
	//書き込みプロテクトの解除
	FLASH.FWEPROR.BIT.FLWE = 1;

	//ブロック単位の書き込み許可

	FLASH.DFLWE0.WORD = 0x1EFF;
	FLASH.DFLWE1.WORD = 0xE1FF;

	FLASH.FASTAT.BIT.CMDLK = 0;

	//プログラムコマンド発行
	*addr_b = 0xE8;
	*addr_b = 0x01;
//	for (i = 0; i < 4; i++) {
	*adde_w = *ram;
//	}
	*addr_b = 0xD0;

	//コマンド完了待ち
//	wait_time(5);
	while (FLASH.FSTATR0.BIT.FRDY == 0) {

	}

	//エラーチェック
	if (FLASH.FSTATR0.BIT.ILGLERR == 1 || FLASH.FSTATR0.BIT.ERSERR == 1) {
		RIGHTWING = 1;
		ret = FLD_ERROR;
	}

	//protect and disable to erase
	FLASH.FWEPROR.BIT.FLWE = 2;
	FLASH.DFLWE0.WORD = 0x1E00;
	FLASH.DFLWE1.WORD = 0xE100;

	return ret;
}

int32_t fld_enable_read(void) {
	//check to read mode
	if (FLASH.FENTRYR.WORD & 0x00ff) { //p1865
		FLASH.FENTRYR.WORD = 0xAA00; //AAh is key
	}

	//読み出し許可
	FLASH.DFLRE0.WORD = 0x2DFF;
	FLASH.DFLRE1.WORD = 0xD2FF;

	return FLD_OK;
}

void init_dataflash(void) {
	FLASH.FRESETR.BIT.FRESET = 1;
	wait_time(10);
	FLASH.FRESETR.BIT.FRESET = 0;
	wait_time(10);

	fld_init_fcu_ram();
	fld_int_pclk_notification();
}

void erase_unit(uint16_t block) {
	uint32_t addr = (uint32_t) (0x00100000 + block * 32);
	fld_erase_2KB(addr);
}

void erase_all(void) {
	uint16_t i;
	for (i = 0; i < 1024; i++) {
		erase_unit(i);
//		speaker_on(1 * C_5, 6.000000, 100);
	}
}

void write_unit(uint16_t block, uint16_t data) {
	uint32_t addr = (uint32_t) (0x00100000 + block * 32);
	fld_program_2byte(addr, &data);
}

void write_walldata(uint16_t start_block, walldate_t walldata) {
	uint8_t i;
	for (i = 0; i < 17; i++) {
		write_unit(start_block + i, walldata.column[i]);
	}
	for (i = 0; i < 17; i++) {
		write_unit(start_block + i + 17, walldata.row[i]);
	}
}

void read_walldata(uint16_t start_block, walldate_t *walldata) {
	uint8_t i;
	uint16_t *read;
	fld_enable_read();
	for (i = 0; i < 17; i++) {
		read = (uint16_t *) (0x00100000 + (start_block + i) * 32);
		walldata->column[i] = *read;
	}
	for (i = 0; i < 17; i++) {
		read = (uint16_t *) (0x00100000 + (start_block + i + 17) * 32);
		walldata->row[i] = *read;
	}
}

void write_all_walldatas(uint8_t flag) {
	if (failsafe_flag == 0) {
		init_dataflash();
		erase_all();
		write_walldata(0, walldate_real);
		write_walldata(40, walldate_checked);
		write_walldata(80, walldate_adachi);
		if (flag != 255) {
			speaker_on(C_5, 6.0, 240);
		}else{
			hikakin(250, 1);
		}
//		cde(250, 1);
	}
}

void read_all_walldatas(void) {
	init_dataflash();
	read_walldata(0, &walldate_real);
	read_walldata(40, &walldate_checked);
	read_walldata(80, &walldate_adachi);
//b	hikakin(220, 1);
}

