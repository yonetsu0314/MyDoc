/*======================================================================
 * File name	: pri0029.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0029.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0029.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi029.h 1.1 1999/05/27 16:34:39 kawamura Exp
 *log: nzi029.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0029_H)
#define	PRI0029_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

typedef struct pri00290t {
	short		weight;		/* 重量				*/
	enum prprm003e	wh_status;	/* WHステータス			*/
	enum prprm003e	bh_status;	/* BHステータス			*/
} PRI00290T;

typedef struct pri00291t {
	unsigned char		nof_phead;	/* number of parent head*/
	struct pri00290t	phead[2];	/* parameter[nof_phead]	*/
} PRI00291T;

/*------------------------------------------------------*/
/*	組み合わせ計算結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0029t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct pri00291t	sect[8];/* セクション演算結果		*/
} PRI0029T;

#if	defined(STORAGE)
struct pri0029t	pri0029s;
#else
#if	!defined(NOEXTERN)
extern struct pri0029t	pri0029s;
#endif
#endif

#endif
