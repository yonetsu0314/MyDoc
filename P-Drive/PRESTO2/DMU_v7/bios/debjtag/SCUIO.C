/*----------------------------------------------------------------------*
 * File name	: scuio.asm	/ scu io svc handler						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/scuio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.							*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*
 *		PHONE (0775) 53-4141											*
 *----------------------------------------------------------------------*/
#include <intrpt.h>
#include "dmu5562.h"

#define	SCU_DEV_MAX	(2)

#define		TIE_BP		0x80
#define		RIE_BP		0x40
#define		TXE_BP		0x20
#define		RXE_BP		0x10

#define		BIT8		0x00
#define		BIT7		0x40
#define		STOP2		0x08
#define		STOP1		0x00
#define		NON			0x00
#define		EVEN		0x20
#define		ODD			0x30

static const unsigned char	chr_length[] = {				/* ｷｬﾗｸﾀ長のﾃｰﾌﾞﾙ			*/
			(unsigned char)BIT7, (unsigned char)BIT8
};
static const unsigned char	stp_length[] = {				/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ長のﾃｰﾌﾞﾙ		*/
			(unsigned char)STOP1, (unsigned char)STOP2
};
static const unsigned char	parity[] = {					/* ﾊﾟﾘﾃｨ選択ﾃｰﾌﾞﾙ			*/
			(unsigned char)NON, (unsigned char)ODD, (unsigned char)EVEN
};
struct cksci {
	unsigned char	n;										/* ｸﾛｯｸｿｰｽ/1/4/16/64		*/
	unsigned char	br;										/* ﾋﾞｯﾄﾚｰﾄﾚｼﾞｽﾀ/N			*/
}
static const bsci[] = {										/* baud rate 設定用テーブル */
			{(unsigned char)1,	(unsigned char)383},		/* 0 :	  1200 BPS			*/
			{(unsigned char)0,	(unsigned char)383},		/* 1 :	  2400 BPS			*/
			{(unsigned char)0,	(unsigned char)191},		/* 2 :	  4800 BPS			*/
			{(unsigned char)0,	(unsigned char)95},			/* 3 :	  9600 BPS			*/
			{(unsigned char)0,	(unsigned char)47},			/* 4 :	 19.2K BPS			*/
			{(unsigned char)0,	(unsigned char)23},			/* 5 :	 38.4K BPS			*/
			{(unsigned char)0,	(unsigned char)15},			/* 6 :	 57.6K BPS			*/
			{(unsigned char)0,	(unsigned char)7}			/* 7 :	 115.2K BPS			*/
};

static unsigned char baudrate[SCU_DEV_MAX];

void	scu_io(void);
int		*scu_io_init(int, int, int,int);
int		scu_io_scm(int,int);
int		scu_io_sst(int);
int		scu_io_putc(int,int);
int		scu_io_baud(int,int);

#pragma	inline_asm(scu_io)
static void scu_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_scu,R2  ;arrangement Top Address
          ADD         R2,R0        ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          scu_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
scu_exit:
          MOV.L       Lrsxxx,R0   ;scu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _scu_io_err

          .ALIGN 4
jt_scu:
          .DATA.L     _scu_io_init  ; r0 = 0
          .DATA.L     _scu_io_scm   ;    = 1
          .DATA.L     _scu_io_sst   ;    = 2
          .DATA.L     _scu_io_putc  ;    = 3
          .DATA.L     _scu_io_baud  ;    = 4
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int scu_io_err(){
	rsstr("*** scu_io err ***");
	return(0);
}

