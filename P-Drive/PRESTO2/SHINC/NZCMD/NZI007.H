/*======================================================================
 * File name    : nzi007.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzi007.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzi007.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzi007.h 1.1 1999/05/27 16:34:34 kawamura
 *Log: nzi007.h
 * リビジョン 1.1  1999/05/27  16:34:34  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZI007_H)
#define	NZI007_H


enum nzi0070e {				/* 状態				*/
	MFS      = 0x0001,		/* マイナス側フルスケール	*/
	PFS      = 0x0002,		/* プラス側フルスケール		*/
	UNSTABLE = 0x0004		/* 不安定			*/
};

typedef struct nzi0070t {
	short		weight;		/* 重量データ		*/
	enum nzi0070e	status;		/* 重量ステータス	*/
} NZI0070T;

/*------------------------------------------------------*/
/*	重量波形データの応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct nzi007t {
	unsigned char	wh_no;		/* WH番号		*/
	unsigned short	index;		/* データのインデックス	*/
	unsigned char	nof_data;	/* データの数		*/
	struct nzi0070t	data[32];	/* データ		*/
} NZI007T;

#if	defined(STORAGE)
struct nzi007t	nzi007s;
#else
#if	!defined(NOEXTERN)
extern struct nzi007t	nzi007s;
#endif
#endif

#endif
