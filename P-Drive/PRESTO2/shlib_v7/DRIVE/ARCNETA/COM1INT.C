/*----------------------------------------------------------------------*
 * File name	: com1int.c	/ com20020 interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/com1int.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: com1int.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/11/13 12:30
 * 不要な命令を削除する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/com1int.c 1.1 1995/11/07 17:13:00 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	割り込みハンドラ。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	"comctxt.h"			/* Show current		*/
#include	"comctl.h"			/*	"		*/
#include	"comrx.h"			/*	"		*/

#define	DEVNO	1				/* device number	*/

//#pragma interrupt(arcnet1_int_hdr)
void arcnet1_int_hdr(void)
{
	struct com_ctxt		*pc;		/* pointer to context	*/
	struct rxh		*ph;		/* pointer to RX header	*/
	unsigned char		st;		/* status register	*/
	int			adr;

	pc = &com_ctxt_tbl[DEVNO];
	ph = &com_rxhd[DEVNO];
	st = inp(com_ctxt_tbl[DEVNO].iobase+STATR);
	outp(pc->iobase+INTMR, 0);
	if(st & RI) {
		pc->reg[INTMR] &= (unsigned char)(~RI);
		adr = _arcnet_inp2(pc, ADRHR)
		      &(((RDDATA|AUTOINC|ADRH_MAX)<<8)|(ADRL_MAX));
		arc_ena_rx(pc, ph);
		_arcnet_outp2(pc, ADRHR, (unsigned short)adr);
		WUP_TSK(com_rx_tsk[DEVNO]->acadr);
	}
	if(st & RECON) {
		_arcnet_outp(pc, COMMR, CLR_RECON);
		if ( com_hd_tbl[DEVNO].recon_cntr+1 )
			++com_hd_tbl[DEVNO].recon_cntr;
	}
	outp(pc->iobase+INTMR, pc->reg[INTMR]);
//	outp(pc->ipfw, FI_COM);

	return;
}
