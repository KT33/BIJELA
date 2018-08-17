/*
 * walldata.c
 *
 *  Created on: 2018/08/18
 *      Author: takao keisuke
 */

#include "walldate.h"
#include "variable.h"
#include "iodefine.h"
#include "CMT.h"
#include "stdint.h"
#include "machine.h"
#include "other.h"

void addWall(void) {

}

void clear_Map(walldate_t *walldate) {
	uint8_t i;
	for (i = 0; i < 16; i++) {
		walldate->column[i] = 0;
		walldate->row[i] = 0;
	}
	walldate->column[0] = 0xffff;
	walldate->row[0] = 0xffff;
	walldate->column[16] = 0xffff;
	walldate->row[16] = 0xffff;
	walldate->column[5]=0x0ff0;
}

void output_Walldate(walldate_t *walldate) {
	int x_check, y_check;

	for (y_check = 15; y_check >= 0; y_check--) {
		myprintf("+");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, 1, walldate) == 1) {
				myprintf("-----");
			} else {
				myprintf("     ");
			}
			myprintf("+");
		}
		myprintf("\n");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, 4, walldate) == 1) {
				myprintf("|");
			} else {
				myprintf(" ");
			}
			myprintf(" %3d ", 0); //step_Map[x_check][y_check]
		}
		if (getWall(15, y_check, 2, walldate) == 1) {
			myprintf("|");
		} else {
			myprintf(" ");
		}
		myprintf("\n");
	}
	myprintf("+");
	for (x_check = 0; x_check < 16; x_check++) {
		if (getWall(x_check, y_check, 1, walldate) == 1) {
			myprintf("-----");
		} else {
			myprintf("     ");
		}
		myprintf("+");
	}
	myprintf("\n");
}

int getWall(int x_check, int y_check, int direction_check, walldate_t *walldate) { //(見たい座標のx,y,とその座標からの方角１～４壁があれば１
	int check_wall = 1;
	if (direction_check == 1) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == 2) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == 3) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == 4) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	}
	return check_wall;
}
