/*======================================================================
 * File name    : rzr159.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ＢＨオーバースケール重量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr159.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr159.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr159.h 1.2 1996/05/09 19:13:59 hiroe
 *Log: rzr159.h
 * リビジョン 1.2  1996/05/09  19:13:59  hiroe
 * bh_reject_weight の型を変更
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR159_H)
#define	RZR159_H

typedef struct rzr1590t {
	long		bh_reject_wt;	/* ＢＨ排出重量			*/
} RZR1590T;

typedef struct rzr1591t {
	unsigned char	nof_phead;	/* number of parent head	*/
	struct rzr1590t	phead[2];	/* PI591 paramter[nof_phead]	*/
} RZR1591T;

/*------------------------------------------------------*/
/*	親ＢＨオーバースケール重量のパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr159t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr1591t	sect[8];	/* PR592 paramter[nof_sect]	*/
} RZR159T;

#if	defined(STORAGE)
struct rzr159t	rzr159s;
#else
#if	!defined(NOEXTERN)
extern struct rzr159t	rzr159s;
#endif
#endif

#endif
