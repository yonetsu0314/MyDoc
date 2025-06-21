/*======================================================================
 * File name    : nzi029.h
 * Original	: rzi029.h
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi029.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi029.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi029.h 1.1 1999/05/27 16:34:39 kawamura
 *Log: nzi029.h
 * リビジョン 1.1  1999/05/27  16:34:39  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI029_H)
#define	NZI029_H

typedef struct nzi0290t {
	short	weight;		/* 重量				*/
	short	wh_status;	/* WHステータス	 		*/
	short	bh_status;	/* BHステータス 		*/
} NZI0290T;

typedef struct nzi0291t {
	unsigned char	nof_phead;	/* number of parent head	*/
	struct nzi0290t	phead[2];	/* PI291 paramter[nof_phead]	*/
} NZI0291T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi029t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct nzi0291t	sect[8];	/* セクション演算結果		*/
} NZI029T;

#if	defined(STORAGE)
struct nzi029t	nzi029s;
#else
#if	!defined(NOEXTERN)
extern struct nzi029t	nzi029s;
#endif
#endif

#endif
