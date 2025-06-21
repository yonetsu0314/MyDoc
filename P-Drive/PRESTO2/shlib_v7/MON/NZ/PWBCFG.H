/*----------------------------------------------------------------------*
 * File name	: pwbcfg.h	/ congiguration header file		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/pwbcfg.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pwbcfg.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/pwbcfg.h 1.1 1996/07/22 19:26:02
 *Log: pwbcfg.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PWBCFG_H)
#define	PWBCFG_H

#ifdef	STORAGE
int		pwbid = 0;			/* ADC board ID number	*/
#else
extern int	pwbid;
#endif

#endif
