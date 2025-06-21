/*======================================================================
 * File name    : rzi025.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	平均値結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi025.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi025.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi025.hv  1.2  93/11/27 08:47:26  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi025.hv
 * Revision 1.2  93/11/27 08:47:26  ryoji
 * 個数を0.1個単位にする
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI025_H)
#define	RZI025_H


typedef struct rzi0250t {
	long		weight;		/* 重量				*/
	long		count;		/* 個数 0.1個単位		*/
} RZI0250T;

/*------------------------------------------------------*/
/*	平均値結果パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi025t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzi0250t	sect[8];	/* セクション演算結果		*/
	struct rzi0250t	ch;		/* チャネル演算結果		*/
} RZI025T;

#if	defined(STORAGE)
struct rzi025t	rzi025s;
#else
#if	!defined(NOEXTERN)
extern struct rzi025t	rzi025s;
#endif
#endif

#endif
