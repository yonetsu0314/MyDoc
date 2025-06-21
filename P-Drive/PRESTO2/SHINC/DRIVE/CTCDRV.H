/*----------------------------------------------------------------------*
 * File name	: ctcdrv.h	/ ctc driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/ctcdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: ctcdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/rcs/ctcdrv.h 1.2 1994/08/18 14:41:51 sato
 *Log: ctcdrv.h
 * リビジョン 1.2  1994/08/18  14:41:51  sato
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(CTCDRV_H)
#define	CTCDRV_H
#include	<biosif\ctc.h>			/* Show biosif		*/

/*------------------------------------------------------*/
/*	CTCドライバーで管理可能な割り込み		*/
/*							*/
/*------------------------------------------------------*/

enum ctcip {
 	CTCINT0,			/* CTC Interrupt no.0		*/
 	CTCINT1,			/*   "		    1		*/
 	CTCINT2,			/*   "	 	    2		*/
	CTCINT_MAX			/* 使用可能な割り込み最大数	*/
};

void	ctc0_int_hdr(void);		/* ctc #0 interrupt handler	*/
void	ctc1_int_hdr(void);		/* ctc #1 interrupt handler	*/
void	ctc2_int_hdr(void);		/* ctc #2 interrupt handler	*/

int		ctc_sys_init(enum ctcip, enum ctcm, union aap *);

#endif
