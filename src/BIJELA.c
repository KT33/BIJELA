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

//PCKBは50Mhz

void main() {
	volatile int i;
	init_ALL();
	mode_flag = 0;
	Moter_Stby = 1;
	Moter_R_FRONT = 1;

	//gyro_flag=1;
	while (1) {
		//	myprintf("%6.2f\n", angle);
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
			chattering();
			go_mode(mode_flag);
		}


	}
}
///////////////////
//	while (1) {
//
//		Moter_Stby = 1;
//		Moter_L_BACK = 0;
//		Moter_L_FRONT = 1;
//		Moter_R_FRONT = 1;
//		Moter_R_BACK = 0;
//		gyro_flag = 1;
//
//		wait_time(1000);
//		set_straight(5000, nomal_run.accel, 600.0, nomal_run.vel_min,
//				nomal_run.vel_min);
//		wait_straight();
//		LEFTFRONT = 1;
//		for (i = 0; i < LogMax; i++) {
//			myprintf("%.2f\n", log[i]);
//		}
//		break;
//	}
////////////////////

//			while (1) {
//				myprintf("%6.2f,%6.2f,%6.2f\n", translation_ideal.accel,
//						translation_ideal.velocity, translation_ideal.dis);
//			}

//			if (distance_L.now >= 1000000 || distance_L.now <= -1000000) {
//				distance_L.now = 0;
//			}
//			if (distance_R.now >= 1000000 || distance_R.now <= -1000000) {
//				distance_R.now = 0;
//			}

//			Moter_Stby = 1;
//			Moter_L_BACK = 1;
//			Moter_L_FRONT = 0;
//			Moter_R_FRONT = 0;
//			Moter_R_BACK = 1;

//			myprintf("speed_L:%8.2f,speed_R;%8.2f,distance_L:%8.2f,distance_R:%8.2f\n",
//				speed_L.now, speed_R.now, distance_L.now, distance_R.now);
//			myprintf("encoder_r:%8d,encoder_l:%8d\n",TPU1.TCNT,TPU2.TCNT);

//			wait_time(100);

//			mario_start(140, 1);
//			nchancha(200, 2);
//			for (i = 0; i < 1; i++) {
//				mizutamari(200, 2);
//			}
//			seven_ATM(240,1);
//			mario_start(140,1);
//			victory_fanfare(100, 1);
//
//			wait_time(1000);

//			Moter_Stby=1;
//			Moter_L_IN1=0;
//			Moter_L_IN2=1;
//			Moter_R_IN1=1;
//			Moter_R_IN2=0;

//			myprintf("%f\n", angle*360.0/(2*3.14));
//			HPB(120,1);
//			for (i = 0; i < 3; i++) {
//				UI_LED1 = 1;
//				UI_LED2 = 0;
//				UI_LED3 = 1;
//				RIGHTWING = 0;
//				LEFTEING = 1;
//				RIGHTFRONT = 1;
//				CENTERFRONT = 0;
//				wait_time(500);
//				UI_LED1 = 0;
//				UI_LED2 = 1;
//				UI_LED3 = 0;
//				RIGHTWING = 1;
//				LEFTEING = 0;
//				RIGHTFRONT = 0;
//				CENTERFRONT=1;
//				wait_time(500);
//			}
//			UI_LED1 = 0;
//			UI_LED2 = 0;
//			UI_LED3 = 0;
//			RIGHTWING = 0;
//			LEFTEING = 0;
//			RIGHTFRONT = 0;
//			CENTERFRONT = 0;
//			wait_time(500);
//			for(i=0;i<3;i++){
//				UI_LED1=1;
//				wait_time(200);
//				UI_LED1=0;
//				UI_LED2=1;
//				wait_time(200);
//				UI_LED2=0;
//				UI_LED3=1;
//				wait_time(200);
//				UI_LED3=0;
//				RIGHTWING=1;
//				wait_time(200);
//				RIGHTWING=0;
//				RIGHTFRONT=1;
//				wait_time(200);
//				RIGHTFRONT=0;
//				CENTERFRONT=1;
//				wait_time(200);
//				CENTERFRONT=0;
//				LEFTEING=1;
//				wait_time(200);
//				LEFTEING=0;
//
//			}

//		test_gyro();
//		gyro_r = communicate_gyro(0x80,0x0);
//		myprintf("WHO AM I %d\n", gyro_r);

