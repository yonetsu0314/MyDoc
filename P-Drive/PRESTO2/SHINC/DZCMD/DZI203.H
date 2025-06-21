/*======================================================================
 * File name    : dzi203.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ係数データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzi203.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzi203.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZI203.Hv  1.2  94/02/18 16:42:12  ryoji
 *Log: P:/INCLUDE/DZCMD/RCS/DZI203.Hv
 * Revision 1.2  94/02/18 16:42:12  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZI203_H)
#define	DZI203_H

/*------------------------------------------------------*/
/*	AFV係数パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzi203t {
	unsigned char	wh_no;		/* 該当するWH番号		*/
	long		new_coeffi;	/* 新ＡＦＶ係数			*/
	long		old_coeffi;	/* 旧ＡＦＶ係数			*/
} DZI203T;

#if	defined(STORAGE)
struct dzi203t	dzi203s;
#else
#if	!defined(NOEXTERN)
extern struct dzi203t	dzi203s;
#endif
#endif

#endif
