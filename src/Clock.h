/*
 * Clock.h
 *
 *  Created on: 2018/04/18
 *      Author: takao keisuke
 */

#ifndef CLOCK_H_
#define CLOCK_H_

/* ---- Please set the main clock and the sub-clock ---- */
#define MAIN_CLOCK_Hz  (12500000L)       /* This sample code uses 12MHz */
#define SUB_CLOCK_Hz   (32768L)          /* This sample code uses 32.768kHz */

/* ---- Please set wait processing for the clock oscillation stabilization ---- */
#define WAIT_TIME_FOR_MAIN_OSCILLATION (11026000L)
#define WAIT_TIME_FOR_SUB_OSCILLATION  (2600000000L)
//#define WAIT_TIME_FOR_SUB_OSCILLATION  (3300000000L) /* 64Pin Package */
#define WAIT_TIME_FOR_PLL_OSCILLATION  (1865000L)
#define WAIT_TIME_FOR_HOCO_OSCILLATION (2000000L)

/* ---- Please choose the sub-clock pattern ---- */
#define PATTERN_A       (1)             /* Sub-clock pattern A */
#define PATTERN_B       (2)             /* Sub-clock pattern B */
#define PATTERN_C       (3)             /* Sub-clock pattern C */
#define PATTERN_D       (4)             /* Sub-clock pattern D */
#define PATTERN_E       (5)             /* Sub-clock pattern E */
#define PATTERN_48      (PATTERN_A)     /* Sub-clock pattern 48Pin Package */

/* Select the sub-clock pattern to use. (Pattern A to E) */
#define SELECT_SUB      (PATTERN_A)     /* This sample code uses pattern A. */

//#define LOW_CL                        /* Drive ability for Low CL Used */

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void R_INIT_Clock(void);
void CGC_oscillation_main(void);
void CGC_oscillation_PLL(void);
void CGC_oscillation_HOCO(void);
void CGC_no_use_subclk(void);
void CGC_disable_subclk_RTC_use_mainclk(void);
void CGC_subclk_as_sysclk(void);
void CGC_subclk_as_RTC(void);
void CGC_subclk_as_sysclk_RTC(void);

/////////////////////////////////////////////////////////////////

/******************************************************************************
Macro definitions
******************************************************************************/
#define PIN_SIZE 64


#if (PIN_SIZE == 177) || (PIN_SIZE == 176)
 #define DEF_P0PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P1PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P2PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P3PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P4PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P5PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P6PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P7PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P8PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P9PDR         (0x00)       /* non-existent pin: none */
 #define DEF_PAPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PBPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PCPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PDPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PEPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PFPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PGPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PJPDR         (0x00)       /* non-existent pin: none */

#elif (PIN_SIZE == 145) || (PIN_SIZE == 144)
 #define DEF_P0PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P1PDR         (0x03)       /* non-existent pin: P10, P11 */
 #define DEF_P2PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P3PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P4PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P5PDR         (0x80)       /* non-existent pin: P57 */
 #define DEF_P6PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P7PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P8PDR         (0x30)       /* non-existent pin: P84, P85 */
 #define DEF_P9PDR         (0xF0)       /* non-existent pin: P94 to P97 */
 #define DEF_PAPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PBPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PCPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PDPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PEPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PFPDR         (0x1F)       /* non-existent pin: PF0 to PF4 */
 #define DEF_PGPDR         (0xFF)       /* non-existent pin: PG0 to PG7 */
 #define DEF_PJPDR         (0x00)       /* non-existent pin: none */

#elif PIN_SIZE == 100
 #define DEF_P0PDR         (0x0F)       /* non-existent pin: P00 to P03 */
 #define DEF_P1PDR         (0x03)       /* non-existent pin: P10, P11 */
 #define DEF_P2PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P3PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P4PDR         (0x00)       /* non-existent pin: none */
 #define DEF_P5PDR         (0xC0)       /* non-existent pin: P56, P57 */
 #define DEF_P6PDR         (0xFF)       /* non-existent pin: P60 to P67 */
 #define DEF_P7PDR         (0xFF)       /* non-existent pin: P70 to P77 */
 #define DEF_P8PDR         (0xFF)       /* non-existent pin: P80 to P87 */
 #define DEF_P9PDR         (0xFF)       /* non-existent pin: P90 to P97 */
 #define DEF_PAPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PBPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PCPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PDPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PEPDR         (0x00)       /* non-existent pin: none */
 #define DEF_PFPDR         (0x3F)       /* non-existent pin: PF0 to PF5 */
 #define DEF_PGPDR         (0xFF)       /* non-existent pin: PG0 to PG7 */
 #define DEF_PJPDR         (0x20)       /* non-existent pin: PJ5 */

