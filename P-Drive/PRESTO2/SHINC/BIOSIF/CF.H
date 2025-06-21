/*----------------------------------------------------------------------*
 * File name	: cf.h	/ cf bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/rcs/cf.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cf.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2005.03.18 _cd_chk追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * cf_baseの定義追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04.12.11 新規追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(CF_H)
#define	CF_H

/*	<<< return parameter >>>					*/

typedef struct cfpinf {
	void		*comm;	/* cf physical address	*/
	void		*attr;	/* cf physical address	*/
	void		*io;	/* cf physical address	*/
} CFPINF;

#ifdef	STORAGE
CFPINF	cf_base;
#else
extern CFPINF	cf_base;
#endif

const CFPINF *_cf_init( int );
int _cd_chk( int );

#endif	/* CF_H */

