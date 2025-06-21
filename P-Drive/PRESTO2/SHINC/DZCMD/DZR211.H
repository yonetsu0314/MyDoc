/*======================================================================
 * File name    : dzi211.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ヘッドの重心座標のデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzr211.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr211.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZR211.Hv  1.2  94/03/18 16:17:34  ryoji
 *Log: P:/INCLUDE/DZCMD/RCS/DZR211.Hv
 * Revision 1.2  94/03/18 16:17:34  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZR211_H)
#define	DZR211_H

/*------------------------------------------------------*/
/*	ヘッドの重心の座標パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzr211t {
	unsigned char	pwb_id;		/* ＡＤＣボード識別番号		*/
	short		coord_x[20];	/* 計量器とAFVの重心のＸ座標	*/
	short		coord_y[20];	/* 計量器とAFVの重心のＹ座標	*/
	long		afv_coeffi[16];	/* ＡＦＶ係数			*/
} DZR211T;

#if	defined(STORAGE)
struct dzr211t	dzr211s;
#else
#if	!defined(NOEXTERN)
extern struct dzr211t	dzr211s;
#endif
#endif

#endif