#elif PIN_SIZE == 64
 #define DEF_P0PDR         (0x8F)       /* non-existent pin: P00 to P03, P07 */
 #define DEF_P1PDR         (0x0F)       /* non-existent pin: P10 to P13 */
 #define DEF_P2PDR         (0x3F)       /* non-existent pin: P20 to P25 */
 #define DEF_P3PDR         (0x1C)       /* non-existent pin: P32 to P34 */
 #define DEF_P4PDR         (0xA0)       /* non-existent pin: P45, P47 */
 #define DEF_P5PDR         (0xCF)       /* non-existent pin: P50 to P53, P56, P57 */
 #define DEF_P6PDR         (0xFF)       /* non-existent pin: P60 to P67 */
 #define DEF_P7PDR         (0xFF)       /* non-existent pin: P70 to P77 */
 #define DEF_P8PDR         (0xFF)       /* non-existent pin: P80 to P87 */
 #define DEF_P9PDR         (0xFF)       /* non-existent pin: P90 to P97 */
 #define DEF_PAPDR         (0xA4)       /* non-existent pin: PA2, PA5, PA7 */
 #define DEF_PBPDR         (0x14)       /* non-existent pin: PB2, PB4 */
 #define DEF_PCPDR         (0x03)       /* non-existent pin: PC0, PC1 */
 #define DEF_PDPDR         (0xFF)       /* non-existent pin: PD0 to PD7 */
 #define DEF_PEPDR         (0xC0)       /* non-existent pin: PE6, PE7 */
 #define DEF_PFPDR         (0x3F)       /* non-existent pin: PF0 to PF5 */
 #define DEF_PGPDR         (0xFF)       /* non-existent pin: PG0 to PG7 */
 #define DEF_PJPDR         (0x28)       /* non-existent pin: PJ3, PJ5 */

#elif PIN_SIZE == 48
 #define DEF_P0PDR         (0xAF)       /* non-existent pin: P00 to P03, P05, P07 */
 #define DEF_P1PDR         (0x0F)       /* non-existent pin: P10 to P13 */
 #define DEF_P2PDR         (0x3F)       /* non-existent pin: P20 to P25 */
 #define DEF_P3PDR         (0x1C)       /* non-existent pin: P32 to P34 */
 #define DEF_P4PDR         (0xB8)       /* non-existent pin: P43 to P45, P47 */
 #define DEF_P5PDR         (0xFF)       /* non-existent pin: P50 to P57 */
 #define DEF_P6PDR         (0xFF)       /* non-existent pin: P60 to P67 */
 #define DEF_P7PDR         (0xFF)       /* non-existent pin: P70 to P77 */
 #define DEF_P8PDR         (0xFF)       /* non-existent pin: P80 to P87 */
 #define DEF_P9PDR         (0xFF)       /* non-existent pin: P90 to P97 */
 #define DEF_PAPDR         (0xA5)       /* non-existent pin: PA0, PA2, PA5, PA7 */
 #define DEF_PBPDR         (0xD4)       /* non-existent pin: PB2, PB4, PB6, PB7 */
 #define DEF_PCPDR         (0x0F)       /* non-existent pin: PC0 to PC3 */
 #define DEF_PDPDR         (0xFF)       /* non-existent pin: PD0 to PD7 */
 #define DEF_PEPDR         (0xE1)       /* non-existent pin: PE0, PE5 to PE7 */
 #define DEF_PFPDR         (0x3F)       /* non-existent pin: PF0 to PF5 */
 #define DEF_PGPDR         (0xFF)       /* non-existent pin: PG0 to PG7 */
 #define DEF_PJPDR         (0x28)       /* non-existent pin: PJ3, PJ5 */

#else
#endif

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void R_INIT_NonExistentPort(void);


/////////////////////////////////////////////////////////////

/******************************************************************************
Macro definitions
******************************************************************************/
#define MODULE_STOP_ENABLE    (1)
#define MODULE_STOP_DISABLE   (0)

#define MSTP_STATE_DMACDTC (MODULE_STOP_DISABLE)  /* DMAC/DTC trans to module-stop state  */
#define MSTP_STATE_EXDMAC  (MODULE_STOP_DISABLE)  /* EXDMAC trans to module-stop state  */
#define MSTP_STATE_RAM0    (MODULE_STOP_DISABLE)  /* RAM0 trans to module-stop state */
#define MSTP_STATE_RAM1    (MODULE_STOP_DISABLE)  /* RAM1 trans to module-stop state */

/*******************************************************************************
Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
void R_INIT_StopModule(void);

#endif /* CLOCK_H_ */
