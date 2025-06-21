/*----------------------------------------------------------------------*
 * File name	: tmrdrv.h	/ timer driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/tmrdrv.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: tmrdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/07/25  10:14
 * 関数tmr5_event,tmr10_eventの第２引き数の型宣言ミスを修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(TMRDRV_H)
#define	TMRDRV_H

enum {
	TMR5ID = 0,			/* 5msﾀｲﾏｰのID番号		*/
	TMR10ID,			/* 10msﾀｲﾏｰのID番号		*/
	TMID_MAX
};

#define	MAX_ELEMENT	6		/* ﾀｲﾏｰのｱｲﾃﾑ数の総和(5ms,10ms)	*/
#define	TMR_MAX		32		/* 1ｱｲﾃﾑのﾀｲﾏｰの最大数		*/


int		tmr_sys_init(int, int *, int *, unsigned short **);
void		tmr5_event(int, unsigned long *);
void		tmr10_event(int, unsigned long *);

#endif	/* TMRDRV_H */

