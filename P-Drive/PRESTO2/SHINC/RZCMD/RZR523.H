/*======================================================================
 * File name    : rzr523.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（５）データ構造
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr523.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr523.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr523.h 1.1 1999/07/07 16:59:03 itami
 *Log: rzr523.h
 * リビジョン 1.1  1999/07/07  16:59:03  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR523_H)
#define	RZR523_H

/*------------------------------------------------------*/
/*	拡張上下限値データ				*/
/*------------------------------------------------------*/
typedef struct rzr5230t {
	long		upper_wt;	/* 拡張上限重量			*/
	long		lower_wt;	/* 拡張下限重量			*/
	unsigned short	upper_cnt;	/* 拡張上限個数			*/
	unsigned short	lower_cnt;	/* 拡張下限個数			*/
} RZR5230T;

typedef struct rzr523t {
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr5230t	sect[8];	/* section paramter[nof_sect]	*/
} RZR523T;

#if	defined(STORAGE)
struct rzr523t	rzr523s;
#else
#if	!defined(NOEXTERN)
extern struct rzr523t	rzr523s;
#endif
#endif

#endif
