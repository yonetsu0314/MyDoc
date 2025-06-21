/*----------------------------------------------------------------------*
 * File name	: scua.h	/ scu bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/scua.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: scua.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/scua.h 1.2 1998/03/02 14:53:42 unknown
 *Log: scua.h
 * Revision 1.2  1998/03/02  14:53:42  unknown
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(SCU_H)
#define	SCU_H

#if	!defined(ADR_BASE)
#if	defined(BS16)
#define	ADR_BASE	2			/* 2 byte		*/
#else
#define	ADR_BASE	1			/* 1 byte		*/
#endif
#endif

/*	<<< SCU I/O Offset ADDRESS >>>		*/

#define	SRB	0			/* RX Data Buffer		*/
#define	STB	0			/* TX Data Buffer		*/
#define	SST	(1*ADR_BASE)		/* Serial Status register	*/
#define	SCM	(1*ADR_BASE)		/* Serial Command register	*/
#define	SMD	(2*ADR_BASE)		/* Serial Mode register		*/
#define	SIMK	(3*ADR_BASE)		/* Serial Interrupt Mask register*/


int *	_scu_init(int, int, int, int);
int	_scu_scm(int, int);

/*----------------------------------------------------------------------*/
/*	<<< return parameter >>>       					*/
					/* 戻り値の定義			*/
#define	BKD		(0x01)		/* ﾌﾞﾚｰｸ状態検出		*/
#define	FE		(0x10)		/* ﾌﾚｰﾐﾝｸﾞ･ｴﾗｰ発生		*/
#define	OVE		(0x20)		/* ｵｰﾊﾞｰﾗﾝ･ｴﾗｰ発生		*/
#define	PE		(0x08)		/* ﾊﾟﾘﾃｨｰ･ｴﾗｰ発生		*/
#define	RBRDY		(0x40)		/* TXﾊﾞｯﾌｧｰのﾘｰﾄﾞ要求		*/
#define	TBRDY		(0x80)		/* TXﾊﾞｯﾌｧｰ書き込み可		*/

int	_scu_sst(int);
/*----------------------------------------------------------------------*/

int	_scu_putc(int, int);

#endif
