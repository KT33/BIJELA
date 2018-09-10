/*
 * SPI.h
 *
 *  Created on: 2018/05/02
 *      Author: takao keisuke
 */

#ifndef SPI_H_
#define SPI_H_
#include "stdint.h"

void init_SPI();
void init_gyro(void);
float test_gyro();
float test_gyro2();
uint8_t  communicate_gyro(char, uint8_t ,uint8_t);


#endif /* SPI_H_ */
