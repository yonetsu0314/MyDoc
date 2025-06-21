/*----------------------------------------------------------------------*
 * File name	: frctxt.h	/ flash rom context header		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(FRCTXT_H)
#define	FRCTXT_H

#ifdef	STORAGE
FRPHY		*flashrom_phy;		/* FLASH ROM phygical addr	*/
FRWRT		interval;		/* write interval ??ms		*/

#else
extern FRPHY	*flashrom_phy;
extern FRWRT	interval;
#endif

#endif	/* FRCTXT_H */
