/*
 * pass.c
 *
 *  Created on: 2018/08/24
 *      Author: takao keisuke
 */

#include "pass.h"
#include "iodefine.h"
#include "run.h"
#include "adachi.h"
#include "walldate.h"
#include "other.h"

void make_pass(uint8_t goal_x,uint8_t goal_y){
	uint16_t i=0;
	uint8_t flag;
	for(i=0;i<500;i++){
		pass[i]=0x0;
	}
	x.pass=x.now;
	y.pass=y.now;
	direction_pass=direction;
	adachi_map(goal_x, goal_y, walldate_adachi);
	coordinate_pass();
	i=0;
	while(1){

		if(x.pass==goal_x&&y.pass==goal_y){
			direction_pass+=2;
			pass[i]=0xff;
			break;
		}

		flag=how_to_move(direction_pass, (int8_t)x.pass,(int8_t) y.pass,walldate_adachi);
		myprintf("flag:%d,%d,%d\n",flag,x.pass,y.pass);
		if(flag==0){
			pass[i]=0;
		}
		if(flag==1){
			direction_pass+=1;
			pass[i]=1;
		}
		if(flag==3){
			direction_pass+=3;
			pass[i]=3;
		}
		if(flag==2||flag==4){
			direction_pass+=2;
			pass[i]=2;
		}

		if (direction_pass > 3) {
			direction_pass -= 4;
		}
		coordinate_pass();
		i++;
	}
}
