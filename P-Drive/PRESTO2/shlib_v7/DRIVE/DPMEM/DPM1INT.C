/*----------------------------------------------------------------------*
 * File name	: dpm1int.c	/ dpmem RX interrupt handler		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpm1int.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpm1int.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpm1int.c 1.1 1993/10/26 10:52:56 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	DP-SRAMの受信割り込みﾊﾝﾄﾞﾗ。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	無し	:							*/
/*									*/
/* 戻り値								*/
/*	無し	:							*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\typedefs.h>		/* Show custom		*/
#include	"dpmctl.h"			/* show current		*/
#include	"dpmctxt.h"			/*	"		*/

#define	CH_NO	1				/* channel number	*/

//#pragma interrupt(dpm1_int_hdr)

void dpm1_int_hdr(void)
{
	volatile unsigned char		*cmd;	/* command		*/
	unsigned short			i;

	cmd = dpm_ctxt_tbl[CH_NO].iobase + dpm_ctxt_tbl[CH_NO].rdcmd;
	if(dpm_ctxt_tbl[CH_NO].mem_type == BYTE_M)
		i = *cmd;
	else {
		do {
			i = *WD_PTR(cmd);
		} while(i != *WD_PTR(cmd));
	}
	WUP_TSK(dpm_rx_tsk[CH_NO][i].acadr);
	return;
}
