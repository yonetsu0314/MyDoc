/*======================================================================
 * File name    : prr5023.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（５）データ構造
 *	拡張上下限値データ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr5023.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr5023.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shinc/rzcmd/RCS/rzr523.h 1.1 1970/01/01 00:00:00
 *Log: rzr523.h
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR5023_H)
#define	PRR5023_H

/*------------------------------------------------------*/
/*	拡張上下限値データ				*/
/*------------------------------------------------------*/
typedef struct prr50230t {
	long		upper_wt;		/* 拡張上限重量			*/
	long		lower_wt;		/* 拡張下限重量			*/
	unsigned short	upper_cnt;		/* 拡張上限個数			*/
	unsigned short	lower_cnt;		/* 拡張下限個数			*/
} PRR50230T;

typedef struct prr5023t {
	unsigned char		nof_sect;	/* 該当するchのセクション数	*/
	struct prr50230t	sect[8];	/* section paramter[nof_sect]	*/
} PRR5023T;

#if	defined(STORAGE)
struct prr5023t	prr5023s;
#else
#if	!defined(NOEXTERN)
extern struct prr5023t	prr5023s;
#endif
#endif

#endif
