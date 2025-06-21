/*======================================================================
 * File name    : rzr217.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	拡張タイミングデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr217.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr217.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr217.h 1.2 1996/07/31 17:04:39 hiroe
 *Log: rzr217.h
 * リビジョン 1.2  1996/07/31  17:04:39  hiroe
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR217_H)
#define	RZR217_H

typedef struct rzr2171t {
	unsigned char	parent_delay;	/* 親ヘッド遅れ時間		*/
} RZR2171T;

typedef struct rzr2170t {
	unsigned char	nof_parent;	/* 該当するsectの親ヘッドの数	*/
	struct rzr2171t	pandc[2];	/* parent paramter[nof_parent]	*/
} RZR2170T;

/*------------------------------------------------------*/
/*	タイミングパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr217t {
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr2170t	sect[8];	/* section paramter[nof_sect]	*/
} RZR217T;

#if	defined(STORAGE)
struct rzr217t	rzr217s;
#else
#if	!defined(NOEXTERN)
extern struct rzr217t	rzr217s;
#endif
#endif

#endif
