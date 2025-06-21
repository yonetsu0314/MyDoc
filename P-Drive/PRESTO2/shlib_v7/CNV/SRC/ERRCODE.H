/*----------------------------------------------------------------------*
 * File name	: errcode.h	/					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/errcode.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: errcode.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#if	!defined(ERRCODE_H)
#define	ERRCODE_H

#ifdef	STORAGE
int		err_code(int err);
#else
extern int	err_code(int err);
#endif

#endif	/* ERRCODE_H */

