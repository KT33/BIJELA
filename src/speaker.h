/*
 * speaker.h
 *
 *  Created on: 2018/05/06
 *      Author: takao keisuke
 */

#ifndef SPEAKER_H_
#define SPEAKER_H_

#define A_3 220
#define B_b_3 233
#define B_3 247
#define C_3 262
#define D_b_3 277
#define D_3 294
#define E_b_3 311
#define E_3 330
#define F_3 394
#define G_b_3 370
#define G_3 392
#define A_b_3 415
#define A_4 440
#define B_b_4 466
#define B_4 494
#define C_5 523
#define D_b_5 554
#define D_5 587
#define E_b_5 622
#define E_5 659
#define F_5 698
#define G_b_5 740
#define G_5 784
#define A_b_5 831
#define A_5 880
#define B_b_5 932
#define B_5 988
#define C_6 1047
#define D_b_6 1109
#define D_6 1175
#define E_b_6 1245
#define E_6 1319
#define F_6 1397
#define G_b_6 1480
#define G_6 1568
#define A_b_6 1661
#define A_6 1760
#define B_b_6 1865
#define B_6 1976
#define C_7 2093
#define D_b_7 2217
#define D_7 2349
#define E_b_7 2489
#define E_7 2637
#define F_7 2794
#define G_b_7 2960
#define G_7 3136
#define A_b_7 3322
#define A_7 3520
#define B_b_7 3729
#define B_7 3951
#define C_8 4186
#define REST 0


void init_speaker(void);
void speaker_on(int,float,int);
void KirbyDance(int, int);

#endif /* SPEAKER_H_ */
