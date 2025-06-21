/*----------------------------------------------------------------------*
 * File name	: xam.h		/ xam bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/xam.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: xam.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/xam.h 1.1 1995/08/21 19:04:40 kabumoto
 *Log: xam.h
 * リビジョン 1.1  1995/08/21  19:04:40  kabumoto
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(XAM_H)
#define	XAM_H

int	_xam_init(void);
int	_xam_start(void);
int	_xam_stop(void);
int	_xam_test(void);

#endif	/* XAM_H */

