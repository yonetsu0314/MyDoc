/*----------------------------------------------------------------------
 * File name	: vecter.h	/ 					
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/vecter.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: vecter.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/02/14 from_io m66230_io を追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *----------------------------------------------------------------------
 * Function								
 *----------------------------------------------------------------------
 */
#ifndef	VECTER_H
#define	VECTER_H
/*****************************************************************/
/* STACK SIZE definition */
/*****************************************************************/
#define STACK_SIZE	(0xd0000/4) 		/* 4096 byte */
#define STACK_PON 	(&stack[STACK_SIZE])
#define STACK_MANUAL 	(&stack[STACK_SIZE])
extern long stack[];
/*****************************************************************/
/* RESET FUNCTION definition */
/*****************************************************************/
#define RESET_PON 	init_setup /* PON RESET H'000 EXPEVT */
#define RESET_MANUAL 	init_setup /* MANUAL RESET H'020 EXPEVT */
/******************************************************************/
/* INTERRUPT FUNCTION definition */
/******************************************************************/
#define TLB_MISS_READ		halt	/* TLB MISS(R) H'040 EXPEVT */
#define TLB_MISS_WRITE		halt	/* TLB MISS(W) H'060 EXPEVT */
#define TLB_1ST_PAGE		halt	/* H'080 EXPEVT */
#define TLB_PROTECT_READ	halt	/* H'0a0 EXPEVT */
#define TLB_PROTECT_WRITE 	halt	/* H'0c0 EXPEVT */
#define ADR_ERROR_WRITE		halt	/* H'0e0 EXPEVT */
#define ADR_ERROR_WRITE		halt	/* H'100 EXPEVT */
/*#define RESERVED halt */ /* H'120 ------ */
/*#define RESERVED halt */ /* H'140 ------ */
#define TRAP			_trap	/* H'160 (with TRA)*/
#define ILLEGAL_INST		halt	/* H'180 EXPEVT */
#define ILLEGAL_SLOT		halt	/* H'1a0 EXPEVT */
#define NMI			halt	/* H'1c0 INTEVT */
#define USER_BREAK		halt	/* H'1e0 EXPEVT */
#define IRQ15			halt	/* H'200 INTEVT */
#define IRQ14			halt	/* H'220 INTEVT */
#define IRQ13			halt	/* H'240 INTEVT */
#define IRQ12			halt	/* H'260 INTEVT */
#define IRQ11			halt	/* H'280 INTEVT */
#define IRQ10			halt	/* H'2a0 INTEVT */
#define IRQ9			halt	/* H'2c0 INTEVT */
#define IRQ8			halt	/* H'2e0 INTEVT */
#define IRQ7			halt	/* H'300 INTEVT */
#define IRQ6			halt	/* H'320 INTEVT */
#define IRQ5			halt	/* H'340 INTEVT */
#define IRQ4			halt	/* H'360 INTEVT */
#define IRQ3			halt	/* H'380 INTEVT */
#define IRQ2			halt	/* H'3a0 INTEVT */
#define IRQ1			halt	/* H'3c0 INTEVT */
/*#define RESERVED 		halt*/	/* H'3e0 ------ */
#define TMU0_TUNI0		halt	/* H'400 INTEVT */
#define TMU1_TUNI1		halt	/* H'420 INTEVT */
#define TMU2_TUNI2		halt	/* H'440 INTEVT */
#define TMU2_TICPI2		halt	/* H'460 INTEVT */
#define RTC_ATI			halt	/* H'480 INTEVT */
#define RTC_PRI			halt	/* H'4a0 INTEVT */
#define RTC_CUI			halt	/* H'4c0 INTEVT */
#define SCI_ERI			halt	/* H'4e0 INTRVT */
#define SCI_RXI			halt	/* H'500 INTRVT */
#define SCI_TXI			halt	/* H'520 INTRVT */
#define SCI_TEI			halt	/* H'540 INTRVT */
#define WDT_ITI			halt	/* H'560 INTEVT */
#define REF_RCMI		halt	/* H'580 INTEVT */
#define DEF_RPVI		halt	/* H'5a0 INTEVT */
/*#define RESERVED		halt*/	/* H'5c0 INTEVT */
/*#define DEF_H-UDI		halt*/	/* H'5e0 INTEVT */
/*#define RESERVED		halt*/	/* H'600 INTEVT */
/*#define RESERVED		halt*/	/* H'620 INTEVT */
/*#define RESERVED		halt*/	/* H'640 INTEVT */
/*#define RESERVED		halt*/	/* H'660 INTEVT */
/*#define IREQ4			halt*/	/* H'680 INTEVT */
/*#define IREQ5			halt*/	/* H'6a0 INTEVT */
/*#define RESERVED		halt*/	/* H'6c0 INTEVT */
/*#define RESERVED		halt*/	/* H'6e0 INTEVT */
/*#define RESERVED		halt*/	/* H'700 INTEVT */
/*#define RESERVED		halt*/	/* H'720 INTEVT */
/*#define RESERVED		halt*/	/* H'740 INTEVT */
/*#define RESERVED		halt*/	/* H'760 INTEVT */
/*#define RESERVED		halt*/	/* H'780 INTEVT */
/*#define RESERVED		halt*/	/* H'7a0 INTEVT */
/*#define RESERVED		halt*/	/* H'7c0 INTEVT */
/*#define RESERVED		halt*/	/* H'7e0 INTEVT */
#define DMAC_DEI0		halt	/* H'800 INTEVT */
#define DMAC_DEI1		halt	/* H'820 INTEVT */
#define DMAC_DEI2		halt	/* H'840 INTEVT */
#define DMAC_DEI3		halt	/* H'860 INTEVT */
/*#define RESERVED		halt*/	/* H'880 INTEVT */
/*#define RESERVED		halt*/	/* H'8a0 INTEVT */
/*#define RESERVED		halt*/	/* H'8c0 INTEVT */
/*#define RESERVED		halt*/	/* H'8e0 INTEVT */
#define SCIF_ERI2		halt	/* H'900 INTEVT */
#define SCIF_RXI2		halt	/* H'920 INTEVT */
#define SCIF_BRI2		halt	/* H'940 INTEVT */
#define SCIF_TXI2		halt	/* H'960 INTEVT */
#define ADC_ADI			halt	/* H'980 INTEVT */
#define RESERVED		halt

