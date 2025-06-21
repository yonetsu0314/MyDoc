/*======================================================================
 * File name    : vecter.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/vecter.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: vecter.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/03/14 TRAPテーブルを16から32に変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO--CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	"vecter.h"
/****************************************************************/
/* ALLOCATE STACK AREA */
/****************************************************************/
#pragma section STK 		/* SECTION name "BSTK" */
	long stack[STACK_SIZE];
#pragma section
/****************************************************************/
/* ALLOCATE DEFINITION TABLE */
/****************************************************************/
void *const stacktbl[]={
	STACK_PON,
	STACK_MANUAL
};
/****************************************************************/
/* ALLOCATE VECTOR TABLE (EXPEVT or INTEVT CODE H'000-H'5a0) */
/****************************************************************/
void (*const vecttbl[])(void) = {
				/* EVT KIND CODE REG */
	RESET_PON, 		/* PON RESET H'000 EXPEVT */
	RESET_MANUAL, 		/* MANUAL RESET H'020 EXPEVT */
	TLB_MISS_READ, 		/* TLB MISS(R) H'040 EXPEVT */
	TLB_MISS_WRITE, 	/* TLB MISS(W) H'060 */
	TLB_1ST_PAGE, 		/* H'080 */
	TLB_PROTECT_READ, 	/* H'0a0 */
	TLB_PROTECT_WRITE, 	/* H'0c0 */
	ADR_ERROR_WRITE, 	/* H'0e0 */
	ADR_ERROR_WRITE, 	/* H'100 */
	RESERVED,		/* H'120 ------ */
	RESERVED,		/* H'140 ------ */
	TRAP,			/* H'160 (with TRA)*/
	ILLEGAL_INST,		/* H'180 EXPEVT */
	ILLEGAL_SLOT,		/* H'1a0 EXPEVT */
	NMI,			/* H'1c0 INTEVT */
	USER_BREAK,		/* H'1e0 EXPEVT */
	IRQ15,			/* H'200 INTEVT */
	IRQ14,			/* H'220 INTEVT */
	IRQ13,			/* H'240 INTEVT */
	IRQ12,			/* H'260 INTEVT */
	IRQ11,			/* H'280 INTEVT */
	IRQ10, 			/* H'2a0 INTEVT */
	IRQ9, 			/* H'2c0 INTEVT */
	IRQ8, 			/* H'2e0 INTEVT */
	IRQ7, 			/* H'300 INTEVT */
	IRQ6, 			/* H'320 INTEVT */
	IRQ5, 			/* H'340 INTEVT */
	IRQ4, 			/* H'360 INTEVT */
	IRQ3, 			/* H'380 INTEVT */
	IRQ2, 			/* H'3a0 INTEVT */
	IRQ1,			/* H'3c0 INTEVT */
	RESERVED, 		/* H'3e0 ------ */
	TMU0_TUNI0, 		/* H'400 INTEVT */
	TMU1_TUNI1,		/* H'420 INTEVT */
	TMU2_TUNI2,		/* H'440 INTEVT */
	TMU2_TICPI2,		/* H'460 INTEVT */
	RTC_ATI,		/* H'480 INTEVT */
	RTC_PRI,		/* H'4a0 INTEVT */
	RTC_CUI,		/* H'4c0 INTEVT */
	SCI_ERI,		/* H'4e0 INTRVT */
	SCI_RXI,		/* H'500 INTRVT */
	SCI_TXI,		/* H'520 INTRVT */
	SCI_TEI,		/* H'540 INTRVT */
	WDT_ITI,		/* H'560 INTEVT */
	REF_RCMI,		/* H'580 INTEVT */
	DEF_RPVI,		/* H'5a0 INTEVT */
	RESERVED,		/* H'5c0 INTEVT */
	RESERVED,		/* H'5e0 INTEVT */
	RESERVED,		/* H'600 INTEVT */
	RESERVED,		/* H'620 INTEVT */
	RESERVED,		/* H'640 INTEVT */
	RESERVED,		/* H'660 INTEVT */
	RESERVED,		/* H'680 INTEVT */
	RESERVED,		/* H'6a0 INTEVT */
	RESERVED,		/* H'6c0 INTEVT */
	RESERVED,		/* H'6e0 INTEVT */
	RESERVED,		/* H'700 INTEVT */
	RESERVED,		/* H'720 INTEVT */
	RESERVED,		/* H'740 INTEVT */
	RESERVED,		/* H'760 INTEVT */
	RESERVED,		/* H'780 INTEVT */
	RESERVED,		/* H'7a0 INTEVT */
	RESERVED,		/* H'7c0 INTEVT */
	RESERVED,		/* H'7e0 INTEVT */
	DMAC_DEI0,		/* H'800 INTEVT */
	DMAC_DEI1,		/* H'820 INTEVT */
	DMAC_DEI2,		/* H'840 INTEVT */
	DMAC_DEI3,		/* H'860 INTEVT */
	RESERVED,		/* H'880 INTEVT */
	RESERVED,		/* H'8a0 INTEVT */
	RESERVED,		/* H'8c0 INTEVT */
	RESERVED,		/* H'8e0 INTEVT */
	SCIF_ERI2,		/* H'900 INTEVT */
	SCIF_RXI2,		/* H'920 INTEVT */
	SCIF_BRI2,		/* H'940 INTEVT */
	SCIF_TXI2,		/* H'960 INTEVT */
	ADC_ADI,		/* H'980 INTEVT */
	RESERVED
};
/***************************************************************/
/* ALLOCATE INTERRUPT PRIORITY TABLE INTEVT H'1c0-H'5a0 */
/***************************************************************/
const char imasktbl[]={
	(char)(15<<4),		/* NMI level 16(IMASK=0-15) */
	(char)IP_RESERVED,	/* --------------- */
	(char)(15<<4),		/* IRQ15 (IRL0000) */
	(char)(14<<4),		/* IRQ14 (IRL0001) */
	(char)(13<<4),		/* IRQ13 (IRL0010) */
	(char)(12<<4),		/* IRQ12 (IRL0011) */
	(char)(11<<4),		/* IRQ11 (IRL0100) */
	(char)(10<<4),		/* IRQ10 (IRL0101) */
	(char)(9<<4),		/* IRQ9  (IRL0110) */
	(char)(8<<4),		/* IRQ8  (IRL0111) */
	(char)(7<<4),		/* IRQ7  (IRL1000) */
	(char)(6<<4),		/* IRQ6  (IRL1001) */
	(char)(5<<4),		/* IRQ5  (IRL1010) */
	(char)(4<<4),		/* IRQ4  (IRL1011) */
	(char)(3<<4),		/* IRQ3  (IRL1100) */
	(char)(2<<4),		/* IRQ2  (IRL1101) */
	(char)(1<<4),		/* IRQ1  (IRL1110) */
	(char)IP_RESERVED,	/* --------------- */
	(char)IP_TMU0,		/* TMU0 TUNI0      */
	(char)IP_TMU1,		/* TMU1 TUNI1      */
	(char)IP_TMU2,		/* TNU2 TUNI2      */
	(char)IP_TMU2,		/* TICPI2          */
	(char)IP_RTC,		/* RTC ATI         */
	(char)IP_RTC,
	(char)IP_RTC,
	(char)IP_SCI,		/* SCI ERI         */
	(char)IP_SCI,
	(char)IP_SCI,
	(char)IP_SCI,
	(char)IP_WDT,		/* WDT ITI         */
	(char)IP_REF,		/* REF RCMI        */
	(char)IP_REF,		/* REF ROVI        */
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_DMAC,		/* DMAC DEI0       */
	(char)IP_DMAC,
	(char)IP_DMAC,
	(char)IP_DMAC,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_RESERVED,
	(char)IP_SCIF,		/* SCIF ERI2       */
	(char)IP_SCIF,		/* SCIF RXI2       */
	(char)IP_SCIF,		/* SCIF BRI2       */
	(char)IP_SCIF,		/* SCIF TXI2       */
	(char)IP_ADC,		/* ADC ADI         */
	(char)IP_RESERVED,
};
void (*const traptbl[])(void) = {
	TRAPA_0,
	TRAPA_1,
	TRAPA_2,
	TRAPA_3,
	TRAPA_4,
	TRAPA_5,
	TRAPA_6,
	TRAPA_7,
	TRAPA_8,
	TRAPA_9,
	TRAPA_10,
	TRAPA_11,
	TRAPA_12,
	TRAPA_13,
	TRAPA_14,
	TRAPA_15,
	TRAPA_16,
	TRAPA_17,
	TRAPA_18,
	TRAPA_19,
	TRAPA_20,
	TRAPA_21,
	TRAPA_22,
	TRAPA_23,
	TRAPA_24,
	TRAPA_25,
	TRAPA_26,
	TRAPA_27,
	TRAPA_28,
	TRAPA_29,
	TRAPA_30,
	TRAPA_31
};

