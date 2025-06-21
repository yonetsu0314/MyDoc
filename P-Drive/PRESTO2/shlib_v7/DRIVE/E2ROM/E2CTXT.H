/*----------------------------------------------------------------------*
 * File name	: e2ctxt.h	/ e2rom context header			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2ctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2ctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(E2CTXT_H)
#define	E2CTXT_H

#ifdef	STORAGE
E2PHY		*e2rom_phy;		/* EEPROM phygical addr		*/
E2WRT		interval;		/* write interval ??ms		*/

#else
extern E2PHY	*e2rom_phy;
extern E2WRT	interval;
#endif

#endif	/* E2CTXT_H */
