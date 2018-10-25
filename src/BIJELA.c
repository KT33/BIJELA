/***************************************************************/
/*                                                             */
/*      PROJECT NAME :  BIJELA                                 */
/*      FILE         :  BIJELA.c                               */
/*      DESCRIPTION  :  Main Program                           */
/*      CPU SERIES   :  RX600                                  */
/*      CPU TYPE     :  RX631                                  */
/*                                                             */
/*      This file is generated by e2 studio.                   */
/*                                                             */
/***************************************************************/
/***********************************************************************/
/*                                                                     */
/*  FILE        :Main.c or Main.cpp                                    */
/*  DATE        :Tue, Oct 31, 2006                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :                                                      */
/*                                                                     */
/*  NOTE:THIS IS A TYPICAL EXAMPLE.                                    */
/*                                                                     */
/***********************************************************************/
//#include "typedefine.h"
#include "iodefine.h"
#include "variable.h"
#include "machine.h"
#include "other.h"
#include "CMT.h"
#include "AD.h"
#include "Clock.h"
#include "SPI.h"
#include "speaker.h"
#include "Moter.h"
#include "run.h"
#include "dataflash.h"
#include "motion.h"

//PCKBは50Mhz

//600の時
//nomal_rotation.accel=9000.0
//nomal_rotation.vel_search=750.0

//left 10→25
//right 11→20

void main() {
	volatile int i;
	init_ALL();
	mode_flag = 0;
	Moter_Stby = 1;
	Moter_R_FRONT = 1;
//	init_dataflash();
	//gyro_flag=1;
	LED_Setting();
	CENTERFRONT = 1;
	while (1) {
		//	myprintf("%6.2f\n", angle);
		x.goal = 7;
		y.goal = 4;
		ui_led_3bit(mode_flag);
		if (mode_select_dis > 50) {
			mode_select_dis = 0;
			mode_flag++;
			if (mode_flag >= 8) {
				mode_flag = 0;
			}
			mode_select_speaker(mode_flag);
		}
		if (SWITCH == 0) {
			ui_reset();
			mode_select_speaker(mode_flag);
			chattering();
			go_mode(mode_flag);
		}
		translation_ideal.accel = 0.0;
		translation_ideal.velocity = 0.0;
	}
}

