/*======================================================================
 * File name    : rzi029.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi029.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi029.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzi029.h 1.1 1994/02/18 13:45:44 ryoji
 *Log: rzi029.h
 * リビジョン 1.1  1994/02/18  13:45:44  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI029_H)
#define	RZI029_H

typedef struct rzi0290t {
	short		weight;		/* 重量				*/
	unsigned char	wh_status;	/* WHステータス	 		*/
	unsigned char	bh_status;	/* BHステータス 		*/
} RZI0290T;

typedef struct rzi0291t {
	unsigned char	nof_phead;	/* number of parent head	*/
	struct rzi0290t	phead[2];	/* PI291 paramter[nof_phead]	*/
} RZI0291T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi029t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzi0291t	sect[8];	/* セクション演算結果		*/
} RZI029T;

#if	defined(STORAGE)
struct rzi029t	rzi029s;
#else
#if	!defined(NOEXTERN)
extern struct rzi029t	rzi029s;
#endif
#endif

#endif
