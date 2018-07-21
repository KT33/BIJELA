/*
 * mode.h
 *
 *  Created on: 2018/07/16
 *      Author: takao keisuke
 */

#ifndef MODE_H_
#define MODE_H_

#include "stdint.h"

void mode_0(void);
void mode_1(void);
void mode_2(void);
void mode_3(void);
void mode_4(void);
void mode_5(void);
void mode_6(void);
void mode_7(void);

void go_mode(uint8_t mode);

#endif /* MODE_H_ */
