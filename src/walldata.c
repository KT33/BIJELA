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

void add_West_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now] |= new;
}

void add_South_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now] |= new;
}

void add_East_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now + 1] |= new;
}

void add_North_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now + 1] |= new;
}

void pull_West_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now] &= ~new;
}

void pull_South_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now] &= ~new;
}

void pull_East_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= y.now;
	walldate->column[x.now + 1] &= ~new;
}

void pull_North_wall(walldate_t *walldate) {
	uint16_t new;
	new = 1;
	new <<= x.now;
	walldate->row[y.now + 1] &= ~new;
}

void addWall(void) {
//	uint16_t new;
	if (add_wall_flag == 1) {
		if (direction == North) {
			if (SEN_L.now > SEN_L.threshold) {
				add_West_wall(&walldate_real);
			} else {
				pull_West_wall(&walldate_adachi);
			}
			if (SEN_R.now > SEN_R.threshold) {
				add_East_wall(&walldate_real);
			} else {
				pull_East_wall(&walldate_adachi);
			}
			if (SEN_F.now > SEN_F.threshold) {
				add_North_wall(&walldate_real);
			} else {
				pull_North_wall(&walldate_adachi);
			}
			add_West_wall(&walldate_checked);
			add_East_wall(&walldate_checked);
			add_North_wall(&walldate_checked);
		} else if (direction == West) {
			if (SEN_L.now > SEN_L.threshold) {
				add_South_wall(&walldate_real);
			} else {
				pull_South_wall(&walldate_adachi);
			}
			if (SEN_R.now > SEN_R.threshold) {
				add_North_wall(&walldate_real);
			} else {
				pull_North_wall(&walldate_adachi);
			}
			if (SEN_F.now > SEN_F.threshold) {
				add_West_wall(&walldate_real);
			} else {
				pull_West_wall(&walldate_adachi);
			}
			add_West_wall(&walldate_checked);
			add_South_wall(&walldate_checked);
			add_North_wall(&walldate_checked);
		} else if (direction == South) {
			if (SEN_L.now > SEN_L.threshold) {
				add_East_wall(&walldate_real);
			} else {
				pull_East_wall(&walldate_adachi);
			}
			if (SEN_R.now > SEN_R.threshold) {
				add_West_wall(&walldate_real);
			} else {
				pull_West_wall(&walldate_adachi);
			}
			if (SEN_F.now > SEN_F.threshold) {
				add_South_wall(&walldate_real);
			} else {
				pull_South_wall(&walldate_adachi);
			}
			add_West_wall(&walldate_checked);
			add_South_wall(&walldate_checked);
			add_East_wall(&walldate_checked);
		} else if (direction == East) {
			if (SEN_L.now > SEN_L.threshold) {
				add_North_wall(&walldate_real);
			} else {
				pull_North_wall(&walldate_adachi);
			}
			if (SEN_R.now > SEN_R.threshold) {
				add_South_wall(&walldate_real);
			} else {
				pull_South_wall(&walldate_adachi);
			}
			if (SEN_F.now > SEN_F.threshold) {
				add_East_wall(&walldate_real);
			} else {
				pull_East_wall(&walldate_adachi);
			}
			add_North_wall(&walldate_checked);
			add_South_wall(&walldate_checked);
			add_East_wall(&walldate_checked);
		}
	}
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
	walldate->column[1] = 0x0001;
}

void clear_adachiMap(walldate_t *walldate) {
	uint8_t i;
	for (i = 0; i < 17; i++) {
		walldate->column[i] = 0xffff;
		walldate->row[i] = 0xffff;
		walldate->row[1] = 0xfffe;
	}
}

void output_Walldate(walldate_t *walldate) {
	int x_check, y_check;
	myprintf("\n");
	for (y_check = 15; y_check >= 0; y_check--) {
		myprintf("\x1b[31m");
		myprintf("+");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, North, walldate) == 1) {
				myprintf("\x1b[31m");
				myprintf("-----");
			} else {
				myprintf("\x1b[m");
				myprintf("     ");
			}
			myprintf("\x1b[31m");
			myprintf("+");
		}
		myprintf("\n");
		for (x_check = 0; x_check < 16; x_check++) {
			if (getWall(x_check, y_check, West, walldate) == 1) {
				myprintf("\x1b[31m");
				myprintf("|");
			} else {
				myprintf(" ");
			}
			myprintf("\x1b[m");
			if (step_map[x_check][y_check] < 1000) {
				myprintf(" %3d ", step_map[x_check][y_check]); //step_Map[x_check][y_check]
			} else {
				myprintf(" %3d ", 999);
			}
		}
		if (getWall(15, y_check, East, walldate) == 1) {
			myprintf("\x1b[31m");
			myprintf("|");
		} else {
			myprintf(" ");
		}
		myprintf("\n");
	}
	myprintf("+");
	y_check = 0;
	for (x_check = 0; x_check < 16; x_check++) {
		if (getWall(x_check, y_check, South, walldate) == 1) {
			myprintf("\x1b[31m");
			myprintf("-----");
		} else {
			myprintf("     ");
		}
		myprintf("+");
	}
	myprintf("\x1b[m");
	myprintf("\n");
	myprintf("\n");
}

int getWall(int x_check, int y_check, int direction_check, walldate_t *walldate) { //(見たい座標のx,y,とその座標からの方角１～４壁があれば１
	int check_wall = 1;
	if (direction_check > 3) {
		direction_check = direction_check - 4;
	}
	if (direction_check == North) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == East) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check + 1];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == South) {
		check_wall <<= x_check;
		check_wall &= walldate->row[y_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	} else if (direction_check == West) {
		check_wall <<= y_check;
		check_wall &= walldate->column[x_check];
		if (check_wall != 0) {
			check_wall = 1;
		}
	}
	return check_wall;
}
