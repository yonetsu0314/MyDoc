/*======================================================================
 * File name	: prr2023.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（２）データ構造
 *	特殊アベコン
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr2023.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr2023.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shinc/rzcmd/RCS/rzr223.h 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr223.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR2023_H)
#define	PRR2023_H

/*------------------------------------------------------*/
/*	特殊アベコンパラメータパケット			*/
/*------------------------------------------------------*/

typedef struct prr2023t {
	unsigned char	avg_cycle;	/* average cycle	*/
	unsigned char	avg_ratio;	/* average ratio	*/
	unsigned short	avg_reset_count;/* average reset count	*/
} PRR2023T;

#if	defined(STORAGE)
struct prr2023t	prr2023s;
#else
#if	!defined(NOEXTERN)
extern struct prr2023t	prr2023s;
#endif
#endif

#endif
