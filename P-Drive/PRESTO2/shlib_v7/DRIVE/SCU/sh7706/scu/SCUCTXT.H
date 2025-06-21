/*----------------------------------------------------------------------*
 * File name	: scuctxt.h	/ scu context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scu/RCS/scuctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_scu/rcs/scuctxt.h 1.1 1994/09/05 14:11:06 kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(SCUCTXT_H)
#define	SCUCTXT_H

/*------------------------------------------------------*/
/*	SCUのコンテキスト構造体				*/
/*							*/
/*------------------------------------------------------*/

typedef struct	scu_ctxt {
	int		ipfw;		/* ICU ipfw I/O address		*/
	int		io_base;	/* SCUﾎﾟｰﾄ･ｱﾄﾞﾚｽ		*/
} SCU_CTXT;

#ifdef	STORAGE
struct scu_ctxt	scu_ctxt_tbl[SCU_CH_MAX] = {
				0, 0,
				0, 0
				};
#else
extern struct scu_ctxt	scu_ctxt_tbl[];
#endif

#endif	/* SCUCTXT_H */