void	(*hard_int_tbl[])(void) = {
	RESET_PON, 		/* PON RESET H'000 EXPEVT */
	RESET_MANUAL, 		/* MANUAL RESET H'020 EXPEVT */
	TLB_MISS_READ, 		/* TLB MISS(R) H'040 EXPEVT */
	TLB_MISS_WRITE, 	/* TLB MISS(W) H'060 */
	TLB_1ST_PAGE, 		/* H'080 */
	TLB_PROTECT_READ, 	/* H'0a0 */
	TLB_PROTECT_WRITE, 	/* H'0c0 */
	ADR_ERROR_WRITE, 	/* H'0e0 */
	ADR_ERROR_WRITE, 	/* H'100 */
	RESERVED,		/* H'120 ------ */
	RESERVED,		/* H'140 ------ */
	TRAP,			/* H'160 (with TRA)*/
	ILLEGAL_INST,		/* H'180 EXPEVT */
	ILLEGAL_SLOT,		/* H'1a0 EXPEVT */
	NMI,			/* H'1c0 INTEVT */
	USER_BREAK,		/* H'1e0 EXPEVT */
	IRQ15,			/* H'200 INTEVT */
	IRQ14,			/* H'220 INTEVT */
	IRQ13,			/* H'240 INTEVT */
	IRQ12,			/* H'260 INTEVT */
	IRQ11,			/* H'280 INTEVT */
	IRQ10, 			/* H'2a0 INTEVT */
	IRQ9, 			/* H'2c0 INTEVT */
	IRQ8, 			/* H'2e0 INTEVT */
	IRQ7, 			/* H'300 INTEVT */
	IRQ6, 			/* H'320 INTEVT */
	IRQ5, 			/* H'340 INTEVT */
	IRQ4, 			/* H'360 INTEVT */
	IRQ3, 			/* H'380 INTEVT */
	IRQ2, 			/* H'3a0 INTEVT */
	IRQ1,			/* H'3c0 INTEVT */
	RESERVED, 		/* H'3e0 ------ */
	TMU0_TUNI0, 		/* H'400 INTEVT */
	TMU1_TUNI1,		/* H'420 INTEVT */
	TMU2_TUNI2,		/* H'440 INTEVT */
	TMU2_TICPI2,		/* H'460 INTEVT */
	RTC_ATI,		/* H'480 INTEVT */
	RTC_PRI,		/* H'4a0 INTEVT */
	RTC_CUI,		/* H'4c0 INTEVT */
	SCI_ERI,		/* H'4e0 INTRVT */
	SCI_RXI,		/* H'500 INTRVT */
	SCI_TXI,		/* H'520 INTRVT */
	SCI_TEI,		/* H'540 INTRVT */
	WDT_ITI,		/* H'560 INTEVT */
	REF_RCMI,		/* H'580 INTEVT */
	DEF_RPVI,		/* H'5a0 INTEVT */
	RESERVED,		/* H'5c0 INTEVT */
	RESERVED,		/* H'5e0 INTEVT */
	RESERVED,		/* H'600 INTEVT */
	RESERVED,		/* H'620 INTEVT */
	RESERVED,		/* H'640 INTEVT */
	RESERVED,		/* H'660 INTEVT */
	RESERVED,		/* H'680 INTEVT */
	RESERVED,		/* H'6a0 INTEVT */
	RESERVED,		/* H'6c0 INTEVT */
	RESERVED,		/* H'6e0 INTEVT */
	RESERVED,		/* H'700 INTEVT */
	RESERVED,		/* H'720 INTEVT */
	RESERVED,		/* H'740 INTEVT */
	RESERVED,		/* H'760 INTEVT */
	RESERVED,		/* H'780 INTEVT */
	RESERVED,		/* H'7a0 INTEVT */
	RESERVED,		/* H'7c0 INTEVT */
	RESERVED,		/* H'7e0 INTEVT */
	DMAC_DEI0,		/* H'800 INTEVT */
	DMAC_DEI1,		/* H'820 INTEVT */
	DMAC_DEI2,		/* H'840 INTEVT */
	DMAC_DEI3,		/* H'860 INTEVT */
	RESERVED,		/* H'880 INTEVT */
	RESERVED,		/* H'8a0 INTEVT */
	RESERVED,		/* H'8c0 INTEVT */
	RESERVED,		/* H'8e0 INTEVT */
	SCIF_ERI2,		/* H'900 INTEVT */
	SCIF_RXI2,		/* H'920 INTEVT */
	SCIF_BRI2,		/* H'940 INTEVT */
	SCIF_TXI2,		/* H'960 INTEVT */
	ADC_ADI,		/* H'980 INTEVT */
	RESERVED
};

