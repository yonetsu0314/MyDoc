/*======================================================================
 * File name	: pri0001.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	秤診断データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0001.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0001.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi001.h 1.1 1999/05/27 16:35:04 kawamura Exp
 *log: nzi001.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0001_H)
#define	PRI0001_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

typedef struct pri00010t{
	short		weight;		/* ヘッド重量			*/
	enum prprm003e	status;		/* ヘッドステータス		*/
} PRI00010T;

/*------------------------------------------------------*/
/*	秤診断データパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0001t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	min_wh_no;	/* 最小WH番号			*/
	unsigned char	nof_wh;		/* 該当するchのWH数		*/
	struct pri00010t	wh_3rd[32];/* 3段目重量&ステータス[nof_wh]*/
	struct pri00010t	wh_4th[32];/* 4段目重量&ステータス[nof_wh]*/
	struct pri00010t	wh_5th[32];/* 5段目重量&ステータス[nof_wh]*/
	unsigned char	nof_ph;		/* 該当するchのPH数		*/
	struct pri00010t	ph_3rd[32];/* 3段目重量&ステータス[nof_ph]*/
	struct pri00010t	ph_4th[32];/* 4段目重量&ステータス[nof_ph]*/
	struct pri00010t	ph_5th[32];/* 5段目重量&ステータス[nof_ph]*/
	unsigned char	min_df_no;	/* 最小DF番号			*/
	unsigned char	nof_df;		/* 該当するchのDF数		*/
	struct pri00010t	df[8];	/* WH重量&ステータス[nof_df]	*/
	unsigned char	nof_afv;	/* ＡＦＶセル数			*/
	unsigned short	afv_out[16];	/* ＡＦＶセル出力		*/
} PRI0001T;

#if	defined(STORAGE)
struct pri0001t	pri0001s;
#else
#if	!defined(NOEXTERN)
extern struct pri0001t	pri0001s;
#endif
#endif

#endif
