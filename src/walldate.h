/*
 * walldate.h
 *
 *  Created on: 2018/08/18
 *      Author: takao keisuke
 */

#ifndef WALLDATE_H_
#define WALLDATE_H_

#include "variable.h"
#include "stdint.h"

void addWall(void);
void clear_Map(walldate_t *walldate);
void clear_adachiMap(walldate_t *walldate);
void output_Walldate(walldate_t*);
int getWall(int x_check, int y_check, int direction_check, walldate_t *walldate); //(見たい座標のx,y,とその座標からの方角１～４壁があれば１

#endif /* WALLDATE_H_ */