extern void init_setup(void);
extern void halt(void);
extern void _trap(void);

/********************************************************************/
/* INTERRUPT MASK definition */
/********************************************************************/
#define IP_TMU0		(0<<4)
#define IP_TMU1		(0<<4)
#define IP_TMU2		(0<<4)
#define IP_RTC		(0<<4)
#define IP_SCI		(0<<4)
#define IP_WDT		(0<<4)
#define IP_REF		(0<<4)
#define IP_SCIF		(0<<4)
#define IP_ADC		(0<<4)
#define IP_DMAC		(0<<4)
#define IP_RESERVED	(15<<4)

/********************************************************************/
/* IPRA,IPRB definition */
/********************************************************************/
#define WORD_IPRA ((IP_TMU0<<12)|(IP_TMU1<<8)|(IP_TMU2<<4)|IP_RTC)
#define WORD_IPRB ((IP_WDT<<12)|(IP_REF<<8)|(IP_SCI<<4)|0)
extern void set_ip(void);
extern long stack[];

/********************************************************************/
/* TRAPA system call definition */
/********************************************************************/
extern	void scu_io(void);
extern	void tcu_io(void);
extern	void pio_io(void);
extern	void icu_io(void);
extern	void e2rom_io(void);
extern	void dpmem_io(void);
extern	void com_io(void);
extern	void sram_io(void);
extern	void from_io(void);
extern	void fpga_io(void);
extern	void m66230_io(void);

#define TRAPA_0 scu_io
#define TRAPA_1 tcu_io
#define TRAPA_2 m66230_io
#define TRAPA_3 pio_io
#define TRAPA_4 dpmem_io
#define TRAPA_5 icu_io
#define TRAPA_6 e2rom_io
#define TRAPA_7 dpmem_io
#define TRAPA_8 halt
#define TRAPA_9 halt
#define TRAPA_10 sram_io
#define TRAPA_11 from_io
#define TRAPA_12 com_io
#define TRAPA_13 halt
#define TRAPA_14 halt
#define TRAPA_15 halt 	/*#15(#0F) should be Exception routine(Illegal use )*/
#define TRAPA_16 halt
#define TRAPA_17 halt
#define TRAPA_18 halt
#define TRAPA_19 halt
#define TRAPA_20 halt
#define TRAPA_21 halt
#define TRAPA_22 fpga_io
#define TRAPA_23 halt
#define TRAPA_24 halt
#define TRAPA_25 halt
#define TRAPA_26 halt
#define TRAPA_27 halt
#define TRAPA_28 halt
#define TRAPA_29 halt
#define TRAPA_30 halt
#define TRAPA_31 halt

#endif
