/*----------------------------------------------------------------------*
 * File name	: scuctxt.h	/ scu context header						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/m66230/RCS/scuctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/scush3/RCS/scuctxt.h 1.1 1970/01/01
 *Log: scuctxt.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/

#if	!defined(SCUCTXT_H)
#define	SCUCTXT_H

/*------------------------------------------------------*/
/*	SCUのコンテキスト構造体								*/
/*														*/
/*------------------------------------------------------*/

typedef struct	m66230_ctxt {
	int		ipfw;			/* ICU ipfw I/O address		*/
	int		io_base;		/* SCUﾎﾟｰﾄ･ｱﾄﾞﾚｽ			*/
} M66230_CTXT;

#ifdef	STORAGE
struct m66230_ctxt	m66230_ctxt_tbl[M66230_CH_MAX] = {
				0, 0,
				0, 0
				};
#else
extern struct m66230_ctxt	m66230_ctxt_tbl[];
#endif

#endif	/* SCUCTXT_H */
