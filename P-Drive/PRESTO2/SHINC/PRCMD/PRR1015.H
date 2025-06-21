/*======================================================================
 * File name	: prr1015.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ駆動特性データ構造 拡張パラメータ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr1015.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: prr1015.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR1015_H)
#define	PRR1015_H

typedef struct prr10150t {
	short	Ap;
	short	Aon;
	short	Bp;
	short	Bon;
} PRR10150T;

typedef struct prr10151t {
	struct prr10150t start;
	struct prr10150t stop;
} PRR10151T;


/*------------------------------------------------------*/
/*	フィーダ駆動特性拡張 パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr1015t {
	struct prr10151t	param[4];
} PRR1015T;

#if	defined(STORAGE)
struct prr1015t	prr1015s;
#else
#if	!defined(NOEXTERN)
extern struct prr1015t	prr1015s;
#endif
#endif

#endif
