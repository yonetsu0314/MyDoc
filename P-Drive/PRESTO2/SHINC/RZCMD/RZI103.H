/*======================================================================
 * File name    : rzi103.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計算結果データ構造（拡張２）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi103.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi103.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi103.h 1.1 1996/07/15 10:10:36 hiroki
 *Log: rzi103.h
 * リビジョン 1.1  1996/07/15  10:10:36  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI103_H)
#define	RZI103_H

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi103t {
	unsigned char	dump_cnt;	/* 回数計量カウント		*/
	unsigned char	dump_flag;	/* 排出フラグ　　　　　　　	*/
} RZI103T;

#if	defined(STORAGE)
struct rzi103t	rzi103s;
#else
#if	!defined(NOEXTERN)
extern struct rzi103t	rzi103s;
#endif
#endif

#endif
