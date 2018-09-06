/*
 * dataflash.h
 *
 *  Created on: 2018/09/05
 *      Author: takao keisuke
 */

#ifndef DATAFLASH_H_
#define DATAFLASH_H_

#include "stdint.h"

#define FLD_ERROR 0
#define FLD_OK 1
#define FLD_BLANK 2
#define FLD_NOBLANK 3



void fld_init_fcu_ram(void);
int32_t fld_int_pclk_notification(void);
int32_t flf_blank_check_2KB(uint32_t addr);
int32_t fld_erase_2KB(uint32_t addr);
int32_t fld_program_8byte(uint32_t addr, uint16_t *ram);
int32_t fld_enable_read(void);

#endif /* DATAFLASH_H_ */