int *
scu_io_init(
	int	ch,
	int	char_len,			/* ｷｬﾗｸﾀ長			*/
	int	stp_len,			/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int	pari_sel			/* ﾊﾟﾘﾃｨｰ選択		*/
)
{
	volatile struct st_scif	*p;

	disable_interrupt();
	if(ch == 0){
		sfr_clrw(INTC_VCRL,0xffffff00);
		sfr_setw(INTC_VCRL,(INT_ERI1)<<8);			/* ERI */

		sfr_clrw(INTC_VCRL,0x00ff);
		sfr_setw(INTC_VCRL,INT_RXI1);				/* RXI */

		sfr_clrw(INTC_VCRM,0xffffff00);
		sfr_setw(INTC_VCRM,(INT_BRI1)<<8);			/* BRI */

		sfr_clrw(INTC_VCRM,0x00ff);
		sfr_setw(INTC_VCRM,INT_TXI1);				/* TXI */

		sfr_clrw(INTC_IPRD,0x000f);
		sfr_setw(INTC_IPRD,0x0003);					/* SCIF1 interrupt level 3 */
		sfr_out((unsigned)SCIF_SCFCR1,(UB)0x30);	/* initialize FIFO */
		sfr_out((unsigned)SCIF_SCSCR1,(UB)0xF0);	/* initialize RIE and TIE */

		p = &PSCIF1;
		p->SCSCR.BYTE	= (unsigned char)0;			/* 調歩同期モード、内部clk、SCK入力	*/
		p->SCBRR		= (unsigned char)bsci[baudrate[0]].br;
		p->SCSMR.BYTE	= (unsigned char)(parity[pari_sel] | stp_length[stp_len]
												| chr_length[char_len] | bsci[baudrate[0]].n);
		p->SCSCR.BYTE	= (unsigned char)(RIE_BP|TXE_BP|RXE_BP);	/* RxINT ,Tx/Rx enable */
		p->SC1SSR.BIT.RDF = 0;
	} else {
		sfr_clrw(INTC_VCRN,0xffffff00);
		sfr_setw(INTC_VCRN,(INT_ERI2)<<8);			/* ERI */

		sfr_clrw(INTC_VCRN,0x00ff);
		sfr_setw(INTC_VCRN,INT_RXI2);				/* RXI */

		sfr_clrw(INTC_VCRO,0xffffff00);
		sfr_setw(INTC_VCRO,(INT_BRI2)<<8);			/* BRI */

		sfr_clrw(INTC_VCRO,0x00ff);
		sfr_setw(INTC_VCRO,INT_TXI2);				/* TXI */

		sfr_clrw(INTC_IPRE,0xfffff000);
		sfr_setw(INTC_IPRE,0x3000);					/* SCIF2 interrupt level 3 */
		sfr_out((unsigned)SCIF_SCFCR2,(UB)0x30);	/* initialize FIFO */
		sfr_out((unsigned)SCIF_SCSCR2,(UB)0xF0);	/* initialize RIE and TIE */

		p = &PSCIF2;
		p->SCSCR.BYTE	= (unsigned char)0;			/* 調歩同期モード、内部clk、SCK入力	*/
		p->SCBRR		= (unsigned char)bsci[baudrate[1]].br;
		p->SCSMR.BYTE	= (unsigned char)(parity[pari_sel] | stp_length[stp_len]
												| chr_length[char_len] | bsci[baudrate[1]].n);
		p->SCSCR.BYTE	= (unsigned char)(RIE_BP|TXE_BP|RXE_BP);	/* RxINT ,Tx/Rx enable */
		p->SC1SSR.BIT.RDF = 0;
	}
	enable_interrupt();
	return((int *)p);
}

int
scu_io_scm(
	int	ch,
	int	command
)
{
	if(ch == 0){
		if(command & 0x0001) sfr_set(SCIF_SCSCR1,0x20);	/* enable TX */
		else                 sfr_clr(SCIF_SCSCR1,0x20);
		if(command & 0x0002) sfr_set(SCIF_SCSCR1,0x10);	/* enable RX */
		else                 sfr_clr(SCIF_SCSCR1,0x10);
	} else {
		if(command & 0x0001) sfr_set(SCIF_SCSCR2,0x20);	/* enable TX */
		else                 sfr_clr(SCIF_SCSCR2,0x20);
		if(command & 0x0002) sfr_set(SCIF_SCSCR2,0x10);	/* enable RX */
		else                 sfr_clr(SCIF_SCSCR2,0x10);
	}
	return(0);
}

int
scu_io_sst(
	int	ch
)
{
	int	data;

	if(ch == 0){
		data = sfr_inw(SCIF_SC1SSR1) & 0x00ff;
	} else {
		data = sfr_inw(SCIF_SC1SSR2) & 0x00ff;
	}
	return(data);
}

int
scu_io_putc(
	int	ch,
	int	data
)
{
	
	if(ch == 0){
		sfr_set(SCIF_SCSCR1,0x70);
		sfr_out((unsigned)SCIF_SCFTDR1,(UB)data);
		sfr_clrw(SCIF_SC1SSR1,0x0020);		/* TDFE */
		sfr_set(SCIF_SCSCR1,0x80);			/* initialize TIE */
	} else {
		sfr_set(SCIF_SCSCR2,0x70);
		sfr_out((unsigned)SCIF_SCFTDR2,(UB)data);
		sfr_clrw(SCIF_SC1SSR2,0x0020);		/* TDFE */
		sfr_set(SCIF_SCSCR2,0x80);			/* initialize TIE */
	}
	return(0);
}

int
scu_io_baud(
	int	ch,
	int	baud
)
{
	baudrate[ch] = (unsigned char)baud;
	return(0);
}
